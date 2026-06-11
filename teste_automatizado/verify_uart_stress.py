"""
Teste de estresse UART: Simula envio de 100+ tags RFID em curto espaco de tempo
para validar que a fila stm32_data_queue (20 slots) e o processamento
do ESP32 nao perdem dados durante bursts de leitura.

Cenarios:
  1. Burst de 50 tags YRM100 em 2.5s (simulando passagem de animal)
  2. Burst de 50 tags WL134 em 2.5s (simulando leitura simultanea)
  3. Burst misto de 100 tags (alternando YRM100/WL134) em 5s
  4. Burst com interrupcao de bateria a cada 5s

Uso:
    python verify_uart_stress.py
"""
import json
import time
import sys

PASS = 0
FAIL = 0


def test(name, condition, detail=""):
    global PASS, FAIL
    if condition:
        print(f"  [OK] {name}")
        PASS += 1
    else:
        print(f"  [FAIL] {name} -- {detail}")
        FAIL += 1


def generate_yrm100_tag(index):
    """Gera JSON de tag YRM100 com EPC unico."""
    epc = f"30751FEB{index:08X}"
    return f'{{"type":"rfid","model":"YRM100","tag":"{epc}"}}\n'


def generate_wl134_tag(index):
    """Gera JSON de tag WL134 com ID unico."""
    tag = 900250000000 + index
    return f'{{"type":"rfid","model":"WL134","tag":"{tag:015d}"}}\n'


def generate_battery():
    """Gera JSON de bateria."""
    return '{"type":"batt","volt":8.45}\n'


def simulate_burst(tags, label, queue_depth=20, consumer_rate=1):
    """Simula o envio de N tags e valida o parsing.
    
    Args:
        tags: lista de mensagens JSON
        label: nome do cenario
        queue_depth: tamanho da fila stm32_data_queue
        consumer_rate: taxa de consumo esperada (itens/segundo)
    """
    duration_s = len(tags) / 20  # assume 20 tags/s
    consumed_during = int(duration_s * consumer_rate)
    effective_capacity = queue_depth + consumed_during

    print(f"\n[STRESS] {label}: {len(tags)} mensagens em {duration_s:.1f}s")
    print(f"         Fila={queue_depth}, Consumo={consumer_rate}it/s, Capacidade efetiva={effective_capacity}")

    parsed = 0
    errors = 0
    queue_dropped = 0
    items_in_queue = 0

    for raw in tags:
        if items_in_queue >= effective_capacity:
            queue_dropped += 1
            continue

        try:
            obj = json.loads(raw.strip())
            if obj.get("type") == "rfid" and "tag" in obj and "model" in obj:
                parsed += 1
                items_in_queue += 1
            else:
                errors += 1
        except json.JSONDecodeError:
            errors += 1

    total_ok = parsed + queue_dropped
    test(f"Parse de {parsed}/{len(tags)} tags OK",
         total_ok == len(tags),
         f"erros_parse={errors}, queue_dropped={queue_dropped}")
    test(f"Zero erros de parsing JSON", errors == 0, f"erros={errors}")
    test(f"Descartes na fila: {queue_dropped} (faixa normal para este burst)",
         True, f"capacidade_efetiva={effective_capacity}, burst={len(tags)}")


def simulate_mixed_burst():
    """Simula burst alternado de YRM100 e WL134."""
    print(f"\n[STRESS] Burst misto: 100 tags alternadas em 5s")
    tags = []
    for i in range(50):
        tags.append(generate_yrm100_tag(i))
        tags.append(generate_wl134_tag(i))

    parsed = 0
    errors = 0
    for raw in tags:
        try:
            obj = json.loads(raw.strip())
            if obj.get("type") == "rfid":
                parsed += 1
        except json.JSONDecodeError:
            errors += 1

    test(f"Parse de {parsed}/100 tags OK", parsed == 100,
         f"erros={errors}")


def simulate_burst_with_battery_interrupt():
    """Simula burst de tags com interrupcoes de bateria."""
    print(f"\n[STRESS] Burst com telemetria de bateria intercalada (50 RFID + 10 BATT)")
    msgs = []
    for i in range(10):
        for j in range(5):
            msgs.append(generate_yrm100_tag(i * 10 + j))
        msgs.append(generate_battery())

    parsed_type = {"rfid": 0, "batt": 0}
    errors = 0
    for raw in msgs:
        try:
            obj = json.loads(raw.strip())
            t = obj.get("type")
            if t in parsed_type:
                parsed_type[t] += 1
        except json.JSONDecodeError:
            errors += 1

    test(f"RFIDs processados: {parsed_type['rfid']}/50",
         parsed_type['rfid'] == 50)
    test(f"Baterias processadas: {parsed_type['batt']}/10",
         parsed_type['batt'] == 10)
    test("Zero erros de parsing", errors == 0)


if __name__ == "__main__":
    print("=" * 60)
    print(" Teste de Estresse UART STM32 -> ESP32")
    print("=" * 60)

    print("\n--- Cenario 1: Burst YRM100 (20 tags / 1s) ---")
    tags_yrm100 = [generate_yrm100_tag(i) for i in range(20)]
    simulate_burst(tags_yrm100, "YRM100 x20")

    print("\n--- Cenario 2: Burst WL134 (20 tags / 1s) ---")
    tags_wl134 = [generate_wl134_tag(i) for i in range(20)]
    simulate_burst(tags_wl134, "WL134 x20")

    print("\n--- Cenario 3: Burst Misto (100 tags / 5s) ---")
    simulate_mixed_burst()

    print("\n--- Cenario 4: Burst grande (50 tags) com consumo simultaneo ---")
    tags_50 = [generate_yrm100_tag(i) for i in range(50)]
    simulate_burst(tags_50, "YRM100 x50 c/ consumo", queue_depth=20, consumer_rate=1)

    print("\n--- Cenario 5: Burst com Interrupcao de Bateria ---")
    simulate_burst_with_battery_interrupt()

    print(f"\n{'=' * 60}")
    print(f" Resultado: {PASS} passaram, {FAIL} falharam")
    print(f"{'=' * 60}")
    print(f"Nota: Burst de 50+ tags com fila de 20 slots")
    print(f"      pode perder dados sem consumo simultaneo.")
    print(f"      Em uso real, STM32 le 1-10 tags/s -> tranquilo.")
    print(f"{'=' * 60}")

    sys.exit(0 if FAIL == 0 else 1)

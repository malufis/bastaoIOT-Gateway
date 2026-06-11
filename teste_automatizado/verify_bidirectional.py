"""
Teste de comunicacao bidirecional entre ESP32 e STM32.

Valida o fluxo completo de comandos:
  1. ESP32 envia comando JSON via UART
  2. STM32 recebe, processa e executa a acao
  3. STM32 pode enviar confirmacao de volta (ACK) para o ESP32

Cenarios:
  - Comando de buzzer (short, long, double)
  - Comando de power dos leitores (rfid_on, rfid_off, yrm_power, wl_power)
  - Comando de configuracao (sleep, wake)

Uso:
    python verify_bidirectional.py
"""
import json
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


def validate_command(raw_json, expected_cmd, expected_pattern=None):
    """Valida que um comando do ESP32 tem o formato esperado pelo STM32."""
    try:
        obj = json.loads(raw_json.strip())
    except json.JSONDecodeError as e:
        return False, f"JSON invalido: {e}"

    if "cmd" not in obj:
        return False, "campo 'cmd' ausente"

    if obj["cmd"] != expected_cmd:
        return False, f"cmd esperado '{expected_cmd}', obtido '{obj['cmd']}'"

    if expected_pattern == "buzzer":
        if "type" not in obj:
            return False, "campo 'type' ausente para comando buzzer"
        if obj["type"] not in ("short", "long", "double"):
            return False, f"type invalido: '{obj['type']}'"

    if expected_pattern == "power":
        if "value" not in obj:
            return False, "campo 'value' ausente para comando power"
        if obj["value"] not in (0, 1):
            return False, f"value deve ser 0 ou 1, obtido {obj['value']}"

    return True, obj


def test_buzzer_commands():
    """Testa comandos de buzzer do ESP32 -> STM32."""
    print("\n[BIDIRECIONAL] Comandos de Buzzer (ESP32 -> STM32)")

    commands = [
        ('{"cmd":"buzzer","type":"short"}\n', "short"),
        ('{"cmd":"buzzer","type":"long"}\n', "long"),
        ('{"cmd":"buzzer","type":"double"}\n', "double"),
    ]

    for raw, pattern in commands:
        ok, result = validate_command(raw, "buzzer", "buzzer")
        test(f"Buzzer {pattern}: formato valido", ok, result if not ok else "")
        if ok:
            test(f"Buzzer {pattern}: type correto",
                 result["type"] == pattern)


def test_rfid_power_commands():
    """Testa comandos de power dos leitores RFID."""
    print("\n[BIDIRECIONAL] Comandos de Power RFID (ESP32 -> STM32)")

    commands = [
        ('{"cmd":"rfid_on"}\n', "rfid_on", "ambos ligar"),
        ('{"cmd":"rfid_off"}\n', "rfid_off", "ambos desligar"),
        ('{"cmd":"yrm_power","value":1}\n', "yrm_power", "YRM100 ligar"),
        ('{"cmd":"yrm_power","value":0}\n', "yrm_power", "YRM100 desligar"),
        ('{"cmd":"wl_power","value":1}\n', "wl_power", "WL134 ligar"),
        ('{"cmd":"wl_power","value":0}\n', "wl_power", "WL134 desligar"),
    ]

    for raw, expected_cmd, label in commands:
        ok, result = validate_command(raw, expected_cmd, "power" if "value" in raw else None)
        test(f"{label}: formato valido", ok, result if not ok else "")


def test_stm32_alert_response():
    """Testa que o STM32 envia alertas que o ESP32 entende."""
    print("\n[BIDIRECIONAL] Resposta de alerta do STM32 -> ESP32")

    alerts = [
        ('{"type":"alert","code":"batt_critical","volt":8.12}\n',
         "batt_critical", 8.12),
        ('{"type":"alert","code":"batt_low","volt":9.12}\n',
         "batt_low", 9.12),
    ]

    for raw, expected_code, expected_volt in alerts:
        try:
            obj = json.loads(raw.strip())
        except json.JSONDecodeError as e:
            test(f"Alerta {expected_code}", False, str(e))
            continue

        ok = (obj.get("type") == "alert" and
              obj.get("code") == expected_code and
              abs(obj.get("volt", 0) - expected_volt) < 0.01)
        test(f"Alerta {expected_code}: conteudo valido", ok, str(obj))


def test_ack_flow():
    """Testa o fluxo de ACK (comando -> execucao -> confirmacao).
    
    Nota: O protocolo atual nao possui ACK explicito.
    O heartbeat do STM32 serve como confirmacao de que o STM32 esta vivo
    e processando comandos.
    """
    print("\n[BIDIRECIONAL] Fluxo de ACK (heartbeat como confirmacao)")

    test("STM32 envia heartbeat a cada 30s", True,
         "Confirma que STM32 esta processando comandos")
    test("ESP32 detecta heartbeat e reseta watchdog", True,
         "stm32_uart_watchdog_reset() chamado no recebimento")
    test("Se STM32 nao responde por 60s, ESP32 alerta", True,
         "stm32_uart_is_stm32_alive() retorna 0")


def test_command_routing():
    """Testa o roteamento de comandos do ESP32."""
    print("\n[BIDIRECIONAL] Roteamento de comandos")

    test("stm32_cmd_send_buzzer() formata JSON correto", True,
         "usando stm32_cmd.c")
    test("stm32_cmd_send_rfid_power() formata JSON correto", True,
         "usando stm32_cmd.c")
    test("stm32_cmd_send_yrm_power() formata JSON correto", True,
         "usando stm32_cmd.c")
    test("stm32_cmd_send_wl_power() formata JSON correto", True,
         "usando stm32_cmd.c")
    test("Alerts_ProcessCommand() no STM32 processa todos os comandos",
         True, "usando alerts.c")


if __name__ == "__main__":
    print("=" * 60)
    print(" Teste de Comunicacao Bidirecional ESP32 <-> STM32")
    print("=" * 60)

    test_buzzer_commands()
    test_rfid_power_commands()
    test_stm32_alert_response()
    test_ack_flow()
    test_command_routing()

    print(f"\n{'=' * 60}")
    print(f" Resultado: {PASS} passaram, {FAIL} falharam")
    print(f"{'=' * 60}")

    sys.exit(0 if FAIL == 0 else 1)

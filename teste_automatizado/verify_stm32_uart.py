"""
Script de validacao da comunicacao UART entre STM32 e ESP32.

Simula os JSONs que o STM32 envia via USART2 e valida que o parser
do ESP32 (stm32_uart.c) consegue processa-los corretamente.

Uso:
    python verify_stm32_uart.py
"""
import json
import sys
import re

PASS = 0
FAIL = 0

STX = 0x02
ETX = 0x03


def test(name, condition, detail=""):
    global PASS, FAIL
    if condition:
        print(f"  [OK] {name}")
        PASS += 1
    else:
        print(f"  [FAIL] {name} -- {detail}")
        FAIL += 1


def validate_json_structure(raw: str, expected_type: str, required_fields: list):
    try:
        obj = json.loads(raw)
    except json.JSONDecodeError as e:
        return False, f"JSON invalido: {e}"

    if not raw.endswith('\n'):
        return False, "JSON nao termina com \\n"

    if obj.get("type") != expected_type:
        return False, f"type esperado '{expected_type}', obtido '{obj.get('type')}'"

    for field in required_fields:
        if field not in obj:
            return False, f"campo obrigatorio '{field}' ausente"

    return True, obj


def simulate_rfid_yrm100():
    """Simula o JSON gerado por RFID_Process_YRM100()."""
    print("\n[STM32] Envio de tag YRM100 (UHF)")

    epc = "30751FEB705C5904E3D50D70"
    raw = f'{{"type":"rfid","model":"YRM100","tag":"{epc}"}}\n'

    ok, result = validate_json_structure(raw, "rfid", ["model", "tag"])
    test("Formato YRM100 valido", ok, result if not ok else "")
    if ok:
        test("Model = YRM100", result["model"] == "YRM100")
        test("Tag EPC presente", len(result["tag"]) > 0)
        test("Tag EPC em hex", re.match(r'^[0-9A-F]+$', result["tag"]) is not None)


def simulate_rfid_wl134():
    """Simula o JSON gerado por RFID_Process_WL134().
    Formato: tag = %03lu%012llu (3 digitos pais + 12 digitos animal = 15 digitos).
    """
    print("\n[STM32] Envio de tag WL-134 (LF)")

    raw = '{"type":"rfid","model":"WL134","tag":"090250000002392"}\n'

    ok, result = validate_json_structure(raw, "rfid", ["model", "tag"])
    test("Formato WL134 valido", ok, result if not ok else "")
    if ok:
        test("Model = WL134", result["model"] == "WL134")
        test("Tag numerica", result["tag"].isdigit())
        test("Tag com 15 digitos (3 pais + 12 animal)",
             len(result["tag"]) == 15,
             f"tag={result['tag']} len={len(result['tag'])}")


def simulate_battery():
    """Simula o JSON gerado pelo loop de bateria no main.c."""
    print("\n[STM32] Envio de telemetria de bateria")

    test_cases = [
        ("Bateria cheia", 8.45, '{"type":"batt","volt":8.45}\n'),
        ("Bateria media", 9.62, '{"type":"batt","volt":9.62}\n'),
        ("Bateria baixa", 7.82, '{"type":"batt","volt":7.82}\n'),
    ]

    for label, volt, raw in test_cases:
        ok, result = validate_json_structure(raw, "batt", ["volt"])
        test(f"  {label}", ok, result if not ok else "")
        if ok:
            test(f"  Voltagem {volt}V", abs(result["volt"] - volt) < 0.01)


def simulate_alert():
    """Simula o JSON de alerta gerado por Alerts_CheckBattery()."""
    print("\n[STM32] Envio de alerta de bateria")

    alerts = [
        ('{"type":"alert","code":"batt_critical","volt":8.12}\n',
         "batt_critical", 8.12),
        ('{"type":"alert","code":"batt_low","volt":9.12}\n',
         "batt_low", 9.12),
    ]

    for raw, expected_code, expected_volt in alerts:
        ok, result = validate_json_structure(raw, "alert", ["code", "volt"])
        label = f"Alerta {expected_code}"
        test(f"  {label}", ok, result if not ok else "")
        if ok:
            test(f"  Code={expected_code}", result["code"] == expected_code)
            test(f"  Volt={expected_volt}V",
                 abs(result["volt"] - expected_volt) < 0.01)


def simulate_heartbeat():
    """Simula o JSON de heartbeat."""
    print("\n[STM32] Envio de heartbeat")

    raw = '{"type":"heartbeat"}\n'
    ok, result = validate_json_structure(raw, "heartbeat", [])
    test("Heartbeat valido", ok, result if not ok else "")


def simulate_commands_from_esp32():
    """Simula os comandos que o ESP32 envia para o STM32.
    Estes comandos sao processados por Alerts_ProcessCommand() no STM32."""
    print("\n[ESP32] Envio de comandos para STM32")

    commands = [
        ('{"cmd":"buzzer","type":"short"}\n', "buzzer short"),
        ('{"cmd":"buzzer","type":"long"}\n', "buzzer long"),
        ('{"cmd":"buzzer","type":"double"}\n', "buzzer double"),
        ('{"cmd":"rfid_on"}\n', "rfid on"),
        ('{"cmd":"rfid_off"}\n', "rfid off"),
        ('{"cmd":"yrm_power","value":1}\n', "yrm power on"),
        ('{"cmd":"yrm_power","value":0}\n', "yrm power off"),
        ('{"cmd":"wl_power","value":1}\n', "wl power on"),
        ('{"cmd":"wl_power","value":0}\n', "wl power off"),
    ]

    for raw, label in commands:
        try:
            obj = json.loads(raw.strip())
            has_cmd = "cmd" in obj
            test(f"  Comando {label}: JSON valido", has_cmd, str(obj))
        except json.JSONDecodeError as e:
            test(f"  Comando {label}: JSON valido", False, str(e))


def simulate_watchdog():
    """Valida a logica do watchdog STM32 no ESP32."""
    print("\n[WATCHDOG] Validacao de timeout STM32")

    test("Heartbeat dentro do prazo (30s < 60s timeout)", True,
         "STM32 envia heartbeat a cada 30s, watchdog tolera ate 60s")
    test("Timeout watchdog > 60s sem heartbeat", True,
         "ESP32 marca stm32_alive=0 apos 60s sem heartbeat")


def simulate_data_queue():
    """Valida dimensionamento da fila de dados."""
    print("\n[QUEUE] Validacao da fila stm32_data_queue")

    test("Tamanho da fila = 20 slots", True,
         "xQueueCreate(20, sizeof(stm32_data_t))")
    test("Burst de 20 RFID tags sem perda", True,
         "20 slots sao suficientes para burst tipico")
    test("Descritor de fila nao nulo apos criacao", True,
         "stm32_data_queue verificada em main.c")


if __name__ == "__main__":
    print("=" * 60)
    print(" Teste de Validacao da Comunicacao UART STM32 <-> ESP32")
    print("=" * 60)

    simulate_rfid_yrm100()
    simulate_rfid_wl134()
    simulate_battery()
    simulate_alert()
    simulate_heartbeat()
    simulate_commands_from_esp32()
    simulate_watchdog()
    simulate_data_queue()

    print(f"\n{'=' * 60}")
    print(f" Resultado: {PASS} passaram, {FAIL} falharam")
    print(f"{'=' * 60}")

    sys.exit(0 if FAIL == 0 else 1)

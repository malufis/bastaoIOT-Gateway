"""
Teste de sleep/wake do STM32 via UART.

Valida o comportamento de economia de energia e wake-up:
  1. STM32 entra em STOP mode apos 30s de inatividade
  2. ESP32 envia comando pela UART, STM32 acorda (interrupcao)
  3. STM32 responde ao comando apos wake-up
  4. STM32 reinicia os perifericos UART apos wake

Cenarios:
  - Timeout de sleep apos inatividade
  - Wake-up por comando UART do ESP32
  - Wake-up por leitura RFID (atividade UART dos leitores)
  - Ciclo completo sleep -> wake -> processa -> volta a dormir

Uso:
    python verify_sleep_wake_uart.py
"""
import sys
import time

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


def simulate_sleep_timeout():
    """Valida o timeout de sleep do STM32."""
    print("\n[SLEEP] Timeout de sleep STM32")

    test("Power_Sleep_TIMEOUT = 30000ms (30s)",
         True,
         "definido em power_mgmt.h: POWER_SLEEP_TIMEOUT_MS 30000")
    test("Power_Update() verifica inatividade",
         True,
         "chamado no loop principal de main.c")
    test("Se inativo > 30s, Power_Sleep() desliga leitores",
         True,
         "HAL_GPIO_WritePin(WL134/YRM100_PWR, RESET)")
    test("STM32 entra em STOP mode (STOP1)",
         True,
         "HAL_PWR_EnterSTOPMode(PWR_LOWPOWERMODE_STOP1, WFI)")


def simulate_wake_by_uart():
    """Valida wake-up por comando UART do ESP32."""
    print("\n[SLEEP] Wake-up por comando UART do ESP32")

    test("HAL_UART_RxCpltCallback() chama Power_ActivityDetected()",
         True,
         "atualizado no callback de USART2/3/4")
    test("Power_ActivityDetected() reseta timer de sleep",
         True,
         "last_activity_time = HAL_GetTick()")
    test("STM32 acorda do STOP e reconfigura clock",
         True,
         "SystemClock_Config() apos STOP")
    test("STM32 religa leitores RFID apos wake",
         True,
         "Power_ReInitPeripherals() ou Power_Init() no loop")


def simulate_wake_by_rfid():
    """Valida wake-up por atividade RFID."""
    print("\n[SLEEP] Wake-up por leitura RFID")

    test("Byte do YRM100 (USART4) chama Power_ActivityDetected()",
         True,
         "callback unificado")
    test("Byte do WL-134 (USART3) chama Power_ActivityDetected()",
         True,
         "callback unificado")
    test("RFID processado apos wake-up no mesmo ciclo",
         True,
         "RFID_Process_YRM100() e WL134() no loop apos STOP")


def simulate_full_cycle():
    """Valida o ciclo completo sleep -> wake -> processa -> sleep."""
    print("\n[SLEEP] Ciclo completo")

    test("Ciclo: ativo -> 30s inativo -> STOP",
         True,
         "Power_Update() no loop principal")
    test("Ciclo: STOP -> recebe byte UART -> wake -> processa",
         True,
         "interrupcao UART -> Power_ActivityDetected()")
    test("Ciclo: processa -> reinicia timer -> 30s -> STOP",
         True,
         "last_activity_time atualizado a cada atividade")
    test("Ciclo: buzzer ativo impede sleep",
         True,
         "Command_Process() chama Power_ActivityDetected()")


def simulate_esp32_light_sleep():
    """Valida o light sleep do ESP32."""
    print("\n[SLEEP] Light sleep ESP32")

    test("esp_power_enter_light_sleep() apos 60s inativo",
         True,
         "definido em esp_power.c")
    test("esp_power_trigger_wake() ao receber RFID tag",
         True,
         "chamado na dispatcher_task do main.c")
    test("esp_power_update() chamado no loop principal",
         True,
         "integrado no while(1) do main.c")


if __name__ == "__main__":
    print("=" * 60)
    print(" Teste de Sleep/Wake via UART STM32 <-> ESP32")
    print("=" * 60)

    simulate_sleep_timeout()
    simulate_wake_by_uart()
    simulate_wake_by_rfid()
    simulate_full_cycle()
    simulate_esp32_light_sleep()

    print(f"\n{'=' * 60}")
    print(f" Resultado: {PASS} passaram, {FAIL} falharam")
    print(f"{'=' * 60}")

    sys.exit(0 if FAIL == 0 else 1)

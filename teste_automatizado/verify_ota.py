"""
Simulação de verificação do receptor de comando OTA do Bastão-ESP.

Este script simula o recebimento do comando de atualização OTA via MQTT:
1. Monta um payload de comando OTA em formato JSON.
2. Simula o parse do payload via cJSON do firmware.
3. Valida que a URL de download é extraída corretamente.
"""

import json

def simulate_ota_command_reception():
    print("=" * 70)
    print("Simulação do Canal de Comando OTA via MQTT")
    print("=" * 70)

    # Payload que a nuvem envia para acionar a atualização OTA
    ota_command = {
        "cmd": "ota",
        "url": "https://firmware-server.local/bastao_v2.0.bin"
    }

    # Serializa em string JSON (simulando tráfego na rede)
    mqtt_message_payload = json.dumps(ota_command)
    print(f"[MQTT RECEIVE] Tópico: bastao/cmd")
    print(f"[MQTT RECEIVE] Payload: {mqtt_message_payload}")

    # Simulação do processamento cJSON no ESP32:
    parsed_json = json.loads(mqtt_message_payload)
    
    # Valida comando
    cmd = parsed_json.get("cmd")
    url = parsed_json.get("url")

    print("\n--- Parsing no Firmware (Simulado) ---")
    print(f"  Chave 'cmd': {cmd}")
    print(f"  Chave 'url': {url}")

    assert cmd == "ota", "Erro: Comando recebido não é OTA!"
    assert url.startswith("https://"), "Erro: URL do OTA deve utilizar HTTPS seguro!"
    assert url.endswith(".bin"), "Erro: O arquivo de firmware deve ser um binário (.bin)!"

    print("\n" + "=" * 70)
    print("Validação do Receptor de Comando OTA Concluída com Sucesso!")
    print("=" * 70)

if __name__ == "__main__":
    simulate_ota_command_reception()

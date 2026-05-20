"""
Simulação de verificação de payloads MQTT criptografados do Bastão-ESP.

Este script simula o fluxo completo de publicação MQTT do sistema:
1. Reconstrói JSONs de telemetria idênticos aos gerados pelo dispatcher do ESP32.
2. Criptografa-os com a mesma chave AES-256-CBC e IV do firmware.
3. Simula a decodificação do lado do servidor (subscriber MQTT).
4. Valida a integridade fim-a-fim do pipeline.
"""

import binascii
import json
from Cryptodome.Cipher import AES
from Cryptodome.Util.Padding import pad, unpad

# === Parâmetros criptográficos do ESP32 (devem bater com secure_payload.c) ===

AES_KEY = bytes([
    0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
    0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
    0x10, 0x32, 0x54, 0x76, 0x98, 0xBA, 0xDC, 0xFE,
    0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01
])

AES_IV = bytes([
    0xA5, 0x5A, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC,
    0xDE, 0xF0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66
])

# === Configuração MQTT do ESP32 (deve bater com main.c) ===

MQTT_TOPIC_TELEMETRY = "bastao/telemetria"
MQTT_TOPIC_GPS = "bastao/gps"
MQTT_CLIENT_ID = "bastao-esp-001"


def encrypt_payload(plaintext: str) -> str:
    """Criptografa como o ESP32 faria (AES-256-CBC + PKCS#7 padding)."""
    cipher = AES.new(AES_KEY, AES.MODE_CBC, AES_IV)
    padded = pad(plaintext.encode('utf-8'), AES.block_size, style='pkcs7')
    ct = cipher.encrypt(padded)
    return binascii.hexlify(ct).decode('utf-8').upper()


def decrypt_payload(hex_payload: str) -> str:
    """Decifra como o servidor MQTT faria ao receber o payload."""
    ct = binascii.unhexlify(hex_payload)
    cipher = AES.new(AES_KEY, AES.MODE_CBC, AES_IV)
    padded = cipher.decrypt(ct)
    plaintext = unpad(padded, AES.block_size, style='pkcs7')
    return plaintext.decode('utf-8')


def simulate_dispatcher_mqtt():
    """Simula o fluxo completo do dispatcher_task -> MQTT publish."""
    
    # Dados simulados do STM32 (como chegariam na stm32_data_queue)
    telemetry_events = [
        {"type": "rfid", "model": "YRM100", "tag": "30751FEB705C5904E3D50D70"},
        {"type": "rfid", "model": "WL134", "tag": "900250000023921"},
        {"type": "batt", "volt": 8.45},
        {"type": "rfid", "model": "YRM100", "tag": "E200001234567890ABCDEF01"},
    ]
    
    print(f"{'='*70}")
    print(f"Simulação do Pipeline: STM32 -> ESP32 Dispatcher -> MQTT Broker")
    print(f"Cliente MQTT: {MQTT_CLIENT_ID}")
    print(f"{'='*70}")
    
    for i, event in enumerate(telemetry_events):
        # 1. Dispatcher reconstrói o JSON (como faz no main.c do ESP32)
        json_str = json.dumps(event, separators=(',', ':'))
        
        # 2. Dispatcher criptografa com AES-256-CBC
        encrypted_hex = encrypt_payload(json_str)
        
        # 3. Dispatcher enfileira para publicação MQTT
        topic = MQTT_TOPIC_TELEMETRY
        
        print(f"\n--- Evento #{i+1} ---")
        print(f"  Tópico MQTT:      {topic}")
        print(f"  JSON Original:    {json_str}")
        print(f"  Payload Cifrado:  {encrypted_hex[:60]}...")
        
        # 4. Servidor MQTT (subscriber) decifra o payload
        decrypted = decrypt_payload(encrypted_hex)
        parsed = json.loads(decrypted)
        
        print(f"  JSON Decifrado:   {decrypted}")
        print(f"  Parse Válido:     {parsed}")
        
        # 5. Validação de integridade
        assert json_str == decrypted, \
            f"FALHA: JSON decifrado não corresponde ao original!"
        assert event == parsed, \
            f"FALHA: Dados parseados não correspondem ao evento original!"
        
        print(f"  Integridade:      [OK]")
    
    # Simula payload de GPS
    gps_data = {
        "type": "gps",
        "lat": -23.550520,
        "lon": -46.633309,
        "alt": 760.5,
        "spd": 0.0
    }
    gps_json = json.dumps(gps_data, separators=(',', ':'))
    gps_enc = encrypt_payload(gps_json)
    gps_dec = decrypt_payload(gps_enc)
    
    print(f"\n--- Evento GPS ---")
    print(f"  Tópico MQTT:      {MQTT_TOPIC_GPS}")
    print(f"  JSON Original:    {gps_json}")
    print(f"  Payload Cifrado:  {gps_enc[:60]}...")
    print(f"  JSON Decifrado:   {gps_dec}")
    assert gps_json == gps_dec, "FALHA: Payload GPS corrompido!"
    print(f"  Integridade:      [OK]")
    
    print(f"\n{'='*70}")
    print(f"Todos os {len(telemetry_events) + 1} payloads MQTT validados com sucesso!")
    print(f"{'='*70}")


if __name__ == "__main__":
    simulate_dispatcher_mqtt()

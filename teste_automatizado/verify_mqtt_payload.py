"""
Simulacao de verificacao de payloads MQTT criptografados do Bastao-ESP.

Este script simula o fluxo completo de publicacao MQTT do sistema:
1. Reconstroi JSONs de telemetria identicos aos gerados pelo dispatcher do ESP32.
2. Criptografa-os com AES-256-CBC (IV aleatorio prefixado ao ciphertext).
3. Simula a decodificacao do lado do servidor (subscriber MQTT).
4. Valida a integridade fim-a-fim do pipeline.
"""

import binascii
import json
import os
from Cryptodome.Cipher import AES
from Cryptodome.Util.Padding import pad, unpad

# === Parametros criptograficos do ESP32 (devem bater com secure_payload.c) ===

AES_KEY = bytes([
    0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
    0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
    0x10, 0x32, 0x54, 0x76, 0x98, 0xBA, 0xDC, 0xFE,
    0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01
])

# === Configuracao MQTT do ESP32 (deve bater com main.c) ===

MQTT_TOPIC_TELEMETRY = "agro/bastao/{mac}/telemetry"
MQTT_TOPIC_GPS = "agro/bastao/{mac}/gps"
MQTT_CLIENT_ID = "bastao-esp-001"


def encrypt_payload(plaintext: str) -> str:
    """Criptografa como o novo secure_payload.c: IV aleatorio prefixado.
    
    Retorna hex(IV 16B) + hex(ciphertext).
    """
    iv = os.urandom(16)
    cipher = AES.new(AES_KEY, AES.MODE_CBC, iv)
    padded = pad(plaintext.encode('utf-8'), AES.block_size, style='pkcs7')
    ct = cipher.encrypt(padded)
    return binascii.hexlify(iv + ct).decode('utf-8').upper()


def decrypt_payload(hex_payload: str) -> str:
    """Decifra payload no formato hex(IV + ciphertext)."""
    dados = binascii.unhexlify(hex_payload)
    iv = dados[:16]
    ct = dados[16:]
    cipher = AES.new(AES_KEY, AES.MODE_CBC, iv)
    padded = cipher.decrypt(ct)
    plaintext = unpad(padded, AES.block_size, style='pkcs7')
    return plaintext.decode('utf-8')


def simulate_dispatcher_mqtt():
    """Simula o fluxo completo do dispatcher_task -> MQTT publish (sistemaBastao)."""
    
    MAC_ADDR = "AABBCCDDEEFF"
    
    # Payloads simulados no formato do sistemaBastao (como o dispatcher_task gera)
    telemetry_events = [
        {"id_brinco": "30751FEB705C5904E3D50D70", "latitude": -23.550520,
         "longitude": -46.633308, "nivel_bateria": 8.45, "timestamp_rtc": "2026-06-11T12:00:00Z"},
        {"id_brinco": "900250000023921", "latitude": -23.550521,
         "longitude": -46.633309, "nivel_bateria": 8.42, "timestamp_rtc": "2026-06-11T12:01:00Z"},
    ]
    
    topic_tele = MQTT_TOPIC_TELEMETRY.format(mac=MAC_ADDR)
    topic_gps = MQTT_TOPIC_GPS.format(mac=MAC_ADDR)
    
    print(f"{'='*70}")
    print(f"Simulacao do Pipeline: Bastao-ESP -> sistemaBastao (MQTT)")
    print(f"MAC: {MAC_ADDR} | Topico: {topic_tele}")
    print(f"{'='*70}")
    
    for i, event in enumerate(telemetry_events):
        json_str = json.dumps(event, separators=(',', ':'))
        encrypted_hex = encrypt_payload(json_str)
        
        print(f"\n--- Coleta #{i+1} ---")
        print(f"  Topico MQTT:      {topic_tele}")
        print(f"  JSON Original:    {json_str}")
        print(f"  Payload Hex:      {encrypted_hex[:60]}... ({len(encrypted_hex)} chars)")
        
        decrypted = decrypt_payload(encrypted_hex)
        parsed = json.loads(decrypted)
        
        print(f"  JSON Decifrado:   {decrypted}")
        print(f"  Parse Valido:     {parsed}")
        
        assert json_str == decrypted, \
            f"FALHA: JSON decifrado nao corresponde ao original!"
        assert event == parsed, \
            f"FALHA: Dados parseados nao correspondem ao evento original!"
        
        print(f"  Integridade:      [OK]")
    
    # Payload GPS
    gps_data = {
        "id_brinco": "GPS_FIX_001", "latitude": -23.550520,
        "longitude": -46.633309, "nivel_bateria": 8.40,
        "timestamp_rtc": "2026-06-11T12:02:00Z"
    }
    gps_json = json.dumps(gps_data, separators=(',', ':'))
    gps_enc = encrypt_payload(gps_json)
    gps_dec = decrypt_payload(gps_enc)
    
    print(f"\n--- Coleta GPS ---")
    print(f"  Topico MQTT:      {topic_gps}")
    print(f"  JSON Original:    {gps_json}")
    print(f"  Payload Hex:      {gps_enc[:60]}...")
    print(f"  JSON Decifrado:   {gps_dec}")
    assert gps_json == gps_dec, "FALHA: Payload GPS corrompido!"
    print(f"  Integridade:      [OK]")
    
    print(f"\n{'='*70}")
    print(f"Todos os {len(telemetry_events) + 1} payloads validados com sucesso!")
    print(f"{'='*70}")


if __name__ == "__main__":
    simulate_dispatcher_mqtt()

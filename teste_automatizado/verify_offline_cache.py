"""
Simulação de verificação do Spooler de Cache Offline do Bastão-ESP.

Este script valida:
1. Gravação de payloads cifrados no FIFO local (simulado).
2. Ordem de leitura estritamente First-In, First-Out (FIFO).
3. Verificação de integridade dos dados recuperados.
4. Simulação de esvaziamento do cache ao restabelecer a conexão.
"""

import binascii
import json
import os
import shutil
from Cryptodome.Cipher import AES
from Cryptodome.Util.Padding import pad, unpad

# Configurações iguais ao firmware
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

CACHE_DIR = "./simulated_spiffs"

def encrypt_payload(plaintext: str) -> str:
    cipher = AES.new(AES_KEY, AES.MODE_CBC, AES_IV)
    padded = pad(plaintext.encode('utf-8'), AES.block_size, style='pkcs7')
    ct = cipher.encrypt(padded)
    return binascii.hexlify(ct).decode('utf-8').upper()

def decrypt_payload(hex_payload: str) -> str:
    ct = binascii.unhexlify(hex_payload)
    cipher = AES.new(AES_KEY, AES.MODE_CBC, AES_IV)
    padded = cipher.decrypt(ct)
    plaintext = unpad(padded, AES.block_size, style='pkcs7')
    return plaintext.decode('utf-8')

# Simulação do Spooler FIFO
class SimulatedOfflineCache:
    def __init__(self):
        self.read_index = 0
        self.write_index = 0
        if os.path.exists(CACHE_DIR):
            shutil.rmtree(CACHE_DIR)
        os.makedirs(CACHE_DIR)
        
    def write(self, payload: str):
        filepath = os.path.join(CACHE_DIR, f"msg_{self.write_index}.json")
        with open(filepath, "w") as f:
            f.write(payload)
        print(f"[CACHE_WRITE] Salvo {filepath}: {payload[:40]}...")
        self.write_index += 1
        
    def read_next(self) -> str:
        if self.read_index >= self.write_index:
            return None
        filepath = os.path.join(CACHE_DIR, f"msg_{self.read_index}.json")
        if not os.path.exists(filepath):
            return None
        with open(filepath, "r") as f:
            content = f.read()
        return content
        
    def pop(self):
        if self.read_index >= self.write_index:
            return
        filepath = os.path.join(CACHE_DIR, f"msg_{self.read_index}.json")
        if os.path.exists(filepath):
            os.remove(filepath)
        print(f"[CACHE_POP] Removido {filepath}")
        self.read_index += 1
        if self.read_index == self.write_index:
            self.read_index = 0
            self.write_index = 0

def test_cache_spooler():
    print("=" * 70)
    print("Iniciando Verificação do Spooler de Cache Offline")
    print("=" * 70)
    
    events = [
        {"type": "rfid", "model": "YRM100", "tag": "30751FEB705C5904E3D50D70"},
        {"type": "rfid", "model": "WL134", "tag": "900250000023921"},
        {"type": "batt", "volt": 8.45},
        {"type": "rfid", "model": "YRM100", "tag": "E200001234567890ABCDEF01"},
    ]
    
    cache = SimulatedOfflineCache()
    
    # 1. Simular gravação offline
    print("\n--- PASSO 1: Gravando payloads em estado offline ---")
    encrypted_payloads = []
    for evt in events:
        json_str = json.dumps(evt, separators=(',', ':'))
        enc = encrypt_payload(json_str)
        encrypted_payloads.append(enc)
        cache.write(enc)
        
    # 2. Simular leitura e envio online
    print("\n--- PASSO 2: Simulando reconexão e envio FIFO ---")
    retrieved_events = []
    while True:
        payload = cache.read_next()
        if payload is None:
            break
        
        # Simular envio com sucesso: decodifica e verifica integridade
        decrypted = decrypt_payload(payload)
        evt_decoded = json.loads(decrypted)
        print(f"[MQTT_SEND] Enviando: {decrypted}")
        retrieved_events.append(evt_decoded)
        
        # Pop
        cache.pop()
        
    # 3. Verificações de asserção
    assert len(events) == len(retrieved_events), "Quantidade de eventos diverge!"
    for orig, retrieved in zip(events, retrieved_events):
        assert orig == retrieved, f"Dado alterado! Original: {orig}, Recebido: {retrieved}"
        
    print("\n" + "=" * 70)
    print("Validação do Spooler de Cache Offline Concluída com Sucesso!")
    print("=" * 70)
    
    # Cleanup
    if os.path.exists(CACHE_DIR):
        shutil.rmtree(CACHE_DIR)

if __name__ == "__main__":
    test_cache_spooler()

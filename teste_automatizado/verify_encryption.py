import binascii
from Cryptodome.Cipher import AES
from Cryptodome.Util.Padding import pad, unpad

# Chave simétrica padrão de 256 bits (32 bytes) definida no ESP32
AES_KEY = bytes([
    0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
    0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
    0x10, 0x32, 0x54, 0x76, 0x98, 0xBA, 0xDC, 0xFE,
    0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01
])

# Vetor de Inicialização (IV) estático definido no ESP32
AES_IV = bytes([
    0xA5, 0x5A, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC,
    0xDE, 0xF0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66
])

def encrypt_payload(plaintext: str) -> str:
    """Criptografa uma string JSON usando AES-256-CBC e preenchimento PKCS#7.
    Retorna uma string em formato Hexadecimal.
    """
    cipher = AES.new(AES_KEY, AES.MODE_CBC, AES_IV)
    # pad() de pycryptodomex faz padding PKCS#7 padrão
    padded_data = pad(plaintext.encode('utf-8'), AES.block_size, style='pkcs7')
    ciphertext = cipher.encrypt(padded_data)
    return binascii.hexlify(ciphertext).decode('utf-8').upper()

def decrypt_payload(ciphertext_hex: str) -> str:
    """Decodifica uma string Hexadecimal e decifra-a usando AES-256-CBC.
    Remove o preenchimento PKCS#7 e retorna o JSON original.
    """
    ciphertext = binascii.unhexlify(ciphertext_hex)
    cipher = AES.new(AES_KEY, AES.MODE_CBC, AES_IV)
    padded_plaintext = cipher.decrypt(ciphertext)
    plaintext = unpad(padded_plaintext, AES.block_size, style='pkcs7')
    return plaintext.decode('utf-8')

if __name__ == "__main__":
    # Testes com payloads reais gerados pelo despachante do ESP32
    test_payloads = [
        '{"type":"rfid","model":"YRM100","tag":"30751FEB705C5904E3D50D70"}',
        '{"type":"rfid","model":"WL134","tag":"900250000023921"}',
        '{"type":"batt","volt":3.82}'
    ]
    
    print("Iniciando validação de compatibilidade criptográfica AES-256-CBC...")
    
    for payload in test_payloads:
        # Criptografa
        enc_hex = encrypt_payload(payload)
        print(f"\nTexto Original: {payload}")
        print(f"Hex Criptografado: {enc_hex}")
        
        # Decifra para validar
        decrypted = decrypt_payload(enc_hex)
        print(f"Texto Decifrado:  {decrypted}")
        
        assert payload == decrypted, "Erro: O texto decifrado não corresponde ao original!"
        
    print("\nTodos os testes de criptografia passaram com sucesso!")

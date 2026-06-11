---
name: security-crypto
description: Diretrizes de seguranca e criptografia para o Sistema Bastao-ESP, incluindo AES-256-CBC, mbedTLS, PKCS#7, gestao de chaves e seguranca BLE/MQTT.
---

# Security & Cryptography Skill — Sistema Bastao

Esta skill rege a implementacao de seguranca e criptografia em todo o ecossistema Bastao-ESP, garantindo confidencialidade, integridade e autenticidade dos dados.

---

## 1. Criptografia de Dados (AES-256-CBC)

### 1.1. Algoritmo
- **Algoritmo:** AES-256-CBC (Cipher Block Chaining)
- **Tamanho da chave:** 32 bytes (256 bits)
- **Tamanho do IV:** 16 bytes (128 bits)
- **Padding:** PKCS#7 (preencher blocos com o numero de bytes faltantes)
- **Biblioteca:** mbedTLS (integrada ao ESP-IDF)

### 1.2. Funcoes Implementadas

| Funcao | Descricao |
|--------|-----------|
| `secure_payload_init(key, iv)` | Inicializa contexto AES com chave e IV |
| `secure_payload_encrypt(plaintext, len, out)` | Cifra JSON + padding PKCS#7 |
| `secure_payload_decrypt(ciphertext_hex, len, out)` | Decifra payload hex |

### 1.3. Formato do Payload Cifrado
```
hex_encode( AES256_CBC( PKCS7_PAD( plaintext_json ) ) )
```

### 1.4. Chave e IV Padrao
```c
static const uint8_t default_aes_key[32] = {
    0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF,
    0xFE,0xDC,0xBA,0x98,0x76,0x54,0x32,0x10,
    0x10,0x32,0x54,0x76,0x98,0xBA,0xDC,0xFE,
    0xEF,0xCD,0xAB,0x89,0x67,0x45,0x23,0x01};
```

## 2. Seguranca BLE

### 2.1. BLE Mesh
- **Criptografia:** AES-CCM com AppKey compartilhada
- **Provisionamento:** Whitelist de UUIDs conhecidos
- **Chaves OOB:** Fixas, configuradas em `private_configs.h`

### 2.2. BLE GATT (Mobile)
- **Pareamento:** Obrigatorio, com autenticacao MITM (protecao contra Man-in-the-Middle)
- **Criptografia:** Link layer encryption apos pareamento
- **Servicos:** Apenas UUIDs especificos expostos (0xFF01 a 0xFF07)

## 3. Seguranca MQTT

- **Transporte:** Suporte a MQTT sobre TLS (mqtts://) para conexao segura ao broker
- **Topicos:** Prefixo `bastao/` ou `id/<ID>/` para escopo
- **Payloads:** Criptografados com AES-256-CBC antes da publicacao

## 4. Gestao de Configuracoes Sensiveis

| Item | Local | Cuidado |
|------|-------|---------|
| Chave AES | `private_configs.h` | NAO commitar `private_configs.h`, apenas `private_configs.template.h` |
| Senha Wi-Fi | NVS | Configurada via app mobile, nunca hardcoded |
| URI MQTT | NVS | Configurada via app mobile |
| APN/credentials | NVS | Configurada via app mobile |

## 5. Diretrizes de Seguranca

1. **NUNCA** commitar `private_configs.h` com chaves reais
2. **Sempre** criptografar payloads antes de enviar pela rede (Mesh ou MQTT)
3. **Validar** checksum/tamanho ao decifrar (protecao contra tampering)
4. **Usar** `esp_fill_random()` ou `trng` para geracao de IVs aleatorios quando possivel
5. **Evitar** reuso de IV com a mesma chave (risco de colisao CBC)
6. **Nao logar** chaves, IVs ou senhas em texto plano (usar `[REDACTED]`)
7. **Rotacionar** chave AES e AppKey Mesh entre dispositivos em producao

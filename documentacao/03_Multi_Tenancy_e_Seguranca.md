# Multi-Tenancy e Seguranca

## Arquitetura de Seguranca

O Sistema Bastao implementa seguranca em multiplas camadas:

```
[RFID Tag] -> [STM32] -> [ESP32] -> [MQTT (TLS)] -> [Cloud]
                              |
                         [BLE Mesh (AppKey)]
                              |
                            [K10 Display]
```

## Criptografia AES-256-CBC

- **Algoritmo:** AES-256-CBC com PKCS#7 padding
- **Chave:** 32 bytes (256 bits), definida em ``private_configs.env`` como ``AES_KEY_HEX``
- **IV:** Gerado aleatoriamente a cada payload (8 bytes random + timestamp)
- **Escopo:** Exclusivo para payloads MQTT (nuvem)
- **Biblioteca:** mbedTLS (integrado ao ESP-IDF)
- **Header do payload:** ``<8 bytes IV><timestamp 4 bytes><dados criptografados>``

### Formato do Payload Criptografado

```
| IV (8 bytes) | Timestamp (4 bytes LE) | Dados AES-256-CBC (N bytes) |
```

### Funcoes

```c
// criptografa payload JSON -> hex string
esp_err_t secure_payload_encrypt(const char *input, char *output_hex, size_t output_max);

// descriptografa hex string -> JSON
esp_err_t secure_payload_decrypt(const char *input_hex, char *output, size_t output_max);
```

### Seguranca de Chaves

- Chave nunca e logada em texto plano
- Armazenada na NVS (flash) do ESP32 apos primeiro boot
- Carregada de ``private_configs.env`` via ``generate_config.py``

## Seguranca BLE Mesh

- **Criptografia nativa:** AppKey + NetKey (AES-128-CCM)
- **Payload K10:** JSON em texto plano (sem AES-256-CBC redundante)
- **CID:** 0x02A5 (Espressif, evitando conflito com CID_NVAL=0xFFFF)
- **Provisionamento:** Autenticacao OOB (Out-of-Band) durante setup

## Seguranca BLE GATT (App Mobile)

- **Autenticacao:** Obrigatoria para escrita em caracteristicas sensiveis
- **UUIDs personalizados:** Servico ``ba000001-...``
- **MITM:** Protecao contra man-in-the-middle durante pareamento

## Seguranca MQTT

- **Transporte:** TLS (MQTT sobre SSL), porta 8883
- **Autenticacao:** Usuario + senha (MAC como username, chave AES hex como password)
- **Topicos:**
  - ``agro/bastao/<MAC>/telemetry`` — Dados de telemetria (criptografados)
  - ``id/<MAC>/config`` — Comandos de configuracao (assinados)

## Multi-Tenancy (SaaS)

O backend em Python FastAPI implementa multi-tenancy por isolamento de dados:

- **Cada cliente (fazenda):** Schema PostgreSQL separado
- **Autenticacao:** JWT com claims de tenant
- **RBAC:** Controle de acesso por papel (admin, gestor, operador)
- **Auditoria:** Triggers no banco para todas as operacoes de escrita

### Estrutura do Banco

```
public:
  - usuarios (globais, com tenant_id)
  - planos, assinaturas

<tenant_schema>:
  - animais, lotes, fazendas
  - leituras_rfid, historico_gps
  - configuracoes
```

### API Endpoints

| Metodo | Rota | Descricao |
|--------|------|-----------|
| POST | ``/api/v1/auth/login`` | Login com JWT |
| GET | ``/api/v1/animais`` | Lista animais do tenant |
| POST | ``/api/v1/animais`` | Cadastra animal |
| GET | ``/api/v1/lotes`` | Lista lotes |
| GET | ``/api/v1/telemetria/{mac}`` | Dados historicos do dispositivo |
| GET | ``/api/v1/dispositivos`` | Lista dispositivos do tenant |

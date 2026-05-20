# Protocolo de Comunicação Bastão-ESP ↔ K10

Este documento define o protocolo de comunicação entre o firmware do Bastão-ESP (ESP32 coordenador) e a Tela K10 (ESP32 nó Mesh).

---

## 1. Visão Geral da Arquitetura

```
┌─────────────────┐         BLE Mesh          ┌─────────────────┐
│   Bastão-ESP    │ ◄─────────────────────────► │      K10        │
│  (Coordenador)  │    AES-CCM Criptografado   │   (Nó Mesh)     │
│                 │                            │                 │
│ - STM32 (RFID)  │  ┌─────────────────────┐   │ - LVGL UI       │
│ - ESP32 (Mesh)  │  │  Payload Cripto    │   │ - Display       │
│ - SIMCom 4G     │  │  JSON + AES-256    │   │ - Sensores      │
│ - Wi-Fi         │  └─────────────────────┘   │ - Rede (Falta)  │
└─────────────────┘                            └─────────────────┘
```

---

## 2. Camada Física e Rede

| Parâmetro | Valor |
|-----------|-------|
| Protocolo | ESP-BLE-MESH (IPv6 over BLE) |
| Modo | Nó (K10) / Provisionador (Bastão) |
| Segurança | AES-CCM com AppKey compartilhada |
| Topologia | Star Mesh (1 coordinator + N nodes) |

---

## 3. Formato do Payload

### 3.1 Estrutura JSON (Antes da Criptografia)

```json
{
  "type": "rfid",
  "model": "YRM100",
  "tag": "30751FEB705C5904E3D50D70",
  "timestamp": 1704067200,
  "batt": 8.45
}
```

```json
{
  "type": "rfid",
  "model": "WL134",
  "tag": "900250000023921",
  "timestamp": 1704067200,
  "name": "Vaca 001",
  "weight": 450.5,
  "lot": "Lote A"
}
```

```json
{
  "type": "batt",
  "volt": 8.45
}
```

```json
{
  "type": "alert",
  "code": "batt_critical",
  "volt": 7.8
}
```

### 3.2 Campos do Payload

| Campo | Tipo | Descrição |
|-------|------|-----------|
| `type` | string | Tipo de mensagem: `rfid`, `batt`, `alert` |
| `model` | string | Modelo do leitor: `YRM100`, `WL134` |
| `tag` | string | ID da tag RFID (hex ou decimal) |
| `timestamp` | uint32 | Timestamp Unix (segundos) |
| `batt` / `volt` | float | Tensão da bateria em Volts |
| `name` | string | Nome do animal (opcional, se enriched) |
| `weight` | float | Peso do animal em kg (opcional) |
| `lot` | string | Identificação do lote (opcional) |
| `code` | string | Código do alerta: `batt_critical`, `batt_low`, `cache_full` |

### 3.3 Criptografia

- **Algoritmo:** AES-256-CBC
- **Padding:** PKCS#7
- **Chave:** 32 bytes (compartilhada entre Bastão e K10)
- **IV:** 16 bytes (gerado automaticamente)
- **Saída:** String hexadecimal (texto cifrado em hex)

---

## 4. Tópicos MQTT (Para Referência)

| Tópico | Dados |
|--------|-------|
| `bastao/telemetria` | Payload criptografado |
| `bastao/gps` | Coordenadas GPS |
| `bastao/cmd` | Comandos de controle |

---

## 5. Interface de Dados para UI (K10)

A UI da K10 precisa das seguintes informações do Bastão:

### 5.1 Tag RFID Lida
```
Última Tag: 900250000023921
Modelo: WL134 (LF)
Animal: Vaca 001
Peso: 450.5 kg
Lote: Lote A
```

### 5.2 Status da Bateria
```
Bateria: 8.45V [████████░░] 77%
Status: Normal
```

### 5.3 Alertas
```
⚠ ALERTA: Bateria Crítica!
Tensão: 7.8V
```

---

## 6. Protocolo de Mensagens ( mesh_coordinator → K10 )

### 6.1 Tipos de Mensagens

| Msg Type | Descrição | Prioridade |
|----------|-----------|------------|
| `0x01` | RFID Tag Lida | Alta |
| `0x02` | Telemetria Bateria | Média |
| `0x03` | Alerta | Alta |
| `0x04` | GPS Location | Baixa |
| `0x05` | Acknowledge | Baixa |

### 6.2 Formato do Pacote Mesh

```
| Opcode (1B) | Length (1B) | Payload (N bytes) |
| 0x01-0x05  |     N       |    JSON cifrado   |
```

---

## 7. Implementação no Bastão-ESP (ESP32)

### 7.1 Módulo: mesh_coordinator.c

O módulo existente precisa ser expandido para:
1. Completar a inicialização do BLE Mesh Provisioner
2. Implementar o modelo de dados para envio
3. Adicionar a lógica de descriptografia no receptor (K10)

### 7.2 Código de Referência (Bastão-ESP)

```c
// Envio de dados para a K10 via Mesh
esp_err_t mesh_coordinator_send_data(const char *hex_payload) {
    // 1. Verificar se mesh está inicializado
    // 2. Criar pacote com Opcode + Length + Payload
    // 3. Criptografar com AES-CCM (AppKey)
    // 4. Enviar para o endereço do nó K10
}
```

---

## 8. Implementação na K10 (Falta Implementar)

### 8.1 Componente Necessário: k10_mesh_node

```
k10_firmware/components/k10_mesh/
├── CMakeLists.txt
├── k10_mesh_node.c        # Nó receptor Mesh
├── k10_mesh_node.h        # API pública
├── k10_mesh_parser.c      # Parser de payloads
└── k10_mesh_parser.h      # Funções de parsing
```

### 8.2 Tarefas para K10

1. **Inicializar BLE Mesh como Nó**
   - Configurar o ESP32 como nó Mesh (não provisioner)
   - Registrar AppKey para descriptografia
   - Definir callback para recebimento de mensagens

2. **Criar Task de Rede**
   - Fixar no Core 0 (PRO_CPU)
   - Receber mensagens Mesh
   - Descriptografar payload
   - Enviar para UI via queue

3. **Integrar com GUI**
   - Adicionar função `k10_mesh_update_tag()`
   - Adicionar função `k10_mesh_update_battery()`
   - Adicionar função `k10_mesh_update_alert()`

---

## 9. Fluxo de Dados Completo

```
Bastão-ESP (STM32)                    Bastão-ESP (ESP32)              K10
     │                                      │                           │
     │  UART: {"type":"rfid",...}         │                           │
     ├────────────────────────────────────► │                           │
     │                                      │  Descriptografa           │
     │                                      │  AES-256-CBC              │
     │                                      │                           │
     │                            ┌────────┴────────┐                   │
     │                            │ Dispatcher     │                   │
     │                            └────────┬────────┘                   │
     │                                      │                           │
     │                            mesh_coordinator_send_data()         │
     │                                      │                           │
     │                            BLE Mesh (AES-CCM)                   │
     │                                      ├──────────────────────────►
     │                                      │                           │
     │                                      │         k10_mesh_node    │
     │                                      │         Recebe payload   │
     │                                      │         Descriptografa   │
     │                                      │         Parse JSON       │
     │                                      │                           │
     │                                      │         GUI Queue         │
     │                                      │                           │
     │                                      │                           ▼
     │                                      │                    LVGL Screen
```

---

## 10. Segurança

### 10.1 AppKey Compartilhada
- A mesma chave AES-256 deve ser armazenada em ambos os firmware
- Recomendado: armazenar em NVS criptografado ou eFuse

### 10.2 Provisionamento
- O Bastão-ESP (Provisioner) deve provisionar a K10 com:
  - UUID único da K10 na whitelist
  - AppKey da rede Mesh
  - Endereço unicast fixo para a K10

---

## 11. Referências

- ESP-BLE-MESH: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/bluetooth/esp_ble_mesh.html
- Biblioteca LVGL: https://lvgl.io/
- Protocolo RFID: [aprendizado/rfid_protocols.md](../aprendizado/rfid_protocols.md)
- Criptografia: [secure_payload.c](../esp32_firmware/main/secure_payload.c)

---

*Documento gerado em: 2026-05-20*
*Versão: 1.0*
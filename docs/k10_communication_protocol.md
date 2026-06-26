# Protocolo de Comunicação Bastão-ESP ↔ K10 (BLE Mesh)

Este documento define o protocolo de comunicação entre o firmware do Bastão-ESP (ESP32 coordenador) e a Tela K10 (ESP32 nó Mesh) via BLE Mesh.

---

## 1. Arquitetura

```
┌─────────────────────────┐       BLE Mesh        ┌─────────────────────────┐
│     Bastão-ESP          │ ◄─────────────────────► │         K10            │
│   (Coordenador Mesh)    │   JSON plain text      │    (Nó Mesh Node)      │
│                         │   CID=0x02A5 MID=0x0001│                        │
│ - RFID (STM32)          │                        │ - LVGL UI (LCD 240x320)│
│ - GPS (SIMCom)          │  ┌──────────────────┐  │ - Acelerômetro SC7A20  │
│ - 4G status             │  │  JSON puro (sem   │  │ - Bateria LiPo         │
│ - Bateria do sistema    │  │  criptografia)    │  │ - BLE Mesh Node        │
└─────────────────────────┘  └──────────────────┘  └─────────────────────────┘
```

### 1.1. CID do Vendor Model

**IMPORTANTE:** O CID do vendor model BLE Mesh é `0x02A5` (Espressif).

NUNCA use `0xFFFF` — o stack ESP-IDF v5.4 usa `CID_NVAL = 0xFFFF` como flag interna
para indicar modelo SIG. Com CID=0xFFFF, o bind do modelo falha silenciosamente
e o K10 nunca recebe dados.

### 1.2. Segurança

BLE Mesh link layer já oferece criptografia via AppKey + NetKey.
**Não é necessário AES-256-CBC adicional** entre coordenador e K10.
A criptografia AES-256-CBC é usada exclusivamente para o path MQTT (nuvem).

---

## 2. Mensagens: Coordenador → K10

Todas as mensagens são enviadas como **JSON plain text** via opcode `0xC00001`.
O K10 diferencia os tipos usando `strstr()` no campo `"type"`.

### 2.1. RFID Tag

**Trigger:** Cada leitura de tag RFID (STM32 → ESP32 → Mesh)
**Frequência:** Por evento (cada tag única detectada)

```json
{"type":"rfid","model":"YRM100","tag":"30751FEB705C5904E3D50D70","name":"Vaca 001","weight":450.5,"lot":"Lote A"}
```

| Campo | Tipo | Descrição |
|-------|------|-----------|
| `type` | string | `"rfid"` |
| `model` | string | `"YRM100"` ou `"WL134"` |
| `tag` | string | ID da tag RFID |
| `name` | string | Nome do animal (se encontrado no banco local) |
| `weight` | float | Peso em kg (se disponível) |
| `lot` | string | Lote (se disponível) |
| `x`/`y`/`z` | float | Acelerômetro (se leitura com accel) |
| `movement` | int | 1 se movimento detectado |

### 2.2. Bateria do Sistema

**Trigger:** Por evento (STM32 envia a cada 5s)

```json
{"type":"batt","volt":8.45}
```

### 2.3. GPS

**Trigger:** Periódico (a cada 30s)
**Fonte:** Modem SIMCom (AT+CGPSINFO)

```json
{"type":"gps","lat":-20.444204,"lon":-54.619444,"fix":1,"alt":850.5,"speed":0.5}
```

| Campo | Tipo | Descrição |
|-------|------|-----------|
| `lat` | double | Latitude em graus decimais (6 casas) |
| `lon` | double | Longitude em graus decimais (6 casas) |
| `fix` | int | 1 = GPS fix válido, 0 = sem sinal |
| `alt` | float | Altitude em metros |
| `speed` | float | Velocidade em km/h |

### 2.4. Status Celular

**Trigger:** Periódico (a cada 60s)

```json
{"type":"cell","rssi":-75,"connected":1,"operator":"VIVO"}
```

| Campo | Tipo | Descrição |
|-------|------|-----------|
| `rssi` | int | RSSI em dBm (-113 a -51) |
| `connected` | int | 1 = MQTT conectado, 0 = offline |
| `operator` | string | Nome da operadora |

---

## 3. Mensagens: K10 → Coordenador

### 3.1. Acelerômetro

**Trigger:** A cada 500ms (se houver movimento)
**Opcode:** `0xC00002`

```json
{"type":"accel","x":0.12,"y":0.05,"z":0.98,"movement":0}
```

### 3.2. Status do Display

**Trigger:** A cada 30s
**Opcode:** `0xC00003`

```json
{"type":"display","volt":3.70,"pct":85,"crit":0,"screen":1}
```

| Campo | Tipo | Descrição |
|-------|------|-----------|
| `volt` | float | Tensão da bateria da K10 |
| `pct` | int | Percentual estimado (0-100) |
| `crit` | int | 1 = bateria crítica |
| `screen` | int | 1 = tela ativa |
| `event` | string | Evento UI opcional |

---

## 4. Fluxo de Dados Completo

```
STM32 (leitura RFID)
  │ UART JSON
  v
dispatcher_task (ESP32)
  │
  ├── JSON plain ──BLE Mesh──► K10 (RFID)
  │                           ├── lbl_bovine_id = animal name
  │                           └── lbl_time_label = model
  │
system_orchestrator_task (ESP32, a cada 30s)
  │
  ├── JSON plain ──BLE Mesh──► K10 (GPS)
  │                           └── lbl_gps_coords = "-20.444204, -54.619444"
  │
  ├── JSON plain ──BLE Mesh──► K10 (Cell, a cada 60s)
  │                           └── icon_4g = verde/vermelho
  │
  └── AES-256-CBC ──MQTT──► Nuvem (telemetria criptografada)

K10 (a cada 500ms)
  │
  ├── JSON accel ──BLE Mesh──► Coordenador (movimento)
  └── JSON display ──BLE Mesh──► Coordenador (status a cada 30s)
```

---

## 5. Implementação

### 5.1. Coordenador (ESP32)

| Arquivo | Função |
|---------|--------|
| `mesh_coordinator.c` | Provisioner BLE Mesh, vendor model CID=0x02A5, envio de dados |
| `mesh_coordinator.h` | API pública: `mesh_coordinator_init()`, `mesh_coordinator_send_data()` |
| `main.c` (dispatcher_task) | Envia RFID/bateria para K10 |
| `main.c` (system_orchestrator_task) | Envia GPS/cell para K10 |

### 5.2. K10 (Display)

| Arquivo | Função |
|---------|--------|
| `k10_mesh_node.c` | Node BLE Mesh, parsing de JSON recebido |
| `k10_mesh_node.h` | Structs: `k10_last_rfid_t`, `k10_battery_status_t`, `k10_gps_data_t`, `k10_cell_status_t` |
| `gui_manager.c` | Atualização da UI LVGL |
| `main.c` (K10) | `gui_task` (polling Mesh + sensores a cada 500ms), `network_task` (BLE Mesh no Core 0) |

---

## 6. Provisionamento

1. K10 inicia como **não provisionado** (anuncia como `dev_uuid` baseado no MAC)
2. Coordenador detecta o anúncio e **provisiona** a K10
3. Coordenador adiciona **AppKey** ao nó K10
4. Coordenador faz **bind** do AppKey ao modelo vendor da K10
5. Coordenador faz **bind** do AppKey ao modelo vendor local
6. Comunicação estabelecida

**Persistência:** Com `CONFIG_BLE_MESH_SETTINGS=y`, o provisionamento persiste
entre reboots na NVS da K10.

---

## 7. Troubleshooting

| Sintoma | Causa | Solução |
|---------|-------|---------|
| K10 mostra "Model not bound" | CID=0xFFFF | Mudar para 0x02A5 |
| K10 recebeu mas não exibe | Payload encriptado AES | Enviar JSON plain |
| "No subnets to advertise" | K10 provisionada mas sem AppKey | Aguardar bind (pode levar 2-3s) |
| GPS mostra "SEM SINAL" no boot | Primeira leitura GPS demora | Aguardar até 30s para sync |

---

*Documento atualizado em: 2026-06-17*
*Versão: 2.0*

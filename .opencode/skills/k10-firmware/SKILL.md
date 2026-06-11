---
name: k10-firmware
description: Diretrizes para desenvolvimento do firmware da Tela K10, incluindo LVGL, BLE Mesh Node, display HAL e sensores (acelerometro SC7A20, bateria).
---

# K10 Firmware Skill — Tela K10

Esta skill rege o desenvolvimento do firmware da **Tela K10**, um display touch com ESP32 que atua como no BLE Mesh no ecossistema Bastao-ESP.

---

## 1. Arquitetura do Firmware K10

```
main/k10_main_updated.c
├── gui_task()              → Core 1: LVGL + display + sensores
│   ├── hal_display_init()  → Inicializacao do display
│   ├── hal_sensors_init()  → Inicializacao do acelerometro SC7A20
│   └── gui_manager_init()  → Inicializacao da interface LVGL
│
└── mesh_node_task()        → Core 0: comunicacao BLE Mesh
    └── k10_mesh_node_init()
        → k10_mesh_send_accel()
        → k10_mesh_get_last_rfid()
        → k10_mesh_get_battery_status()
        → k10_mesh_get_alert()
```

## 2. Responsabilidades

### 2.1. Interface Grafica (LVGL)
- Framework: **LVGL** (Light and Versatile Graphics Library)
- Task dedicada no Core 1 com `lv_tick_inc()` em cada iteracao
- Widgets: labels, painéis, icones para exibicao de:
  - Tags RFID lidas (modelo, tag, timestamp)
  - Nivel da bateria
  - Alertas (bateria critica)
  - Status de conexao Mesh
  - Dados do acelerometro (x, y, z)

### 2.2. Comunicacao BLE Mesh (ESP-BLE-MESH)
- **Modo:** No (Node)
- **Provisionamento:** Pelo Bastao-ESP (Coordenador)
- **Seguranca:** AES-CCM com AppKey compartilhada
- **Dados recebidos do Bastao:**
  - `k10_last_rfid_t` — Tag RFID lida (modelo, tag hex)
  - `k10_battery_status_t` — Nivel de bateria do bastao
  - `k10_alert_t` — Alertas (codigo, mensagem)
- **Dados enviados ao Bastao:**
  - Acelerometro: `{x, y, z, movement_detected}`
  - Flag de movimento (threshold > 0.15g)

### 2.3. Sensores
- **Acelerometro SC7A20:**
  - I2C, endereco padrao `0x18`
  - Leitura a cada ~500ms (50 iteracoes do loop LVGL)
  - Threshold de movimento: > 0.15g
- **Bateria:**
  - Leitura via ADC do ESP32
  - Atualizacao periodica no display

## 3. Diretrizes de Implementacao

- **Tasks:** `gui_task` no Core 1, `mesh_node_task` no Core 0
- **Compartilhamento de dados:** Ponteiros para estruturas compartilhadas (protegidas por mutex se necessario)
- **LVGL:** `lv_tick_inc(tick_diff)` no inicio do loop; `lv_task_handler()` a cada iteracao
- **Nao bloquear** a task LVGL com I/O, delays ou operacoes longas
- **Reconexao Mesh:** Tratar timeout e tentar reconexao automatica
- **Display:** Inicializar apos LVGL; usar `hal_display_flush()` como callback de flush
- **Adotar** as boas praticas de programacao C e Doxygen

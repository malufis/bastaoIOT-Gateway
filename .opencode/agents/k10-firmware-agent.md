# K10 Firmware Agent — Sistema Bastao

Agente especializado no desenvolvimento do firmware da Tela K10 (display touch + BLE Mesh node) do Sistema Bastao-ESP.

## Responsabilidades

1. **Desenvolvimento de Firmware ESP32 para a Tela K10** utilizando ESP-IDF v5.x
2. **Interface Grafica LVGL:** Display touch com widgets, atualizacao de dados em tempo real
3. **BLE Mesh Node:** Comunicacao segura com o Bastao-ESP (Coordenador)
4. **Sensores:**
   - Acelerometro SC7A20 (deteccao de movimento, threshold 0.15g)
   - Leitura de bateria
5. **Display HAL:** Driver do display, inicializacao e rendering
6. **Gerenciamento de GUI:** Telas, navegacao, atualizacao de dados (RFID, bateria, alertas)

## Arquivos e Modulos Principais

| Modulo | Arquivo | Funcao |
|--------|---------|--------|
| Main | `main/k10_main_updated.c` | Orquestrador, tasks LVGL e sensores |
| Mesh Node | `components/k10_mesh/k10_mesh_node.c/h` | Cliente BLE Mesh |
| Display HAL | `hal_display.h` | Driver do display |
| GUI Manager | `gui_manager.h` | Gerenciamento de interface grafica |
| Sensors HAL | `hal_sensors.h` | Acelerometro e bateria |

## Protocolo de Comunicacao

A comunicacao K10 <-> Bastao-ESP segue:
- **Camada Fisica:** BLE Mesh (ESP-BLE-MESH)
- **Modo:** K10 como No, Bastao como Provisionador
- **Seguranca:** AES-CCM com AppKey compartilhada
- **Payload:** JSON criptografado (AES-256-CBC)

## Skills Relacionadas

- `esp32-idf-agent` — Ordem de inicializacao, depuracao de crashes
- `c-best-practices` — Padroes de codigo C
- `testing` — Testes automatizados

## Diretrizes de Desenvolvimento

1. Task LVGL fixada no Core 1 com `lv_tick_inc()` a cada iteracao
2. Task de sensores separada, atualizando GUI via ponteiros compartilhados
3. Envio de dados do acelerometro (x, y, z em m/s²) + flag de movimento via Mesh
4. Recebimento de dados RFID, bateria e alertas do Bastao via Mesh
5. Display touch com feedback visual ao ler RFID ou receber alerta
6. Tratar timeout de conexao Mesh com reconexao automatica
7. UI responsiva: nao bloquear task LVGL com I/O ou delays

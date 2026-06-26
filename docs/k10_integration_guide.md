# Guia de Integração: Bastão-ESP ↔ K10

Este documento descreve a integração atual entre o firmware do Bastão-ESP e a Tela K10 via BLE Mesh.

---

## Status da Integração

| Componente | Status | Observação |
|------------|--------|------------|
| BLE Mesh Coordinator | ✅ Implementado | CID=0x02A5, AppKey compartilhada, binds assíncronos |
| BLE Mesh Node (K10) | ✅ Implementado | Provisiona automaticamente pelo coordenador |
| RFID → K10 | ✅ Funcionando | JSON plain a cada leitura de tag |
| Bateria → K10 | ✅ Funcionando | JSON plain a cada 5s |
| GPS → K10 | ✅ Funcionando | A cada 30s, 6 casas decimais |
| Status Celular → K10 | ✅ Funcionando | A cada 60s, ícone 4G verde/vermelho |
| Acelerômetro K10 → Coordenador | ✅ Funcionando | A cada 500ms |
| Status Display K10 → Coordenador | ✅ Funcionando | A cada 30s |

---

## Repositórios

| Firmware | Localização |
|----------|-------------|
| Bastão-ESP (Coordenador) | `D:\git\Bastao\Bastao-ESP\esp32_firmware\` |
| K10 Display | `D:\git\Bastao\Bastao-ESP\k10_firmware\` |

Ambos usam **ESP-IDF v5.4.4** e estão no mesmo repositório.

---

## Arquivos Relevantes

### Bastão-ESP (Coordenador)

| Arquivo | Função |
|---------|--------|
| `main/mesh_coordinator.c` | Provisioner BLE Mesh, CID=0x02A5, send/receive |
| `main/mesh_coordinator.h` | API `mesh_coordinator_init()`, `mesh_coordinator_send_data()` |
| `main/main.c` | `dispatcher_task` (envia RFID/batt), `system_orchestrator_task` (envia GPS/cell) |

### K10 Display

| Arquivo | Função |
|---------|--------|
| `components/k10_mesh/k10_mesh_node.c` | Node BLE Mesh, parsing JSON, send accel/display |
| `components/k10_mesh/k10_mesh_node.h` | Structs e API pública |
| `components/gui/gui_manager.c` | LVGL UI, updates de RFID, GPS, cell status |
| `components/gui/include/gui_manager.h` | Protótipos das funções de UI |
| `main/main.c` | `gui_task` (Core 1), `network_task` (Core 0) |

---

## Protocolo

### Coordenador → K10 (opcode 0xC00001)

| Tipo JSON | Frequência | Campos |
|-----------|-----------|--------|
| `{"type":"rfid",...}` | Por evento | model, tag, name, weight, lot, accel |
| `{"type":"batt",...}` | A cada 5s | volt |
| `{"type":"gps",...}` | A cada 30s | lat, lon, fix, alt, speed |
| `{"type":"cell",...}` | A cada 60s | rssi, connected, operator |

### K10 → Coordenador

| Tipo JSON | Opcode | Frequência | Campos |
|-----------|--------|-----------|--------|
| `{"type":"accel",...}` | 0xC00002 | 500ms | x, y, z, movement |
| `{"type":"display",...}` | 0xC00003 | 30s | volt, pct, crit, screen, event |

---

## Detalhes de Implementação

### BLE Mesh CID

**NUNCA use CID=0xFFFF.** O stack ESP-IDF v5.4 usa `CID_NVAL = 0xFFFF` como flag
interna para indicar modelo SIG. Use `CID=0x02A5` (Espressif).

### Criptografia

BLE Mesh NÃO usa AES-256-CBC entre coordenador e K10. O payload é JSON plain.
A segurança é garantida pela criptografia de link layer do BLE Mesh (AppKey + NetKey).
A criptografia AES-256-CBC é exclusiva para o path MQTT (nuvem).

### Provisionamento

- Com `CONFIG_BLE_MESH_SETTINGS=y`, o provisionamento persiste entre reboots na K10
- Para forçar reprovisionamento: `idf.py erase-flash flash`
- A ordem dos eventos é: PROV_ENABLE → ADD_LOCAL_APP_KEY → BIND_LOCAL_MODEL

### K10 GUI

- **lbl_gps_coords**: já existe na tela Home, atualizado com dados reais do GPS
- **icon_4g**: variável global, verde se conectado, vermelho se offline
- **lbl_bovine_id**: mostra nome do animal ou tag RFID
- **lbl_farm_title**: texto estático "Fazenda Raptor"

---

## Compilação e Flash

### Coordenador
```bash
cd esp32_firmware
python generate_config.py
idf.py set-target esp32s3
idf.py build
idf.py -p COM7 erase-flash flash monitor
```

### K10
```bash
cd k10_firmware
idf.py set-target esp32s3
idf.py build
idf.py -p COM5 erase-flash flash monitor
```

---

## Troubleshooting

| Problema | Causa | Solução |
|----------|-------|---------|
| K10 mostra "Model not bound" | CID=0xFFFF | Mudar para 0x02A5 e erase-flash |
| K10 recebe lixo hexadecimal | AES encryption ativada | Enviar JSON plain |
| K10 não aparece | BLE Mesh desligado | Verificar `CONFIG_BLE_MESH=y` |
| GPS não atualiza | K10 não reprovisionada | `idf.py erase-flash` na K10 |

---

*Documento atualizado em: 2026-06-17*
*Versão: 2.0*

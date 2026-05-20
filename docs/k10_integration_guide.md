# Guia de Integração: Bastão-ESP ↔ K10

Este documento descreve as alterações necessárias nos dois repositórios para estabelecer comunicação via BLE Mesh entre o Bastão-ESP e a Tela K10.

---

## Repositórios

| Repositório | URL |
|-------------|-----|
| Bastão-ESP | `D:\git\Bastao\Bastão-ESP` |
| K10 Firmware | `https://github.com/malufis/bastaoIOT` |

---

## 1. Alterações no Bastão-ESP (ESP32)

### 1.1 Arquivos Existentes

O módulo `mesh_coordinator.c` já está implementado mas precisa de ajustes para BLE Mesh completo:

**Arquivo:** `esp32_firmware/main/mesh_coordinator.c`

Este arquivo precisa ser expandido para:
1. Completar inicialização do Provisioner
2. Implementar envio real de mensagens Mesh
3. Adicionar whitelist de UUIDs

### 1.2 Configuração Necessária

No `sdkconfig` do ESP32, ativar:
```
CONFIG_BLE_MESH=y
CONFIG_BLE_MESH_PROVISIONER=y
CONFIG_BLE_MESH_NODE=y
CONFIG_BLE_MESH_GENERIC_ONOFF_CLIENT=y
```

---

## 2. Alterações na K10

### 2.1 Novos Arquivos Criados

Для facilitar, foram criados os seguintes arquivos (precisam ser adicionados ao repositório K10):

```
k10_firmware/components/k10_mesh/
├── CMakeLists.txt        ✓ (criado)
├── k10_mesh_node.h       ✓ (criado)
└── k10_mesh_node.c       ✓ (criado)

k10_firmware/main/
└── k10_main_updated.c    ✓ (criado - substituir main.c)
```

### 2.2 Atualização do main.c

O `main.c` precisa ter:
1. Include do header `k10_mesh_node.h`
2. Task de rede Mesh fixada no Core 0

### 2.3 Atualização do gui_manager.h

Adicionar funções para receber dados do Mesh:
- `gui_manager_update_rfid()`
- `gui_manager_update_battery_mesh()`
- `gui_manager_show_alert()`

---

## 3. Passos para Implementação

### No Bastão-ESP:

1. Revisar `mesh_coordinator.c` e ativar `CONFIG_BLE_MESH` no sdkconfig
2. Testar compilação: `idf.py build`

### Na K10:

1. Criar pasta `k10_firmware/components/k10_mesh/`
2. Adicionar arquivos: `CMakeLists.txt`, `k10_mesh_node.h`, `k10_mesh_node.c`
3. Atualizar `CMakeLists.txt` principal para incluir o componente k10_mesh
4. Substituir `main.c` pela versão atualizada
5. Adicionar funções ao `gui_manager.h/c`
6. Compilar: `idf.py build`

---

## 4. Formato do Payload

Os dados são enviados como JSON criptografado em AES-256-CBC:

```json
{"type":"rfid","model":"YRM100","tag":"30751FEB705C5904E3D50D70","timestamp":1704067200}
{"type":"batt","volt":8.45}
{"type":"alert","code":"batt_critical","volt":7.8}
```

### Descriptografia na K10

A K10 precisa ter a mesma chave AES-256 do Bastão-ESP para descriptografar.

---

## 5. Arquivos de Referência Criados

| Arquivo | Descrição |
|---------|-----------|
| `docs/k10_communication_protocol.md` | Protocolo completo de comunicação |
| `k10_firmware/components/k10_mesh/` | Módulo Mesh para K10 |
| `k10_firmware/main/k10_main_updated.c` | main.c atualizado |
| `k10_firmware/components/gui/include/gui_manager_updated.h` | Header atualizado |

---

## 6. Próximos Passos

1. **Adicionar arquivos ao repositório K10** - Copiar os arquivos criados para o repositório local da K10
2. **Compilar e testar** - Verificar se tudo compila corretamente
3. **Testar comunicação** - Com os dois dispositivos flashados, verificar se os dados aparecem na K10

---

## 7. Tabela de Compatibilidade

| Campo | Bastão-ESP (Envio) | K10 (Recebimento) |
|-------|-------------------|-------------------|
| `type` | ✅ Implementado | ✅ Parser existente |
| `model` | ✅ Implementado | ✅ Parser existente |
| `tag` | ✅ Implementado | ✅ Parser existente |
| `name` | ✅ (animal_db) | ✅ Parser existente |
| `weight` | ✅ (animal_db) | ✅ Parser existente |
| `lot` | ✅ (animal_db) | ✅ Parser existente |
| `batt/volt` | ✅ Implementado | ✅ Parser existente |
| `alert` | ✅ Implementado | ✅ Parser existente |

---

*Documento gerado em: 2026-05-20*
*Versão: 1.0*
*Projeto: Bastão-ESP ↔ K10 Integration*
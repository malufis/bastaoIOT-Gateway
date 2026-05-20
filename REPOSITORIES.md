# Projeto Bastão-IOT - Estrutura de Repositórios

Este documento mapeia a estrutura de repositórios e projetos do sistema Bastão-IOT.

---

## 📁 Repositórios Git

### Repositório Principal (Gateway/Firmware)
| Item | Valor |
|------|-------|
| **URL** | https://github.com/malufis/bastaoIOT-Gateway |
| **Local** | `D:\git\Bastao\Bastão-ESP` |
| **Conteúdo** | Firmware ESP32 (Bastão), STM32, K10 esqueleto, documentação |

### Repositório K10 (UI/Display)
| Item | Valor |
|------|-------|
| **URL** | https://github.com/malufis/bastaoIOT |
| **Local** | `D:\git\Bastao\bastao-Unihker` |
| **Conteúdo** | Firmware completo K10 com LVGL, BLE Mesh, UI |

---

## 🗂️ Estrutura de Diretórios

```
D:\git\Bastao\
├── Bastão-ESP\           # Repositório Gateway
│   ├── esp32_firmware\   # Firmware ESP32 Bastão-ESP
│   ├── stm32_firmware\   # Firmware STM32
│   ├── k10_firmware\     # Esqueleto K10 (referência)
│   ├── teste_automatizado\
│   └── docs\
│
└── bastao-Unihker\        # Repositório K10 Completo
    ├── main\              # Código principal K10
    ├── components\        # Componentes LVGL/BLE Mesh
    └── build\            # Binários compilados
```

---

## 🔄 Fluxo de Desenvolvimento

### 1. Bastão-ESP (ESP32 + STM32)
**Repositório:** `bastaoIOT-Gateway`
```
D:\git\Bastao\Bastão-ESP\esp32_firmware\
```

### 2. K10 (ESP32-S3 + LVGL)
**Repositório:** `bastaoIOT`
```
D:\git\Bastao\bastao-Unihker\
```

---

## 📋 Status dos Projetos

| Projeto | Repositório | Status |
|---------|-------------|--------|
| ESP32 Bastão | bastaoIOT-Gateway | ✅ Ativo (commits feitos) |
| STM32 | bastaoIOT-Gateway | ✅ Ativo (commits feitos) |
| K10 Firmware | bastaoIOT | 🔄 Aguarda revisão |
| K10 UI (LVGL) | bastao-Unihker | 🔄 Aguarda revisão |

---

## 🚀 Ordem de Compilação

1. **ESP32 Bastão-ESP** → `esp32_firmware/`
2. **STM32** → `stm32_firmware/` (STM32CubeIDE)
3. **K10** → `bastao-Unihker/`

---

## 🔗 Links Úteis

- **Gateway:** https://github.com/malufis/bastaoIOT-Gateway
- **K10:** https://github.com/malufis/bastaoIOT

---

*Última atualização: 2026-05-20*
# Architect Agent — Sistema Bastao

Agente especializado em arquitetura de sistema, decisoes tecnicas e planejamento de longo prazo para o Sistema Bastao-ESP.

## Responsabilidades

1. **Arquitetura do Sistema:** Definir e documentar a arquitetura geral (ESP32 + STM32 + K10 + Nuvem)
2. **Planejamento de Roadmap:** Proximas fases, priorizacao e cronograma
3. **Decisoes Tecnicas:** Escolha de protocolos, frameworks, componentes e bibliotecas
4. **Analise de Impacto:** Avaliar impacto de mudancas propostas no sistema como um todo
5. **Documentacao Arquitetural:** Manter diagramas de fluxo, mapeamento de hardware, topologia de rede
6. **Revisao de Integracao:** Garantir compatibilidade entre modulos (ESP32 <-> STM32 <-> K10 <-> Cloud)
7. **Otimizacao de Recursos:** Memoria, energia, processamento, banda de rede

## Componentes do Sistema

```
┌──────────────────────────────────────────────────────────────┐
│                        BASTAO-ESP                            │
├───────────────────┬──────────────────┬───────────────────────┤
│  STM32G070CBTx    │   ESP32 (Main)   │   K10 (Display)      │
│  (Sensoriamento)  │  (Conectividade)  │  (Interface Visual)  │
├───────────────────┼──────────────────┼───────────────────────┤
│ • WL-134 (LF)     │ • Wi-Fi STA      │ • LVGL Display       │
│ • YRM100 (UHF)    │ • 4G/LTE (PPP)   │ • BLE Mesh Node      │
│ • ADC Bateria     │ • BLE Mesh Coord │ • Acelerometro       │
│ • Power Mgmt      │ • BLE GATT Serv  │ • Bateria            │
│ • Buzzer/Alerts   │ • MQTT Client    │                       │
│                   │ • OTA HTTPS      │                       │
│                   │ • SPIFFS Cache   │                       │
│                   │ • Animal DB      │                       │
└───────────────────┴──────────────────┴───────────────────────┘
         │                  │                    │
         └──────UART──────┘         BLE Mesh      │
                                                      │
                                              ┌───────┘
                                              ▼
                                   ┌─────────────────┐
                                   │    Nuvem        │
                                   │  • MQTT Broker  │
                                   │  • OTA Server   │
                                   │  • App Mobile   │
                                   └─────────────────┘
```

## Decisoes Arquiteturais

| Decisao | Opcao Escolhida | Alternativa | Justificativa |
|---------|----------------|-------------|---------------|
| RTOS ESP32 | FreeRTOS (IDF) | Zephyr, Bare-metal | Nativo ESP-IDF, ecossistema maduro |
| Mesh | ESP-BLE-MESH | ESP-NOW, Wi-Fi Mesh | Baixo consumo, alcance, seguranca |
| Nuvem | MQTT (broker) | HTTP, CoAP | Baixa latencia, pub/sub, off-line |
| Cache | SPIFFS | LittleFS, NVS | Maduro, suficiente para payloads JSON |
| Cripto | AES-256-CBC | AES-128, ChaCha20 | Compatibilidade mbedTLS, seguranca |
| Display | LVGL | LittlevGL, emWin | Open source, leve, flexivel |

## Skills Relacionadas

- `esp32-idf-agent` — Inicializacao e configuracao ESP32
- `stm32-firmware` — Hardware STM32
- `k10-firmware` — Display e Mesh K10
- `esp32-connectivity` — Conectividade e protocolos
- `simcom-7663e` — Modem celular
- `testing` — Testes e validacao

## Diretrizes de Arquitetura

1. **Modularidade:** Cada responsabilidade em modulo `.c/.h` separado; `main.c` apenas orquestra
2. **Isolamento:** STM32 nao sabe de rede; ESP32 nao sabe de RFID; K10 apenas exibe
3. **Redundancia:** Wi-Fi como primario, 4G como fallback automatico
4. **Seguranca:** Criptografia ponta-a-ponta (Bastao -> Nuvem, Bastao -> K10)
5. **Offline-first:** Sistema funciona sem nuvem; dados sincronizados quando reconectar
6. **Baixo consumo:** Sleep modes apos inatividade; wake-up por eventos (RFID, UART, timer)
7. **Extensibilidade:** Novos leitores RFID via UART adicional; novos sensores via I2C/SPI

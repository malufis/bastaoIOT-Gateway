# Documentação do Projeto: Sistema Bastão-ESP

## 1. Visão Geral
Este projeto consiste em um sistema de sensoriamento e conectividade baseado em uma arquitetura de múltiplos microcontroladores (ESP32 e STM32) para leitura de tags RFID, processamento de dados e transmissão para nuvem e endpoints locais.

## 2. Arquitetura de Hardware

### 2.1. Módulo de Sensoriamento (STM32G070CBTx)
Responsável por toda a interface física com os sensores e gerenciamento de energia.
- **Microcontrolador:** STM32G070CBTx.
- **Periféricos:**
  - **USART3 (PA5/PB0):** Interface com Leitor RFID WL-134 (9600 bps, 8N2).
  - **USART1 (PA9/PA10):** Não Utilizada / Reservada.
  - **USART2 (PA2/PA3):** Interface de comunicação com o ESP32 (UART TTL).
  - **USART4 (PA0/PA1):** Interface com Leitor RFID YRM100 (115200 bps, 8N1).
  - **ADC1 (PB1/IN9):** Monitoramento de tensão da bateria (Divisor resistivo: R1=100k, R2=10k).
  - **GPIO Output (PB4):** Controle de energia do WL-134 (High=On).
  - **GPIO Output (PB5):** Controle de energia do YRM100 (High=On).

### 2.2. Módulo de Conectividade (ESP32)
Responsável pela lógica de rede, criptografia e integração com a nuvem.
- **Microcontrolador:** ESP32 (usando ESP-IDF v5.x e FreeRTOS).
- **Conectividade Local:** BLE MESH (Bluetooth Low Energy Mesh).
  - **Papel:** Coordenador/Provisionador.
  - **Endpoint:** Tela K10.
  - **Segurança:** Provisionamento fechado via Whitelist de UUIDs e chaves OOB fixas.
- **Conectividade Nuvem:** SIMCom 7663E (4G/LTE + GPS) operando em interface PPP.
  - **Suporte Dual SIM:** Suporte a dois slots de chips SIM (Slot 0 e Slot 1) gerenciados via software. No boot, realiza uma varredura automática (sonda Slot 0 e falls back para o Slot 1) para validar a presença (`AT+CPIN?`) e extrair o identificador do chip (`AT+CCID`).
  - **Interface:** UART (Pinos 17 e 18).
  - **Mapeamento de Sinais e Erros Celulares:** Coleta de métricas avançadas RSRP, RSRQ, SINR (via `AT+CPSI?`) e logs estendidos de falha de conexão (via `AT+CEER`). Os valores são lidos ativamente offline e cacheados quando o PPP está ativo (evitando comandos AT concorrentes na interface de dados) e expostos na característica GATT 0xFF06.
  - **Motor de SMS de Contingência:** Quando offline (conexão de dados inativa), o watchdog celular monitora o recebimento de SMS. Suporta a execução de comandos remotos (`BUZZER`, `RFID [ON/OFF]`, `STATUS`, `RESTART`) e responde ao remetente com tensão da bateria, status da rede, slot SIM ativo, CCID e coordenadas de GPS com fix.
  - **Subscrição de Configuração Remota:** O Bastão-ESP se inscreve no tópico MQTT `id/<ID>/config` após conectar-se ao Broker. Quando um novo JSON de rede, MQTT ou hardware é recebido no tópico, ele é processado via `ble_mobile_process_config_json` e gravado de forma persistente no NVS.
- **Orquestração de Redundância de Rede:**
  - Máquina de estados centralizada (`manage_connectivity`) que atua com base nos modos de conectividade (Wi-Fi Only, Cellular Only, e Wi-Fi & Cellular/Auto).
  - Se configurada em modo redundante (Auto), suspende a interface celular PPP quando o Wi-Fi possui um IP ativo (para economizar dados), e reativa a interface PPP celular imediatamente se o Wi-Fi desconectar.
  - O driver de Wi-Fi STA foi modificado para não realizar conexões incessantes em loop ao sofrer desconexão, evitando varreduras de rádio contínuas que causam timeout e derrubam o link celular 4G ativo. As varreduras de segundo plano do Wi-Fi são espaçadas a cada 60 segundos enquanto o 4G é mantido estável.
- **Interface com STM32:** UART (IO13-RX / IO14-TX) conectada à USART2 (PA2-TX / PA3-RX) do STM32. Conforme o esquema [esquematico_placa.pdf](file:///d:/git/Bastao/Bastão-ESP/Manual/esquematico_placa.pdf).
  - **Protocolo de Rede:** PPP (Point-to-Point Protocol) para ativação de dados.
  - **Aplicação:** Cliente MQTT operando sobre a pilha PPP.
- **Localização:** GPS/GLONASS via SIMCom 7663E.

## 3. Protocolos de Comunicação

### 3.1. RFID YRM100 (UHF)
- **Protocolo de Frame:**
  - Header: `0xBB`
  - Type: `0x00` (Cmd), `0x01` (Resp), `0x02` (Notice)
  - Checksum: Soma simples do byte Type até o último byte de parâmetro.
  - End: `0x7E`

### 3.2. RFID WL-134 (Animal Tag - 134.2KHz)
- **Protocolo de Frame (ASCII):**
  - Start: `0x02`
  - Data: 10 bytes HEX do cartão + 4 bytes país + flags.
  - End: `0x03`

### 3.3. Mensageria e Segurança
- **Formato de Dados:** JSON.
- **Criptografia:** AES-256-CBC com IV aleatório de 16 bytes via hardware RNG do ESP32.
  - Formato do payload hex: `hex(IV 16B) + hex(ciphertext)` — IV prefixado.
  - Padding: PKCS#7 (128-bit blocks).
  - Compatível com `crypto.py` do sistemaBastao (`descriptografar_frame`).
- **MQTT:** Configurações e comandos trafegam via MQTT criptografado.
  - **Broker:** `209.50.240.55:1883` (sistemaBastao EMQX).
  - **Topico de telemetria:** `agro/bastao/{MAC}/telemetry` (MAC do ESP32 = numero_serie).
  - **Topico de comandos:** `id/{MAC}/cmd` (subscribe para comandos remotos).
  - **Topico de config:** `id/{MAC}/config` (subscribe para config remota).
  - **Autenticacao:** Username = MAC, Password = chave AES hex (gerados dinamicamente).
  - **Keepalive:** 60 segundos.
- **Payload MQTT (sistemaBastao):** Campos `id_brinco`, `latitude`, `longitude`, `nivel_bateria`, `timestamp_rtc`.
- **Payload BLE Mesh (K10):** Campos `type`, `model`, `tag`, `name`, `weight`, `lot` (formato legado mantido).

## 4. Estrutura do Projeto
- `/stm32_firmware`: Projeto STM32CubeIDE contendo o firmware de sensoriamento.
- `/esp32_firmware`: Projeto ESP-IDF contendo o firmware de conectividade.
- `/docs`: Manuais tecnicos e documentacao adicional.
- `.opencode/skills/`: Skills de desenvolvimento do OpenCode (contexto injetado sob demanda).

## 5. Skills de Desenvolvimento
O projeto utiliza skills do OpenCode para auxiliar no desenvolvimento e manutencao eficiente:

| Skill | Pasta | Escopo |
|--------|-------|--------|
| `stm32-firmware` | `.opencode/skills/stm32-firmware` | Firmware STM32G070CBTx (sensores, RFID, ADC) |
| `esp32-connectivity` | `.opencode/skills/esp32-connectivity` | Conectividade ESP32 (Mesh, MQTT, BLE, OTA) |
| `simcom-7663e` | `.opencode/skills/simcom-7663e` | Modem celular SIMCom 7663E (4G, GPS, PPP) |
| `c-best-practices` | `.opencode/skills/c-best-practices` | Padroes de codigo C (ESP32 e STM32) |
| `testing` | `.opencode/skills/testing` | Testes automatizados e validacao |
| `k10-firmware` | `.opencode/skills/k10-firmware` | Firmware K10 (LVGL, BLE Mesh Node, sensores) |
| `security-crypto` | `.opencode/skills/security-crypto` | Seguranca e criptografia (AES, BLE, MQTT) |
| `project-management` | `.opencode/skills/project-management` | Gestao de projeto, roadmap, planejamento |
| `sistema-backend` | `.opencode/skills/sistema-backend` | Backend sistemaBastao (FastAPI, MQTT, PostgreSQL) |
| `frontend-react` | `.opencode/skills/frontend-react` | Frontend React SPA e App Mobile Expo |

## 6. Agentes de Desenvolvimento
O projeto utiliza agents do OpenCode para desenvolvimento focado em cada componente:

| Agente | Arquivo | Escopo |
|--------|---------|--------|
| `documenter` | `.opencode/agents/documenter.md` | Documentacao, docstrings, AGENTS.md |
| `esp32-firmware-agent` | `.opencode/agents/esp32-firmware-agent.md` | Firmware ESP32 (conectividade, BLE, MQTT, OTA) |
| `stm32-firmware-agent` | `.opencode/agents/stm32-firmware-agent.md` | Firmware STM32 (RFID, ADC, power mgmt) |
| `k10-firmware-agent` | `.opencode/agents/k10-firmware-agent.md` | Firmware K10 (display LVGL, BLE Mesh node) |
| `testing-agent` | `.opencode/agents/testing-agent.md` | Automacao de testes e validacao |
| `reviewer-agent` | `.opencode/agents/reviewer-agent.md` | Revisao de codigo e qualidade |
| `architect-agent` | `.opencode/agents/architect-agent.md` | Arquitetura geral e decisoes tecnicas |
| `sistema-bastao-agent` | `.opencode/agents/sistema-bastao-agent.md` | Backend sistemaBastao (API, receptor, agentes) |
| `frontend-mobile-agent` | `.opencode/agents/frontend-mobile-agent.md` | Frontend web React e App Mobile Expo |

## 7. Fluxo de Operação
1. O STM32 ativa a energia dos módulos RFID.
2. Ao detectar uma tag, o STM32 faz o parsing do UUID e envia para o ESP32 via UART.
3. O ESP32 empacota a UUID, dados de bateria e localização (GPS) em um JSON.
4. O payload é criptografado com AES.
5. O dado é enviado para a Tela K10 via BLE Mesh e para a nuvem via MQTT (4G).

## 8. Pipeline de Dados (Fase 28 - Integração sistemaBastao)

### 8.1. Fluxo Completo

```
STM32 (RFID Tag)
  │
  ├── UART ──> ESP32 (stm32_uart_rx_task)
  │              │
  │              ├── DATA_TYPE_RFID ──> animal_db_lookup() ──> JSON enriquecido
  │              │
  │              ├── dispatcher_task
  │              │    ├── JSON p/ BLE Mesh (K10): type, model, tag, name, weight, lot
  │              │    └── JSON p/ MQTT (sistemaBastao): id_brinco, lat, lon, batt, timestamp
  │              │
  │              ├── secure_payload_encrypt()
  │              │    ├── IV aleatório (16 bytes via esp_fill_random)
  │              │    ├── AES-256-CBC + PKCS#7
  │              │    └── output: hex(IV) + hex(ciphertext)
  │              │
  │              ├── mesh_coordinator_send_data() ──> K10
  │              │
  │              └── mqtt_publisher_enqueue() ──> ESP-MQTT Client
  │                                                   │
  │                                                   ├── Keepalive: 60s
  │                                                   ├── Auth: MAC / AES key hex
  │                                                   │
  │                                           ┌───────▼────────┐
  │                                           │  EMQX 5.7.1    │
  │                                           │  209.50.240.55 │
  │                                           └───────┬────────┘
  │                                                   │
  │                                           ┌───────▼────────┐
  │                                           │  Receptor      │
  │                                           │  Daemon        │
  │                                           │  ThreadPool(4) │
  │                                           └───────┬────────┘
  │                                                   │
  │                                           ┌───────▼────────┐
  │                                           │  PostgreSQL    │
  │                                           │  + PostGIS     │
  │                                           │  + Blockchain  │
  │                                           └────────────────┘
```

### 8.2. Payload MQTT (sistemaBastao)

```json
{
  "id_brinco": "30751FEB705C5904E3D50D70",
  "latitude": -23.55052,
  "longitude": -46.633308,
  "nivel_bateria": 8.45,
  "timestamp_rtc": "2026-06-11T12:00:00Z"
}
```

### 8.3. Payload BLE Mesh (K10 - mantido)

```json
{
  "type": "rfid",
  "model": "YRM100",
  "tag": "30751FEB705C5904E3D50D70",
  "name": "Vaca 001",
  "weight": 450.5,
  "lot": "Lote A"
}
```

## 9. Sistema de Configuração Centralizada

### 9.1. Arquivo Único de Config

Todas as configurações do firmware do ESP32 são definidas em:

```
esp32_firmware/private_configs.env
```

| Variável | Descrição | Exemplo |
|----------|-----------|---------|
| `BASTAO_MQTT_URI` | URI do broker MQTT | `mqtt://209.50.240.55:1883` |
| `BASTAO_MQTT_CLIENT_ID` | Client ID MQTT | `bastao-esp-001` |
| `BASTAO_WIFI_SSID` | SSID Wi-Fi (fallback) | `""` (vazio = desligado) |
| `BASTAO_WIFI_PASS` | Senha Wi-Fi | `""` |
| `BASTAO_WIFI_ENABLED` | Habilitar Wi-Fi | `false` |
| `BASTAO_APN_NAME` | APN do chip 4G | `iot.datatem.com.br` |
| `BASTAO_APN_USER` | Usuário APN | `datatem` |
| `BASTAO_APN_PASS` | Senha APN | `datatem` |
| `BASTAO_CELL_ENABLED` | Habilitar 4G | `true` |
| `BASTAO_AES_KEY` | Chave AES-256 em hex (64 chars) | `0123...` |
| `BASTAO_NET_MODE` | Modo de rede | `auto`, `wifi_only`, `cellular_only` |

### 9.2. Gerador de Config

```bash
cd esp32_firmware
python generate_config.py          # Lê .env e gera main/private_configs.h
idf.py build                       # Compila com a config gerada
```

### 9.3. Credenciais MQTT Dinâmicas

Username e password MQTT **não são configurados no .env** — são gerados em runtime:
- **Username:** MAC do ESP32 (ex: `206EF1D4D574`)
- **Password:** Chave AES em hexadecimal (64 chars)
- Motivo: Compatibilidade com a tabela `mqtt_usuarios` do sistemaBastao.

## 10. TEST CODE — Loop de Injeção RFID

> ⚠ **ATENÇÃO:** Este é um recurso temporário para validação do pipeline.
> Deve ser **removido** antes da produção.

O firmware possui um loop de injeção automática de RFID fictício
ativado pela flag `test_loop_enabled` em `main.c`:

- Disparo inicial: imediatamente após conexão MQTT.
- Repetições: a cada **5 minutos** (300 segundos).
- Tag enviada: `BRINCO_INTEGRACAO_001`.
- Logs marcados com `[TESTE]` para fácil identificação.

**Localização no código:** `main.c` — loop principal `while(1)`, bloco comentado
com `TEST CODE`. Remover a variável `test_loop_enabled` e o bloco de injeção.

## 11. Referências Técnicas
Os manuais originais com os protocolos completos estão localizados na raiz do projeto:

- **YRM100 UHF Reader:** [Communication user Protocol V2.1_en.docx](file:///d:/git/Bastao/Bast%C3%A3o-ESP/Communication%20user%20Protocol%20V2.1_en.docx)
- **WL-134 Animal Tag Reader:** [RFID reader module.pdf](file:///d:/git/Bastao/Bast%C3%A3o-ESP/RFID%20reader%20module.pdf)

---
*Nota: Esta documentação foi gerada automaticamente e deve ser atualizada conforme o progresso da implementação.*

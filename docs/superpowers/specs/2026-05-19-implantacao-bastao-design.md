# Especificação de Design: Estruturação e Implantação do Bastão-ESP

Este documento formaliza as decisões de design de arquitetura, estruturação de diretórios, regras de versionamento por Git e a configuração de agentes de IA para o sistema Bastão-ESP.

## 1. Estrutura de Diretórios do Projeto
Para garantir a modularidade e a clareza para desenvolvedores humanos e agentes de IA, o repositório é estruturado da seguinte forma:

```text
Bastão-ESP/
├── .agents/                           # Arquivos de contexto dos agentes de IA
│   ├── c_best_practices_agent.md      # Boas práticas globais de C/C++
│   ├── esp32_connectivity_agent.md   # Contexto específico do firmware ESP32
│   ├── rfid_agent.md                  # Contexto e lógicas dos protocolos RFID (atualizado)
│   ├── simcom_7663e_agent.md          # Contexto específico do modem celular SIMCom
│   └── stm32_agent.md                 # Contexto específico do firmware STM32 (novo)
├── Manual/                            # Manuais técnicos para programadores humanos
│   ├── arquitetura.md                 # Conexões físicas (pinagem) e fluxos de dados
│   ├── funcionalidades.md             # Lógicas de código e detalhamento das funções
│   └── git_guidelines.md              # Regras de fluxo de trabalho do Git e versionamento
├── aprendizado/                       # Base de conhecimento de protocolos para IAs
│   └── rfid_protocols.md              # Especificação matemática dos leitores de RFID
├── teste_automatizado/                # Pasta destinada a scripts de teste e simulações
├── debug/                             # Pasta destinada a logs e dumps de análise
├── esp32_firmware/                    # Código-fonte do firmware ESP32 (ESP-IDF)
├── stm32_firmware/                    # Código-fonte do firmware STM32 (STM32CubeIDE)
└── README.md                          # Visão geral genérica do projeto
```

---

## 2. Diretrizes de Versionamento Git
O controle de código e a evolução do projeto seguirão regras estritas de branchs, commits e versionamento semântico:

### 2.1. Modelo de Branches
* **`main`**: Contém apenas código testado, consolidado e considerado estável para produção em hardware.
* **`develop`**: Ponto central de integração de novas funcionalidades.
* **`feature/*`**: Branches temporárias criadas a partir da `develop` para implementação de novas lógicas ou drivers.
* **`bugfix/*`**: Correções de erros identificados na branch `develop`.
* **`hotfix/*`**: Correções emergenciais criadas a partir da `main` e aplicadas diretamente na `main` e `develop`.

### 2.2. Padrão de Commits
Mensagens estruturadas no formato: `<tipo>: <descrição breve>`
* `feat`: Adição de nova funcionalidade.
* `fix`: Correção de bug.
* `docs`: Alteração em manuais ou arquivos markdown.
* `refactor`: Melhoria de código sem alteração functional.
* `test`: Criação ou ajuste de testes.

### 2.3. Versionamento Semântico
Numeração de versão seguindo o padrão `vMajor.Minor.Patch`:
* **Major**: Alterações estruturais que quebram retrocompatibilidade (ex: mudança física de pinos ou MCUs).
* **Minor**: Adição de recursos compatíveis (ex: suporte a nova tag ou novo comando).
* **Patch**: Correções de bugs sem novas funções.

---

## 3. Fluxo de Dados e Integração Física
O sistema opera através da cooperação de dois microcontroladores:

1. **Sensoriamento (STM32G070CBTx):**
   * Coleta dados do leitor WL-134 (LF, 134.2 KHz) via USART3 (PA5/PB0, 9600 bps 8N2) e do leitor YRM100 (UHF) via USART4 (PA0/PA1, 115200 bps 8N1).
   * Controla a energia dos módulos através de PB4 (WL-134) e PB5 (YRM100).
   * Mede a tensão da bateria no pino PB1 usando ADC1 com divisor de 100kΩ/10kΩ.
   * Transmite pacotes JSON estruturados via USART2 (PA2/PA3, 115200 bps 8N1) para o ESP32.

2. **Conectividade (ESP32):**
   * Recebe os payloads JSON via UART (IO13-RX / IO14-TX) a partir do STM32 (PA2-TX / PA3-RX), conforme detalhado no esquema elétrico [esquematico_placa.pdf](file:///d:/git/Bastao/Bastão-ESP/Manual/esquematico_placa.pdf) (Páginas 2 e 11).
   * Criptografa as mensagens usando AES.
   * Distribui localmente via BLE Mesh (UUID whitelist e OOB fixo) para a Tela K10 e remotamente via MQTT sobre interface PPP operada pelo modem SIMCom 7663E.

---

## 4. Diretrizes Técnicas de Firmware

### 4.1. STM32
* **Mecanismo UART:** A leitura deve obrigatoriamente usar interrupções ou DMA em conjunto com buffers circulares para evitar perda de bytes de dados assíncronos.
* **Mecanismo ADC:** Filtragem das amostras de bateria por média móvel para atenuar ruídos de chaveamento da placa.

### 4.2. ESP32
* **Arquitetura FreeRTOS:** Tasks isoladas com comunicação por filas de mensagens (queues) para lidar com a recepção serial, processamento/criptografia, transmissão Mesh e comunicação celular.

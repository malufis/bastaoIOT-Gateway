# Manual de Funcionalidades do Sistema Bastão-ESP

Este documento descreve detalhadamente o funcionamento de cada algoritmo implementado nos firmwares do STM32 e ESP32.

---

## 1. Funcionalidades do STM32 (Hub de Sensoriamento)

O firmware do STM32G070CBTx gerencia a leitura assíncrona dos sensores usando uma arquitetura baseada em interrupções e processamento no loop principal de forma cooperativa.

### 1.1. Callback de Interrupção de Recepção UART (`HAL_UART_RxCpltCallback`)
* **Objetivo:** Capturar caracteres da serial em tempo real sem bloquear a execução do sistema.
* **Funcionamento:**
  1. A interrupção é acionada toda vez que 1 byte é recebido via USART3 (WL-134) ou USART4 (YRM100).
  2. Identifica qual porta gerou a interrupção.
  3. Insere o byte no buffer circular correspondente (`buffer_wl134` ou `buffer_yrm100`) na posição apontada pelo ponteiro `head`.
  4. Incrementa o ponteiro `head` usando módulo (`head = (head + 1) % RFID_BUFFER_SIZE`) para evitar transbordamento.
  5. Reinicia a escuta da interrupção chamando `HAL_UART_Receive_IT` novamente.

### 1.2. Processador de Tag UHF (`RFID_Process_YRM100`)
* **Objetivo:** Fazer o parsing de frames do leitor YRM100 no loop principal e extrair o EPC da tag.
* **Funcionamento:**
  1. Varre o buffer circular a partir do ponteiro `tail`.
  2. Localiza o byte de cabeçalho `0xBB`.
  3. Verifica se há pelo menos 7 bytes disponíveis no buffer (comprimento mínimo de um frame).
  4. Extrai o campo de tamanho do parâmetro `PL` (2 bytes).
  5. Aguarda até que todos os bytes do frame (`7 + PL`) estejam disponíveis no buffer circular.
  6. Calcula o Checksum somando todos os bytes a partir de `Type` até o último byte do `Parameter`, extraindo o módulo 256.
  7. Compara o Checksum calculado com o contido no frame e valida se o byte de fim é `0x7E`.
  8. Se válido e o tipo de comando for notificação de tag (`0x22`), extrai o EPC (ignorando bytes de RSSI e PC) e converte os bytes em string hexadecimal formatada.
  9. Envia a string em JSON via USART2 (`huart2`) para o ESP32:
     ```json
     {"type":"rfid","model":"YRM100","tag":"30751FEB705C5904E3D50D70"}
     ```
  10. Ajusta o ponteiro `tail` consumindo o pacote processado.

### 1.3. Processador de Tag LF (`RFID_Process_WL134`)
* **Objetivo:** Fazer o parsing do frame ASCII do leitor WL-134 e converter os dados da tag animal de LSB para decimal.
* **Funcionamento:**
  1. Varre o buffer circular buscando o caractere de início `0x02` (STX).
  2. Verifica se o buffer contém pelo menos 30 bytes (comprimento do frame WL-134).
  3. Varre o trecho buscando o caractere final `0x03` (ETX).
  4. Extrai a string de dados de 26 caracteres ASCII que compõem o corpo do dado:
     * **Card Number (Bytes 2 a 11):** 10 caracteres hexadecimais invertidos (LSB First).
     * **Country Code (Bytes 12 a 15):** 4 caracteres hexadecimais invertidos (LSB First).
     * **Data/Animal Flags (Bytes 16 e 17).**
  5. Inverte caractere a caractere as strings de Código do Cartão e Código do País.
  6. Converte as strings invertidas de Hexadecimal para números decimais.
  7. Valida o Checksum calculando o XOR cumulativo de todos os caracteres ASCII (corpo) e comparando com o byte de checksum do pacote (Byte 28) e seu inverso (Byte 29).
  8. Transmite o JSON via USART2 para o ESP32:
     ```json
     {"type":"rfid","model":"WL-134","tag":"900250000023921"}
     ```
  9. Avança o ponteiro `tail` consumindo os bytes correspondentes.

### 1.4. Monitoramento da Bateria (`Battery_Read`)
* **Objetivo:** Medir a tensão real da bateria e enviar via telemetria para o ESP32 a cada 5 segundos.
* **Funcionamento:**
  1. Configura o canal do ADC para ler a porta PB1 (`ADC_CHANNEL_9`).
  2. Dispara a conversão analógico-digital e aguarda a conclusão (`HAL_ADC_PollForConversion`).
  3. Lê o valor bruto do registrador ADC (conversor de 12 bits, escala de 0 a 4095).
  4. Converte o valor baseado no divisor de tensão (R1 = 100kΩ, R2 = 10kΩ, resultando na atenuação de $1/11$):
     $$\text{Tensão} = \left(\frac{\text{Valor ADC} \times 3.3\text{V}}{4095}\right) \times 11.0$$
  5. Envia o JSON correspondente via USART2:
     ```json
     {"type":"batt","volt":8.45}
     ```

---

## 2. Funcionalidades do ESP32 (Conectividade e MESH)

O firmware do ESP32 é construído sobre o ESP-IDF v5.x com uma estrutura multitarefa (FreeRTOS) para garantir que processos de rede não bloqueiem as leituras locais de sensores.

### 2.1. Task UART Receiver
* **Objetivo:** Escutar a comunicação serial vinda do STM32 de forma não bloqueante.
* **Funcionamento:**
  1. Bloqueia a execução aguardando dados na UART vinculada ao STM32.
  2. Lê strings terminadas em caractere de nova linha (`\n`).
  3. Valida a estrutura JSON básica utilizando a biblioteca `cJSON`.
  4. Insere o dado validado na fila global de processamento do sistema (`DataQueue`).

### 2.2. Task Cryptography
* **Objetivo:** Garantir a segurança dos payloads de telemetria locais e remotos.
* **Funcionamento:**
  1. Consome os JSONs recebidos da `DataQueue`.
  2. Aplica criptografia simétrica baseada em **AES-128** (padrão de hardware do ESP32).
  3. Encaminha o buffer criptografado para as filas específicas de envio (Fila Mesh e Fila Celular/MQTT).

### 2.3. Task BLE Mesh Coordinator
* **Objetivo:** Transmitir leituras de tags em tempo real de forma segura para a Tela K10 local.
* **Funcionamento:**
  1. Inicializa o stack ESP-BLE-MESH no modo Provisioner (Coordenador da rede).
  2. Garante a Whitelist de dispositivos aceitando conexões apenas de UUIDs cadastrados em tempo de compilação.
  3. Quando um dado criptografado chega na fila de envio, monta o frame Mesh com criptografia nativa AES-CCM (usando a AppKey da rede).
  4. Transmite a mensagem para a Tela K10.

### 2.4. Task Cellular & SIMCom 7663E
* **Objetivo:** Estabelecer a conectividade de dados celular 4G e enviar telemetrias/alertas para a nuvem utilizando o cliente MQTT embutido do próprio modem, sem o uso de pilha PPP.
* **Funcionamento:**
  1. Inicializa o modem por hardware via pulso no pino PWRKEY (GPIO4) e configura a comunicação serial a 115200 bps.
  2. Inicia uma tarefa em segundo plano (`simcom_uart_rx_task`) para escuta contínua de caracteres da serial e processamento de URCs de entrada (como `+CMQTTDELIVER` para mensagens de subscrição e `+CMQTTCONNLOST` para desconexões).
  3. Configura a APN de rede celular e executa a máquina de estados para registrar o modem na rede (através de comandos síncronos sincronizados por semáforos).
  4. Ativa o motor MQTT integrado do SIMCom (`AT+CMQTTSTART`), cria a sessão do cliente e conecta-se diretamente ao broker via comandos AT (`AT+CMQTTCONNECT`).
  5. Subscreve-se nos tópicos de comandos e de configuração do bastão e direciona os payloads recebidos ao parser de comandos JSON do sistema.
  6. Realiza o envio de telemetria e posições de satélite executando sequências AT de publicação (`AT+CMQTTTOPIC`, `AT+CMQTTPAYLOAD` e `AT+CMQTTPUB`).
  7. O modem SIMCom possui suporte a Dual SIM Single Standby (DSSS) gerenciado ativamente via comando proprietário `AT*SELECTSIMSLOT`, trocando de slot automaticamente em caso de falha de chip.
  8. O receptor GPS/GLONASS integrado no modem é ativado por hardware e consultado a cada 30 segundos diretamente via comandos AT síncronos (`AT+CGNSSINFO`), sem necessidade de suspender fluxos de dados.

### 2.5. Task Bluetooth (BLE) Mobile Connection
* **Objetivo:** Estabelecer uma conexão Bluetooth segura com um aplicativo de celular para envio bidirecional de dados de configuração e de negócios do bastão.
* **Funcionamento:**
  1. **Inicialização do GATT Server**: O ESP32 inicializa a pilha Bluetooth (BLE) e publica um serviço GATT dedicado com características de leitura e escrita seguras.
  2. **Canal de Segurança**: A conexão exige autenticação e pareamento criptografado para evitar interceptação e acesso não autorizado aos dados sensíveis do bastão.
  3. **Recepção e Sincronização de Dados**:
     * **Dados de Configuração**: Sincroniza parâmetros de hardware do bastão (ex: potência do YRM100, tempo de varredura).
     * **Dados de Negócio**: Recebe registros detalhados contendo o cadastro da **fazenda**, identificação do **lote** e perfis individuais do **animal** (ex: peso, idade, vacinas, histórico).
  4. **Armazenamento Local**: O ESP32 valida esses dados e os grava na memória flash não-volátil (NVS/SPIFFS), permitindo a associação e exibição local dessas informações conforme as tags RFID correspondentes são lidas.

### 2.6. Rotina de Atualização de Firmware Remota (OTA - Over-the-Air)
* **Objetivo:** Permitir a atualização segura do firmware do ESP32 Coordenador remotamente.
* **Escopo:** ESP32 Coordenador ✅ | K10 ❌ (apenas USB/serial) | Modem SIMCom ❌ (não implementado)
* **Funcionamento:**
  1. **Disparo do OTA**: A atualização é disparada via comando MQTT no tópico `id/{MAC}/cmd` com payload `{"cmd":"ota","url":"https://..."}`.
  2. **Restrição de Conectividade**:
     * **Wi-Fi apenas**: O componente `esp_https_ota` exige a pilha TCP/IP LwIP, disponível apenas quando o Wi-Fi está ativo (modos `WIFI_ONLY`, `WIFI_CELLULAR` ou `AUTO` com Wi-Fi conectado).
     * **Celular (4G)**: Não suportado — o modem SIMCom gerencia o TCP/IP internamente via comandos AT, sem interface IP local para o `esp_https_ota`.
  3. **Processo de Gravação Seguro (ESP HTTPS OTA)**:
     * O download é efetuado via HTTPS (`esp_https_ota`), com verificação de certificado SSL via `esp_crt_bundle_attach`.
     * A gravação ocorre na partição inativa (`ota_0` ←→ `ota_1`), preservando o firmware anterior para rollback.
     * Partições: `ota_0` (1728K) e `ota_1` (1728K) + `otadata` (8K).
  4. **Rollback de Segurança**:
     * No boot, `ota_manager_init()` chama `esp_ota_mark_app_valid_cancel_rollback()` para confirmar que o firmware é estável.
     * Se o ESP32 reiniciar 2x sem validar, o bootloader reverte para a partição anterior automaticamente.

### 2.7. Mecanismo de Cache e Armazenamento Offline (Spooler de Telemetria)
* **Objetivo:** Prevenir a perda de dados de leituras de tags e eventos de telemetria em áreas remotas sem cobertura de conectividade Wi-Fi ou celular (4G).
* **Funcionamento:**
  1. **Detecção de Estado de Conectividade**:
     * O sistema monitora constantemente o status da conexão à rede (Wi-Fi e o cliente MQTT embutido do celular) e o estado de envio do publicador MQTT.
     * Se as interfaces de rede estiverem desconectadas ou o Broker MQTT inacessível, o sistema direciona as mensagens para o modo **Offline**.
  2. **Persistência em Fila Local (Spooling)**:
     * No modo **Offline**, os JSONs criptografados da fila de dados são gravados no sistema de arquivos flash local (**LittleFS** ou **SPIFFS** do ESP32).
     * Os registros são estruturados em arquivos sequenciais simulando uma fila FIFO (First-In, First-Out).
  3. **Limite e Proteção de Memória**:
     * É configurada uma partição flash dedicada para cache. Caso a partição atinja 95% de uso, o sistema bloqueia novos registros e ativa uma sinalização visual/audível no bastão para evitar estouro de memória.
   4. **Recuperação e Sincronização Posterior**:
      * Quando a conectividade com o Broker MQTT é restabelecida, uma rotina de transmissão em background lê os arquivos da flash, publica os payloads antigos na nuvem e os apaga apenas após a confirmação de entrega (ACK MQTT) do broker.

---

## 3. Funcionalidades da Tela K10 (Display + BLE Mesh Node)

A Tela K10 é um dispositivo ESP32-S3 com display LVGL 480x480 que atua como nó BLE Mesh, exibindo dados recebidos do Coordenador e gerenciando interface local.

### 3.1. Speaker I2S (NS4168)
* **Objetivo:** Emitir sons de confirmação ao ler tags RFID.
* **Funcionamento:**
  1. Driver I2S configurado com `driver/i2s_std.h` (API nova ESP-IDF v5.4.4).
  2. Pinos: BCLK=GPIO0, LRCK=GPIO38, SDO=GPIO45 (MCLK não conectado).
  3. Geração de sine wave 44100Hz/16-bit/mono.
  4. Tipos de beep: `short` (100ms), `long` (300ms), `double` (2x short), `alert` (2kHz contínuo).
  5. Chamado no `main.c` a cada leitura RFID válida.

### 3.2. Tela "BRINCO LIDO"
* **Objetivo:** Exibir confirmação visual de leitura de tag.
* **Funcionamento:**
  1. Tela branca fullscreen com texto "BRINCO LIDO" + número da tag.
  2. Auto-dismiss após 3 segundos via `lv_timer_create()`.
  3. Texto "GPS SINCRONIZADO" exibido abaixo de lat/lon quando `fix=true`.

### 3.3. SPIFFS + Contador Diário
* **Objetivo:** Persistir leituras de tags em armazenamento local.
* **Funcionamento:**
  1. Componente `tag_database/` gerencia banco de tags diárias.
  2. Partição SPIFFS (2MB em 0x310000) com arquivos JSON por data.
  3. Formato: `/spiffs/tags/YYYY-MM-DD.json`.
  4. Buffer em RAM com auto-save a cada 10 reads.
  5. Cada registro contém: tag ID, nome animal, timestamp.

### 3.4. Aba Histórico (Tab2)
* **Objetivo:** Exibir últimas leituras na tela.
* **Funcionamento:**
  1. Header com ícone olho + total de leituras + tags únicas.
  2. Lista scrollável com últimas 20 tags (tag ID + nome animal).
  3. Atualizado a cada leitura via `gui_manager_refresh_history()`.

### 3.5. Exibição de Dados Recebidos via Mesh
* **Objetivo:** Mostrar dados do Coordenador em tempo real.
* **Funcionamento:**
  1. **GPS:** Coordenadas reais do Coordenador (verde) ou "GPS NAO SINCRONIZADO" (vermelho).
  2. **Bateria:** Percentual 0-100% via Mesh (não do STM32).
  3. **4G:** Barras de sinal mapeadas de RSSI (dBm).
  4. **RFID:** Ícone verde se STM32 ativo, vermelho se inativo.
  5. **Backlight:** Timeout 120s, wake por botão ou tag RFID.

### 3.6. GPS Gate (Publicação Condicional)
* **Objetivo:** Publicar MQTT somente com localização válida.
* **Funcionamento:**
  1. Dispatcher verifica `simcom_driver_has_location()`.
  2. **GPS fix:** publica com coordenadas GPS.
  3. **Cell tower fallback:** usa triangulação via Mozilla Location Service.
  4. **Sem localização:** tag enviada à K10 via Mesh (exibição), mas descartada do MQTT.


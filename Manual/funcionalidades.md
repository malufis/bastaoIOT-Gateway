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
* **Objetivo:** Estabelecer a conectividade de dados e enviar telemetria para a nuvem.
* **Funcionamento:**
  1. Inicializa a comunicação serial com o modem SIMCom 7663E.
  2. Executa a pilha PPP (Point-to-Point Protocol) para subir a interface IP de rede a nível de sistema operacional (ESP-NETIF).
  3. Inicializa o cliente MQTT nativo associado à interface PPP recém-criada.
  4. Conecta-se de forma segura (SSL/TLS) ao broker MQTT na nuvem.
  5. Consome dados da fila de envio celular e os publica nos tópicos dedicados do broker.
  6. Modulo SIM7663E gerencia a rede celular, enviando dados para o broker MQTT na nuvem e recebendo comandos de controle remoto.
  7. O modulo SIM7663E possui dois slot de chip, permitindo o uso de duas operadoras diferentes.
  8. O modulo SIM7663E possui GPS/GLONASS integrado, permitindo o rastreamento da localização do bastão.

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
* **Objetivo:** Permitir a atualização segura do firmware do ESP32 remotamente utilizando conexão Wi-Fi (quando disponível em base/curral) ou conexão celular 4G via modem SIMCom 7663E.
* **Funcionamento:**
  1. **Disparo do OTA**: A atualização pode ser disparada via comando MQTT/HTTPS contendo a URL do novo binário de firmware e a assinatura de segurança, ou por verificação periódica em um servidor seguro.
  2. **Seleção de Interface de Conectividade**:
     * **Wi-Fi**: Priorizada quando o bastão estiver na base carregadora ou curral com cobertura de rede Wi-Fi local configurada, poupando dados celulares.
     * **Celular 4G (SIMCom 7663E)**: Utilizada em campo por meio da interface de rede PPP ativa sob comando ou verificação programada.
  3. **Processo de Gravação Seguro (ESP HTTPS OTA)**:
     * O download é efetuado via HTTPS (`esp_https_ota`), exigindo verificação de certificado SSL e gravação alternada na partição inativa (`ota_0`/`ota_1`).
     * Após o download completo, o ESP32 realiza a validação de integridade (SHA-256) do binário.
  4. **Rollback de Segurança**:
     * Se o novo firmware falhar em inicializar ou perder conectividade com a rede de verificação, o gerenciador de boot (App Rollback) reverte automaticamente para a partição anterior estável.

### 2.7. Mecanismo de Cache e Armazenamento Offline (Spooler de Telemetria)
* **Objetivo:** Prevenir a perda de dados de leituras de tags e eventos de telemetria em áreas remotas sem cobertura de conectividade Wi-Fi ou celular (4G).
* **Funcionamento:**
  1. **Detecção de Estado de Conectividade**:
     * O sistema monitora constantemente o status da conexão à rede (Wi-Fi e PPP Celular) e o estado de conexão do cliente MQTT.
     * Se as interfaces de rede estiverem desconectadas ou o Broker MQTT inacessível, o sistema direciona as mensagens para o modo **Offline**.
  2. **Persistência em Fila Local (Spooling)**:
     * No modo **Offline**, os JSONs criptografados da fila de dados são gravados no sistema de arquivos flash local (**LittleFS** ou **SPIFFS** do ESP32).
     * Os registros são estruturados em arquivos sequenciais simulando uma fila FIFO (First-In, First-Out).
  3. **Limite e Proteção de Memória**:
     * É configurada uma partição flash dedicada para cache. Caso a partição atinja 95% de uso, o sistema bloqueia novos registros e ativa uma sinalização visual/audível no bastão para evitar estouro de memória.
  4. **Recuperação e Sincronização Posterior**:
     * Quando a conectividade com o Broker MQTT é restabelecida, uma rotina de transmissão em background lê os arquivos da flash, publica os payloads antigos na nuvem e os apaga apenas após a confirmação de entrega (ACK MQTT) do broker.


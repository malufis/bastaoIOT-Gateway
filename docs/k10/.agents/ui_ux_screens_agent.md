# Especialista em Telas e Fluxo (UI/UX Screens Agent)

**Objetivo**: Implementar e gerenciar a "Etapa 2" do projeto Bastão Unihker, focando na construção detalhada das 4 telas do sistema e na experiência de navegação por botões físicos.

## 1. Diretrizes de Hardware e Inicialização
- **LEDs do Sistema**: Todos os LEDs RGB/Status do K10 devem ser desligados por padrão para economia de energia e foco visual na tela.
- **Navegação Dinâmica**:
    - **Nível 1 (Abas)**: Botão Superior (A) -> Direita, Botão Inferior (B) -> Esquerda.
    - **Ação de Entrada**: Segurar Botão Superior (A) por 3 segundos para entrar nos dados/funcionalidades da tela.
    - **Nível 2 (Interno)**: Botão Superior (A) -> Subir, Botão Inferior (B) -> Descer.
    - **Seleção/Ação**: Segurar por 3 segundos para selecionar uma função dentro da tela.
    - **Saída**: Manter um elemento visual (ícone ou linha) para indicação de saída, acionando a técnica de 3 segundos para retornar ao Nível 1.

## 2. Estrutura Detalhada das Telas

### Tela 1: Inicial / Leituras (Proporção 1 - 5 - 2)
- **Linha Superior (Altura 1)**: 
    - Ícones alinhados à direita: [4G/3G] -> [WiFi] -> [RFID (UHF ou LF/RF)] -> [GPS/Glonass].
- **Área Central (Altura 5)**:
    - `Fazenda Raptor`
    - `Animal: Boi Bandido`
    - `Horário: 25/04/2026 as 07:15`
    - `Local: -20.444203671069168, -54.61944353080301`
- **Linha Inferior (Altura 2)**:
    - Dados do Inclinômetro (X, Y, Z).
    - Tensão da Bateria (V).

### Tela 2: Configuração da Conectividade (Lista)
- **4G / 3G**: Sub-menu com Apn e Operadora.
- **WiFi**: Nome do SSID e Estado da conexão.
- **RFID**: 
    - Ativar (UHF ou LR/RF).
    - Potência do UHF.
- **GPS / GLONASS**: Ativar GPS ou GLONASS.

### Tela 3: Configuração do Dispositivo (Lista)
- **Configuração do App**: Sincronização e Ativação de dispositivo.
- **Arquivos**: Quantidade de arquivos na memória e Memória livre.
- **Envio**: Botão/Opção para enviar arquivo por demanda.
- **Servidor**: Exibir estado atual do servidor.

### Tela 4: Informações do Dispositivo (Lista)
- **Firmware**: Versão do firmware atual.
- **Satélites**: Quantidade de satélites em vista.
- **Sinais**: Potência do sinal GPS/GLONASS, 4G/GSM e UHF.
- **Conexão**: ID/MAC do cliente conectado.

## 3. Padrões de Implementação
- **Agritech Theme**: Seguir o esquema "Modern Dark" com alto contraste.
- **Modularidade**: Cada tela deve ter sua própria função de criação (`gui_create_screen_X`).
- **NVS Persistence**: Configurações alteradas nas telas de Conectividade e Dispositivo devem ser salvas na memória Flash (NVS) para persistirem após o reboot.

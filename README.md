# Bastão de Identificação RFID (Bastão-ESP)

Este repositório contém o firmware e a documentação do projeto do **Bastão de Identificação RFID**, um dispositivo portátil voltado para o rastreio e leitura de tags de animais (LF) e tags UHF de longo alcance.

---

## 1. Visão Geral do Hardware

O bastão é operado por dois microcontroladores acoplados fisicamente para otimizar a coleta e a conectividade de dados:

* **Microcontrolador de Sensoriamento (STM32G070CBTx)**:
  * Gerencia os leitores RFID de forma integrada via canais UART.
  * Realiza a leitura e telemetria da bateria da placa.
* **Microcontrolador de Conectividade (ESP32)**:
  * Centraliza os dados recebidos via interface serial do STM32.
  * Transmite as informações coletadas para a **Tela K10** local via BLE Mesh.
  * Envia os dados criptografados para a nuvem utilizando conectividade celular.

---

## 2. Equipamentos e Placas Utilizadas

* **Leitor RFID LF (WL-134)**:
  * Frequência: 134.2 KHz (Protocolo FDX-B / ISO11784/5).
  * Interface: UART (9600 bps, 8N2).
* **Leitor RFID UHF (YRM100)**:
  * Frequência: UHF (EPC Class 1 Gen 2 / ISO18000-6C).
  * Interface: UART (115200 bps, 8N1).
* **Modem Celular**:
  * **SIMCom 7663E** (Interface PPP / UART a 115200 bps).
* **Tela Local**:
  * **Tela K10** (Equipada com receptor BLE Mesh).

---

## 3. Frameworks e Ferramentas de Desenvolvimento

* **Firmware STM32**:
  * Desenvolvido em linguagem **C** utilizando a biblioteca **STM32Cube HAL** integrada com a IDE **STM32CubeIDE**.
* **Firmware ESP32**:
  * Desenvolvido em linguagem **C** sobre o framework oficial **ESP-IDF v5.x**, operando sob o sistema de tempo real **FreeRTOS**.
* **Modelagem e Visualização**:
  * Diagramas UML e fluxos de dados gerados através da sintaxe **Mermaid**.

---

## 4. Estrutura do Repositório

* **`Manual/`**: Contém manuais técnicos sobre Git/versionamento, arquitetura física de hardware e funcionamento lógico do software.
* **`aprendizado/`**: Base de conhecimento consolidada sobre protocolos de baixo nível (WL-134 e YRM100) para uso por agentes inteligentes de IA.
* **`stm32_firmware/`**: Código-fonte do firmware do microcontrolador STM32G070CBTx.
* **`esp32_firmware/`**: Código-fonte do firmware do microcontrolador ESP32.
* **`teste_automatizado/`**: Scripts e arquivos para a execução de rotinas de testes.
* **`debug/`**: Arquivos de log e relatórios de análise de defeitos.

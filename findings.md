# Findings & Decisions: Estruturação do Bastão-ESP

## Requirements
- Estruturação do projeto com pastas dedicadas: `Manual/`, `aprendizado/`, `teste_automatizado/` e `debug/`.
- Documentação do Git e versionamento (`Manual/git_guidelines.md`).
- Documentação da estrutura do projeto e arquitetura de hardware (`Manual/arquitetura.md`).
- Documentação das funcionalidades e lógica de firmware do ESP32 e STM32 (`Manual/funcionalidades.md`).
- Base de conhecimento dos protocolos RFID para IAs (`aprendizado/rfid_protocols.md`).
- Atualização do perfil do agente de RFID (`.agents/rfid_agent.md`) com lógicas de protocolos.
- Criação de novo perfil de agente para o STM32 (`.agents/stm32_agent.md`) contendo a pinagem específica do STM32G070CBTx.
- Atualização do `README.md` raiz.

## Research Findings
- **Leitor YRM100:** Usa UART a 115200 8N1. Protocolo com frames iniciados por `0xBB` e encerrados por `0x7E`. Checksum calculado por soma acumulada (Type até Parameters) modulo 256.
- **Leitor WL-134:** Usa UART a 9600 8N2. Envia string ASCII de 30 bytes com início `0x02` e fim `0x03`. Tags decodificadas usando LSB-first invertido caractere a caractere em hexadecimal para número de cartão e país.
- **STM32G070CBTx Hardware:**
  - USART3 (PA5/PB0) -> WL-134 (9600 8N2).
  - USART4 (PA0/PA1) -> YRM100 (115200 8N1).
  - USART2 (PA2/PA3) -> Comunicação com ESP32 (115200 8N1) [Conectado a IO13-RX / IO14-TX do ESP32]. Referência: [esquematico_placa.pdf](file:///d:/git/Bastao/Bastão-ESP/Manual/esquematico_placa.pdf).
  - ADC1 (PB1/IN9) -> Divisor de bateria 100k/10k. Fator multiplicador 11.0.
  - PB4 -> WL-134 PWR, PB5 -> YRM100 PWR.

## Technical Decisions
| Decision | Rationale |
|----------|-----------|
| Divisão de diretórios Manual/ e aprendizado/ | Garante leitura limpa para programadores e eficiência de tokens para agentes de IA. |
| Conventional Commits + SemVer | Padronização profissional de histórico e controle de compatibilidade. |
| Buffers circulares UART + Interrupção no STM32 | Evita perda de bytes nos canais seriais paralelos assíncronos. |
| Cifragem AES-256-CBC com PKCS#7 | Compatível nativamente com o hardware do ESP32 (via mbedTLS) e segura para dados locais. |

## Issues Encountered
| Issue | Resolution |
|-------|------------|
| Tentativa de gravar artefato fora da pasta de cérebro | Mudança para escrita normal de arquivo markdown (`IsArtifact: false`) no workspace do usuário. |
| Falha ao carregar DLL da biblioteca `cryptography` do Python | Substituição do script de testes para utilizar a biblioteca robusta `pycryptodomex` já instalada. |

## Resources
- Manual do YRM100: [Communication user Protocol V2.1_en.docx](file:///d:/git/Bastao/Bastão-ESP/Communication%20user%20Protocol%20V2.1_en.docx)
- Manual do WL-134: [RFID reader module.pdf](file:///d:/git/Bastao/Bastão-ESP/RFID%20reader%20module.pdf)
- Script de Validação RFID: [validate_rfid.py](file:///d:/git/Bastao/Bastão-ESP/teste_automatizado/validate_rfid.py)
- Script de Validação Criptográfica: [verify_encryption.py](file:///d:/git/Bastao/Bastão-ESP/teste_automatizado/verify_encryption.py)


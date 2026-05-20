# Plan de Tarefas: Implantação e Estruturação do Bastão-ESP

> [!IMPORTANT]
> O plano de tarefas e o roadmap detalhado do projeto foram unificados no arquivo [ROADMAP.md](file:///d:/git/Bastao/Bast%C3%A3o-ESP/ROADMAP.md). Por favor, consulte este arquivo para acompanhar o andamento do projeto e as próximas fases de implementação.

## Goal
Estruturar o projeto Bastão-ESP, criando a estrutura física de diretórios (Manual, teste_automatizado, debug, aprendizado), gerando as diretrizes de versionamento Git, documentações técnicas de arquitetura e funcionalidades, base de conhecimento de protocolos RFID e configuração dos perfis de agentes (rfid_agent e stm32_agent).

## Current Phase
Transicionado para o [ROADMAP.md](file:///d:/git/Bastao/Bast%C3%A3o-ESP/ROADMAP.md)

## Phases

### Phase 1: Estruturação Física e Diretórios
- [x] Criar a pasta `Manual/`
- [x] Criar a pasta `teste_automatizado/` com `.gitkeep`
- [x] Criar a pasta `debug/` com `.gitkeep`
- [x] Criar a pasta `aprendizado/`
- **Status:** complete

### Phase 2: Documentação de Manuais Humanos
- [x] Criar `Manual/git_guidelines.md` com as regras de Git e versionamento
- [x] Criar `Manual/arquitetura.md` com o mapeamento físico de pinos e fluxos de dados
- [x] Criar `Manual/funcionalidades.md` com as descrições de lógicas de código do STM32 e ESP32
- **Status:** complete

### Phase 3: Base de Conhecimento de Protocolos (aprendizado)
- [x] Criar `aprendizado/rfid_protocols.md` compilando a especificação detalhada de decodificação do YRM100 e WL-134 para IAs
- **Status:** complete

### Phase 4: Atualização e Criação dos Perfis de Agente
- [x] Atualizar `.agents/rfid_agent.md` com os detalhes matemáticos e decodificações
- [x] Criar `.agents/stm32_agent.md` configurado para a MCU STM32G070CBTx e suas conexões
- **Status:** complete

### Phase 5: Atualização do README Geral
- [x] Atualizar o `README.md` raiz com uma visão geral direta do hardware e frameworks do projeto
- **Status:** complete

### Phase 6: Comunicação Bluetooth no ESP32
- [x] Adicionar detalhes de comunicação Bluetooth com app de celular em `Manual/funcionalidades.md`
- **Status:** complete

### Phase 7: Atualização Remota (OTA) no ESP32
- [x] Adicionar detalhes de atualização remota (OTA) via Wi-Fi e 4G em `Manual/funcionalidades.md`
- **Status:** complete

### Phase 8: Sistema de Cache de Dados Offline
- [x] Adicionar detalhes de armazenamento offline e sincronização posterior em `Manual/funcionalidades.md`
- **Status:** complete

## Key Questions
1. Como o STM32 envia a leitura de bateria? (JSON via UART2)
2. Qual a pinagem exata e a configuração serial de cada leitor? (YRM100: USART4 PA0/PA1 115200 8N1; WL-134: USART3 PA5/PB0 9600 8N2)

## Decisions Made
| Decision | Rationale |
|----------|-----------|
| Abordagem 1 | Separação física clara entre documentação humana (Manual/) e base de conhecimento para agentes (aprendizado/) |

## Errors Encountered
| Error | Attempt | Resolution |
|-------|---------|------------|
|       | 1       |            |

## Notes
- Atualizar o status de cada fase conforme avançar.
- Registrar erros no log de erros.

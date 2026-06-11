---
name: project-management
description: Diretrizes de gerenciamento de projeto, planejamento e roadmap para o Sistema Bastao-ESP.
---

# Project Management Skill — Sistema Bastao

Esta skill rege o planejamento, gerenciamento e documentacao do projeto Bastao-ESP, incluindo roadmap, registro de progresso e gestao de tarefas.

---

## 1. Documentos de Gerenciamento

| Documento | Descricao |
|-----------|-----------|
| `ROADMAP.md` | Plano de trabalho definitivo: estado atual, fases concluidas, proximos passos |
| `task_plan.md` | Plano de tarefas detalhado com checkboxes de conclusao |
| `progress.md` | Registro cronologico de sessoes de desenvolvimento |
| `findings.md` | Descobertas tecnicas e decisoes durante o desenvolvimento |
| `PROJETO_BASTAO.md` | Visao geral do projeto, arquitetura, hardware, protocolos |

## 2. Estrutura de Fases

Cada fase no ROADMAP deve conter:

```markdown
### **Fase N: Nome da Fase** - **Status**

- **Objetivo:** Descricao clara do objetivo.
- **Status:** Concluido / Em andamento / Planejado.
- **Tarefas Realizadas:**
  - Modulo X: descricao da implementacao.
  - Modulo Y: descricao da integracao.
  - Script de teste: descricao da validacao.
```

## 3. Registro de Progresso

Cada sessao em `progress.md` deve conter:

```markdown
## Session: YYYY-MM-DD

### Phase N: Nome da Fase
- **Status:** complete / in-progress
- **Started:** YYYY-MM-DDTHH:MM:SS
- **Actions taken:**
  - Descricao das acoes realizadas
- **Files created/modified:**
  - `caminho/do/arquivo` (criado/modificado)
  - `progress.md` (modificado)
```

## 4. Diretrizes de Planejamento

1. **Priorizacao:** Funcionalidades essenciais primeiro (RFID, conectividade), depois otimizacoes (sleep, cache)
2. **Testes:** Cada fase deve incluir script de teste para validacao
3. **Documentacao:** Atualizar ROADMAP.md ao concluir cada fase
4. **Dependencias:** Respeitar ordem de dependencias (ex: STM32 RFID antes de ESP32 MQTT)
5. **Versoes:** Manter registro de versoes no ROADMAP.md com datas de release
6. **AGENTS.md:** Registrar sessoes de desenvolvimento com problemas resolvidos, licoes aprendidas e modificacoes

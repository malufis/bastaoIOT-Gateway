# Documenter Agent — Sistema Bastao

Este agente é acionado automaticamente para gerenciar, auditar e expandir a documentação técnica do **Sistema Bastao** (Sistema de Gerenciamento de Bastões RFID e Rastreabilidade Pecuária), incluindo docstrings de código, diário de desenvolvimento (`AGENTS.md`), README, Roadmap e boas práticas de versionamento Git.

---

## 1. Diretrizes de Documentação de Código (Docstrings)

Toda nova função, método ou classe deve ser documentado em **Português do Brasil (PT-BR)**.

### 1.1. Python (Padrão Google Style)

**Backend FastAPI:**
```python
def descriptografar_frame(payload_hex: str, chave_aes: bytes) -> dict:
    """Descriptografa um frame hex recebido do bastão RFID via AES-256-CBC.

    Args:
        payload_hex: Payload hexadecimal recebido do tópico MQTT.
        chave_aes: Chave simétrica de 32 bytes do bastão.

    Returns:
        Dict com os campos decodificados (brinco_id, latitude, longitude,
        bateria_nivel, timestamp_rtc).

    Raises:
        ValueError: Se o payload for inválido ou o checksum falhar.
    """
    ...
```

**Agentes de Background:**
```python
class AgentConectividade(BaseAgent):
    """Monitora o heartbeat dos bastões e atualiza status de conexão.

    Varre os bastões registrados a cada 30s calculando o tempo desde
    o último keepalive. Dispara alertas com cooldown de 10min.
    """
    ...
```

### 1.2. TypeScript / React (Padrão TSDoc)

**Componentes:**
```typescript
/**
 * Card com efeito glassmorphism para exibição de indicadores.
 *
 * @param titulo - Título exibido no cabeçalho do card.
 * @param icone - Componente lucide-react para o ícone.
 * @param children - Conteúdo interno do card.
 */
export function StatCard({ titulo, icone, children }: StatCardProps) {
  ...
}
```

**Contextos e Hooks:**
```typescript
/**
 * Hook de autenticação do Sistema Bastao.
 * Gerencia sessão via sessionStorage('bastao_auth') e token JWT.
 *
 * @returns {AuthContextType} Objeto com usuario, token, metodos login/logout.
 */
export function useAuth(): AuthContextType {
  ...
}
```

---

## 2. Estrutura de Documentação do Projeto

### 2.1. `AGENTS.md` (Raiz do Projeto)

Registra cronologicamente as sessões de desenvolvimento. Cada sessão deve conter:

1. **Cabeçalho:** `## Sessão XX — Título` + Data + Objetivo
2. **Problemas Resolvidos:** Lista de issues com contexto técnico
3. **Lições Aprendidas:** Decisões arquiteturais e justificativas
4. **Modificações Efetuadas:** Agrupadas por componente (Backend, Frontend, Mobile, Docker, Docs)

Formato do cabeçalho:
```markdown
## Sessão XX — Título Descritivo
**Data:** YYYY-MM-DD
**Objetivo:** Resumo em uma frase do que foi feito.
```

### 2.2. `documentacao/` (Arquivos Técnicos)

| Arquivo | Conteúdo |
|---------|----------|
| `00_Plano_de_Implementacao.md` | Visão geral, stack, fases, glossário |
| `01_Arquitetura_do_Sistema.md` | Fluxo de dados, diagrama, microsserviços |
| `02_Infraestrutura_e_Docker.md` | Redes, containers, portas, variáveis de ambiente |
| `03_Multi_Tenancy_e_Seguranca.md` | Isolamento SaaS, RBAC, triggers de auditoria |
| `04_Agentes_e_Desenvolvimento.md` | Agentes de background (conectividade, BMS, SIM) |
| `08_Roadmap.md` | Fases, cronograma Gantt, entregas |

### 2.3. `README.md`

Deve conter apenas:
- Visão geral do sistema (1 parágrafo)
- Stack tecnológica principal
- Instruções rápidas de desenvolvimento local
- Link para `documentacao/` para detalhes

### 2.4. `documentacao/08_Roadmap.md`

Contém:
- Especificação detalhada de recursos concluídos
- Recursos em andamento
- Planejamento de fases futuras com cronograma (formato Mermaid Gantt)

---

## 3. Diretrizes de Versionamento Git

### 3.1. Branch Principal

- `master` — branch de produção estável
- Desenvolvimento em branches de funcionalidade: `feature/*`
- Correções: `fix/*`
- Merge em `master` apenas com código testado

### 3.2. Mensagens de Commit (Conventional Commits)

Formato: `<tipo>: <descrição em português>`

Tipos:
- `feat`: Nova funcionalidade para o usuário final.
- `fix`: Correção de bug.
- `docs`: Alterações exclusivas em documentação ou comentários.
- `style`: Formatação de código (sem alteração lógica).
- `refactor`: Mudanças estruturais que não corrigem bugs nem adicionam features.
- `test`: Criação ou ajuste de testes.
- `chore`: Build, dependências, infraestrutura.

Exemplos:
```bash
git commit -m "feat: adiciona endpoint de sincronizacao com o SISBOV"
git commit -m "docs: documenta agente de monitoramento de bateria BMS"
git commit -m "fix: corrige timeout na descriptografia de frames AES"
```

### 3.3. Histórico de Commits

- Commits atômicos (uma alteração lógica por commit)
- Mensagens descritivas no corpo quando necessário
- Commits de documentação com prefixo `docs:`

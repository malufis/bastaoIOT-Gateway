# Sistema Bastao Agent — Backend & Integracao

Agente especializado no sistema backend **sistemaBastao** (`https://github.com/malufis/sistemaBastao`), plataforma de rastreabilidade pecuaria que recebe, processa e armazena dados coletados pelos dispositivos Bastao-ESP.

## Responsabilidades

1. **API Gateway FastAPI** (`src/api/`) — 16 roteadores REST com autenticacao JWT + RBAC + multi-tenant
2. **Receptor MQTT Daemon** (`src/container_receptor/`) — Subscriber MQTT, descriptografia AES, persistencia
3. **Agentes de Background** (`src/agentes/`) — 5 threads: bateria, conectividade, SISBOV, SICAR, GTA
4. **Banco de Dados PostgreSQL + PostGIS** — 11 migracoes SQL, triggers de auditoria, blockchain hash
5. **Seguranca e Criptografia** (`src/lib/seguranca/`) — AES-256-CBC (compativel com Bastao-ESP), JWT, argon2id
6. **Infraestrutura Docker** — 8 servicos orquestrados (api, receptor, agentes, mqtt, db, web, sicar, extrator)
7. **Importador GERBOV** (`src/extrator/`) — Parser de planilhas Excel com fuzzy column mapping
8. **Workers** (`src/worker_sicar/`) — Automacao Playwright + 2Captcha para consulta SICAR

## Arquitetura do Sistema

```
┌───────────────────────────────────────────────────────────────────┐
│                      sistemaBastao (Docker)                       │
├─────────────────┬──────────────────┬──────────────────────────────┤
│   API Gateway   │   Receptor MQTT  │   Agentes (5)               │
│   FastAPI:8000   │   (Daemon)       │   - Bateria                 │
│   - 16 routers  │   - Subscribe    │   - Conectividade           │
│   - JWT + RBAC  │   - Decrypt AES  │   - SISBOV                  │
│   - Multi-tenant│   - Persist DB   │   - SICAR                   │
│                 │                  │   - GTA                     │
├─────────────────┴──────────────────┴──────────────────────────────┤
│                        PostgreSQL 15 + PostGIS                     │
│   - fazendas, usuarios, bastoes, animais, coletas, produtores     │
│   - logs_auditoria, mqtt_usuarios, sim_cards, guias_gta           │
│   - fazendas_geometria (PostGIS GIST), brincos_estoque            │
│   - triggers auditoria + blockchain hash (coletas)                │
├─────────────────┬──────────────────┬──────────────────────────────┤
│   EMQX Broker   │   Frontend Web   │   App Mobile                │
│   mqtt:1883     │   React 19       │   Expo + SQLite             │
│   1883/8083     │   Vite 8         │   Leituras offline          │
│                 │   Leaflet mapas  │   Sincronizacao batch       │
│                 │   17 paginas     │                            │
└─────────────────┴──────────────────┴──────────────────────────────┘
```

## Fluxo de Dados (Bastao-ESP → Sistema)

```
Bastao-ESP → MQTT (agro/bastao/{serial}/telemetry)
              → EMQX Broker (autentica via mqtt_usuarios)
                → Receptor Daemon (ThreadPoolExecutor 4 workers)
                  → 1. Extrai serial do topico
                  → 2. Busca chave_aes do bastao no banco
                  → 3. Salva payload RAW em coletas_raw
                  → 4. Descriptografa AES-256-CBC (crypto.py)
                  → 5. Busca ultimo hash_registro (blockchain)
                  → 6. Calcula novo hash SHA-256
                  → 7. Insere em coletas (brinco_id, lat, lon, bateria, hash)
                  → 8. Atualiza bastao (bateria_nivel, ultimo_keepalive, status='online')
```

## Componentes Principais

### API Gateway (FastAPI) — `src/api/main.py`
- 16 roteadores: auth, usuarios, fazendas, produtores, animais, bastoes, bovinos, coletas, sim_cards, papeis, auditoria, sicar, gta (+ mocks)
- 4 middlewares: autenticacao (JWT), isolamento (tenant), seguranca (headers + rate limit), tenant (filtro hierarquico)
- 4 papeis RBAC: integrador (super), gestor, suporte, operador_campo

### Receptor MQTT — `src/container_receptor/main.py`
- Subscribe `agro/bastao/+/telemetry`
- Descriptografia AES-256-CBC com chave por bastao
- Blockchain hash SHA-256 encadeado por bastao
- ThreadPoolExecutor com ate 4 workers

### Agentes Background — `src/agentes/`
| Agente | Funcao | Trigger |
|--------|--------|---------|
| `agent_bateria.py` | Monitora bastoes com bateria baixa | A cada 30s |
| `agent_conectividade.py` | Detecta bastoes offline (sem heartbeat) | A cada 30s |
| `agent_sisbov.py` | Sincroniza dados com SISBOV | A cada 1h |
| `agent_sicar.py` | Valida CAR automaticamente | A cada 24h |
| `agent_gta.py` | Pipeline de envio de GTA para certificadora | A cada 5min |

### Database — `src/lib/banco/migrations/`
- 11 migracoes SQL progressivas
- Triggers de auditoria em todas as tabelas principais
- PostGIS + GIST indexes para dados geograficos
- Blockchain hash em coletas para integridade

### Seguranca — `src/lib/seguranca/`
- `crypto.py`: AES-256-CBC (encrypt/decrypt) + sha256 blockchain hash
- `hash.py`: argon2id + bcrypt para senhas
- `jwt.py`: Tokens JWT HS256 expiraveis (8h padrao)

## Skills Relacionadas

- `security-crypto` — Compatibilidade AES-256-CBC com Bastao-ESP
- `esp32-connectivity` — Integracao MQTT com dispositivos IoT
- `testing` — Testes de pipeline de dados

## Diretrizes de Desenvolvimento

1. **Compatibilidade:** AES-256-CBC deve usar mesma chave/IV que `secure_payload.c` do Bastao-ESP
2. **Formato MQTT:** Topico `agro/bastao/{serial}/telemetry`, payload hex (IV + ciphertext)
3. **Blockchain:** Toda coleta deve ter hash encadeado ao registro anterior do mesmo bastao
4. **Multi-tenant:** Headers `x-cliente-id` (produtor) e `x-empresa-id` (fazenda) em toda request
5. **Auditoria:** Toda alteracao em tabelas principais deve ser logada via trigger
6. **Migracoes:** Nunca alterar migracoes existentes; criar nova migracao incremental
7. **RBAC:** Usar decorator `@RequererPermissao(modulo, acao)` em todos os endpoints
8. **Docker:** Manter compatibilidade com docker-compose de 8 servicos

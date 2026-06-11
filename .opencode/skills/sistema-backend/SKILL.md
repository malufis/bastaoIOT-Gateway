---
name: sistema-backend
description: Diretrizes para desenvolvimento do backend Python do sistemaBastao, incluindo FastAPI, receptor MQTT, agentes de background, PostgreSQL + PostGIS e Docker.
---

# Sistema Bastao Backend Skill

Esta skill rege o desenvolvimento do backend da plataforma **sistemaBastao**, composta por API Gateway FastAPI, Receptor MQTT Daemon, Agentes de Background e banco de dados PostgreSQL + PostGIS.

---

## 1. Arquitetura Backend

```
src/
├── api/                    # FastAPI REST Gateway
│   ├── main.py            # App principal + middlewares + routers
│   ├── routers/           # 16 roteadores REST
│   ├── middlewares/       # 4 middlewares (auth, isolation, security, tenant)
│   └── schemas/           # 8 schemas Pydantic
├── agentes/               # 5 agentes de background (Thread)
│   ├── orquestrador.py    # Entry point dos agentes
│   ├── agent_bateria.py
│   ├── agent_conectividade.py
│   ├── agent_sisbov.py
│   ├── agent_sicar.py
│   └── agent_gta.py
├── container_receptor/    # MQTT subscriber + decrypt + persist
│   └── main.py
├── extrator/              # Importador de planilhas GERBOV
│   └── importar_cadastro.py
├── lib/
│   ├── banco/             # Pool de conexoes + migracoes SQL
│   │   ├── conexao.py     # ThreadedConnectionPool
│   │   └── migrations/    # 11 migracoes SQL
│   └── seguranca/         # Crypto, hash, JWT
│       ├── crypto.py      # AES-256-CBC + blockchain hash
│       ├── hash.py        # argon2id + bcrypt
│       └── jwt.py         # JWT tokens
└── worker_sicar/          # Playwright automacao SICAR
    └── worker_sicar.py
```

## 2. API Gateway (FastAPI)

### 2.1. Padrao de Roteadores
```python
from fastapi import APIRouter, Depends
from src.api.middlewares.autenticacao import RequererPermissao, obter_usuario_atual

router = APIRouter(prefix="/animais", tags=["Animais"])

@router.get("/")
def listar_animais(
    usuario: dict = Depends(obter_usuario_atual),
    _: bool = Depends(RequererPermissao("animais", "ler"))
):
    ...
```

### 2.2. Middleware Stack
1. `seguranca.py` — Security headers (CSP, HSTS) + rate limiting (100 req/min)
2. `autenticacao.py` — JWT validation + RBAC decorator
3. `isolamento.py` — ContextVars para tenant isolation
4. `tenant.py` — Filtro hierarquico Produtor > Fazenda

### 2.3. Resposta Padrao
```python
{"data": ..., "total": N}  # Listas
{"data": ...}              # Objetos
{"detail": "mensagem"}     # Erros
```

## 3. Receptor MQTT (container_receptor)

### 3.1. Pipeline de Processamento
```python
def processar_mensagem(payload: str, topico: str):
    serial = extrair_serial(topico)       # agro/bastao/{serial}/telemetry
    chave = buscar_chave_aes(serial)      # Busca no banco
    salvar_raw(payload, serial)           # coletas_raw
    dados = descriptografar(payload, chave)  # AES-256-CBC
    hash_anterior = buscar_ultimo_hash(serial)  # Blockchain
    novo_hash = calcular_hash(dados, hash_anterior)
    inserir_coleta(dados, hash_anterior, novo_hash)  # coletas
    atualizar_bastao(serial, dados)       # bastoes
```

### 3.2. Formato do Payload MQTT
```
hex_encode( IV(16 bytes) + AES_256_CBC( PKCS7_PAD( json ) ) )
```

## 4. Banco de Dados

### 4.1. Pool de Conexoes
```python
from src.lib.banco.conexao import get_conexao, usar_conexao

with usar_conexao() as conn:
    with conn.cursor() as cur:
        cur.execute("SELECT ...", (params,))
        return cur.fetchall()
```

### 4.2. Migracoes SQL
- Nunca alterar migracoes existentes
- Criar nova migracao incremental (ex: `10_nova_feature.sql`)
- Usar `IF NOT EXISTS` / `IF EXISTS` para idempotencia

### 4.3. Triggers de Auditoria
```sql
CREATE OR REPLACE FUNCTION trigger_auditoria()
RETURNS TRIGGER AS $$
BEGIN
    INSERT INTO logs_auditoria (tabela, operacao, registro_id, usuario_id, dados_antigos, dados_novos)
    VALUES (TG_TABLE_NAME, TG_OP, COALESCE(NEW.id, OLD.id),
            NULLIF(current_setting('app.current_user_id', true), '')::INTEGER,
            row_to_json(OLD), row_to_json(NEW));
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;
```

## 5. Seguranca

### 5.1. AES-256-CBC (crypto.py)
- `descriptografar_frame(payload_hex, chave_aes)` → dict
- `criptografar_frame(dados, chave_aes)` → hex string
- IV aleatorio de 16 bytes, prefixado ao ciphertext
- PKCS#7 padding

### 5.2. Blockchain Hash
- `calcular_hash_coleta(dados, hash_anterior)` → sha256 hex
- Encadeamento: cada coleta contem hash do registro anterior do mesmo bastao

### 5.3. JWT
- Algoritmo: HS256
- Payload: `{usuario_id, papel_id, exp, iat}`
- Validade: 8h (configuravel via `JWT_EXPIRY_HOURS`)
- Session token: `secrets.token_urlsafe(32)` para revogacao de logon unico

## 6. Docker

### 6.1. Servicos
| Servico | Dockerfile | Porta | Rede |
|---------|-----------|-------|------|
| db | postgis/postgis:15-3.4 | 5432 | dados |
| mqtt | emqx/emqx:5.7.1 | 1883, 8083 | iot, publica |
| api | Dockerfile.api | 8000 | dados, publica |
| receptor | Dockerfile.receptor | - | dados, iot |
| agentes | Dockerfile.agentes | - | dados, iot |
| web | Dockerfile.web (multi-stage) | 80 | publica |
| sicar_worker | Dockerfile.sicar | - | dados |
| extrator | Dockerfile.extrator | - | dados |

### 6.2. Variaveis de Ambiente
51 variaveis em `.env.example`, categorizadas por servico:
- `DB_*` — Conexao PostgreSQL
- `MQTT_*` — Conexao EMQX
- `JWT_*` — Configuracao de tokens
- `SISBOV_*` — API SISBOV
- `SICAR_*` — API SICAR + 2Captcha
- `GTA_*` — API certificadora
- `AGENTES_*` — Intervalos dos agentes

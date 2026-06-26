# Agentes e Desenvolvimento

## Agents Disponiveis (OpenCode)

| Agent | Quando Usar | Automacao |
|-------|-------------|-----------|
| `explore` | Busca rapida de arquivos, grep, glob patterns | Automatico |
| `general` | Tarefas multi-step, pesquisa complexa | Automatico |
| `esp32-firmware-agent` | Codigo firmware ESP32 (ESP-IDF v5.4) | Manual |
| `k10-firmware-agent` | Firmware da Tela K10 (LVGL + BLE Mesh Node) | Manual |
| `stm32-firmware-agent` | Firmware STM32 (RFID, ADC, UART) | Manual |
| `sistema-bastao-agent` | Backend Python (FastAPI, MQTT, PostgreSQL) | Manual |
| `frontend-mobile-agent` | Frontend React SPA + App Mobile Expo | Manual |
| `testing-agent` | Testes automatizados (Python, pytest) | Manual |
| `reviewer-agent` | Code review e verificacao de padroes | Manual |
| `documenter` | Geracao de documentacao (Doxygen, README) | Manual |
| `architect-agent` | Arquitetura de sistema, design decisions | Manual |

## Skills Disponiveis

| Skill | Escopo | Arquivos |
|-------|--------|----------|
| `c-best-practices` | Padroes C (Doxygen, naming, seguranca) | Todos .c/.h |
| `esp32-connectivity` | BLE Mesh, UART, MQTT, OTA, Cache | `esp32_firmware/` |
| `esp32-idf-agent` | Inicializacao ESP-IDF, NVS, FreeRTOS | `esp32_firmware/` |
| `k10-firmware` | LVGL, display, acelerometro, mesh node | `k10_firmware/` |
| `stm32-firmware` | RFID, ADC bateria, UART, non-blocking | `stm32_firmware/` |
| `security-crypto` | AES-256-CBC, mbedTLS, PKCS#7 | `secure_payload.*` |
| `sistema-backend` | FastAPI, MQTT receptor, Docker | Backend Python |
| `frontend-react` | React SPA, Expo, RBAC, Leaflet | Frontend |
| `testing` | Scripts Python, pytest, CI/CD | `teste_automatizado/` |
| `project-management` | ROADMAP, progress.md, AGENTS.md | Docs |
| `simcom-7663e` | AT commands, GPS, PPP | `simcom_driver.*` |

## Padroes de Codigo

### C (ESP32 e STM32)
- Doxygen em toda funcao publica
- Header guards obrigatorios
- Nomes em ingles (variaveis, funcoes) comecando com prefixo do modulo
- Comentarios em portugues para logica de negocio
- ``esp_err_t`` para retorno de erros (ESP32)

### Python (Backend)
- PEP 8
- FastAPI com type hints
- Alembic para migracoes de banco
- Pytest para testes

### TypeScript (Frontend)
- TSDoc para componentes e hooks
- Nomes de componentes em PascalCase
- Context API para estado global

## Fluxo de Trabalho Recomendado

1. **Entender o problema** -> carregar skill mais relevante
2. **Explorar codigo** -> agent ``explore``
3. **Implementar** -> seguir padroes da skill
4. **Verificar** -> lint, build, testes
5. **Documentar** -> AGENTS.md (sessao) + comentarios Doxygen

## Versionamento Git

### Branchs
- ``master`` — Producao estavel
- ``feature/*`` — Novas funcionalidades
- ``fix/*`` — Correcoes de bugs

### Mensagens de Commit (Conventional Commits)
```
feat: adiciona endpoint de sincronizacao com o SISBOV
fix: corrige timeout na descriptografia de frames AES
docs: documenta agente de monitoramento de bateria BMS
refactor: reorganiza inicializacao do modem SIMCom
test: adiciona teste de integracao para MQTT offline
chore: atualiza dependencias do ESP-IDF para v5.4.4
```

### Regras
- Commits atomicos (uma alteracao logica por commit)
- Mensagens descritivas em portugues
- Prefixo ``docs:`` para documentacao
- Merge em ``master`` apenas com codigo testado

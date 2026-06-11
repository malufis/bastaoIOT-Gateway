# Testing Agent — Sistema Bastao

Agente especializado em testes automatizados e validacao de firmware para o Sistema Bastao-ESP.

## Responsabilidades

1. **Desenvolvimento e manutencao de scripts de teste em Python**
2. **Validacao de protocolos RFID:** YRM100 (UHF) e WL-134 (LF)
3. **Testes de criptografia:** AES-256-CBC com PKCS#7
4. **Testes de conectividade:** MQTT, BLE GATT, OTA, Wi-Fi/Celular
5. **Testes de resiliencia:** Cache offline FIFO, redundancia de rede
6. **Testes de negocio:** Enriquecimento de dados (animal_db)
7. **Testes de estresse:** Mesh flood, packet loss, reconexao, latencia
8. **Testes de energia:** Modos sleep, consumo

## Scripts de Teste

| Script | Descricao |
|--------|-----------|
| `teste_automatizado/validate_rfid.py` | Validacao de decodificacao YRM100 e WL-134 |
| `teste_automatizado/verify_encryption.py` | Criptografia AES-256-CBC |
| `teste_automatizado/verify_ble_gatt.py` | Caracteristicas e pareamento GATT |
| `teste_automatizado/verify_mqtt_payload.py` | Pipeline MQTT completo |
| `teste_automatizado/verify_offline_cache.py` | FIFO cache SPIFFS |
| `teste_automatizado/verify_ota.py` | Comando OTA via MQTT |
| `teste_automatizado/verify_redundancy.py` | Chaveamento Wi-Fi/Celular |
| `teste_automatizado/verify_business_enrichment.py` | Lookup de animal no banco local |
| `teste_automatizado/verify_network_config.py` | Configuracao de rede via app |
| `teste_automatizado/verify_sleep_modes.py` | Modos de baixo consumo |
| `teste_automatizado/verify_mesh_stress.py` | Estresse da rede Mesh |

## Skills Relacionadas

- `testing` — Diretrizes de testes, estrutura dos scripts
- `c-best-practices` — Validacao de codigo C
- `esp32-connectivity` — Contexto de rede para testes

## Diretrizes de Desenvolvimento

1. Todo script deve usar a classe `TestResult` com metodos `run()` e `print_result()`
2. Saida formatada: `✓ PASS` ou `✗ FAIL` com nome do teste
3. Testes devem ser independentes e reproduziveis
4. Constantes de configuracao no topo do script (tags, chaves)
5. Documentar cada teste com docstring explicando o cenario
6. `sys.exit(1)` se algum teste falhar (para integracao CI/CD)
7. Usar `argparse` sempre que possivel para parametrizacao
8. Manter cobertura de testes para novas funcionalidades

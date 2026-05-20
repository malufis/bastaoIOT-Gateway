# Automated Testing Agent

Este agente é responsável por desenvolver, manter e executar rotinas de testes automatizados para validação do firmware STM32 e ESP32, garantindo a integridade do sistema antes de cada release.

---

## 1. Escopo de Testes

### 1.1 Testes de Validação de Protocolos RFID

Scripts Python para validação de decodificação de tags:

| Script | Descrição | Entrada | Saída Esperada |
|--------|-----------|---------|----------------|
| `validate_rfid.py` | Validação de frames YRM100 e WL-134 | Frames brutos hex | EPCs decodificados em decimal/hex |
| `verify_lsb_conversion.py` | Teste da lógica LSB-first do WL-134 | Strings hex brutas | Código do país e cartão em decimal |

### 1.2 Testes de Criptografia e Segurança

| Script | Descrição | Entrada | Saída Esperada |
|--------|-----------|---------|----------------|
| `verify_encryption.py` | Validação AES-256-CBC com PKCS#7 | JSON plaintext | Payload cifrado hex, decifrável com chave |

### 1.3 Testes de Conectividade

| Script | Descrição | Validação |
|--------|-----------|-----------|
| `verify_mqtt_payload.py` | Pipeline MQTT completo | Payload RFID + battery + GPS → MQTT broker |
| `verify_ble_gatt.py` | Características e pareamento GATT | UUIDs únicos, payload < 512 bytes |
| `verify_ota.py` | Comando OTA via MQTT | Extração de URL e trigger de download |

### 1.4 Testes de Resiliência e Armazenamento

| Script | Descrição | Validação |
|--------|-----------|-----------|
| `verify_offline_cache.py` | FIFO cache SPIFFS | Ordenação FIFO, transmissão após reconexão |
| `verify_redundancy.py` | Chaveamento Wi-Fi/Celular | Suspensão automática, reativação via watchdog |
| `verify_business_enrichment.py` | Lookup de animal no banco local | JSON enriquecido com nome, peso, lote |

---

## 2. Estrutura dos Scripts de Teste

### 2.1 Padrão de Arquitetura

```python
#!/usr/bin/env python3
"""
Script de validação: [Nome do Teste]
Descrição breve do que é testado.
"""
import sys
from Crypto.Cipher import AES
import json

# Constantes de configuração
TEST_TAG_UHF = "30751FEB705C5904E3D50D70"
TEST_TAG_LF = "900250000023921"
ENCRYPTION_KEY = bytes.fromhex("0123456789ABCDEF...")

class TestResult:
    def __init__(self, name: str):
        self.name = name
        self.passed = False
        self.expected = None
        self.actual = None
        self.error = None

    def run(self) -> bool:
        """Executa o teste e retorna status."""
        raise NotImplementedError

    def print_result(self):
        status = "✓ PASS" if self.passed else "✗ FAIL"
        print(f"{status} | {self.name}")
        if not self.passed:
            print(f"  Expected: {self.expected}")
            print(f"  Actual:   {self.actual}")
            if self.error:
                print(f"  Error:    {self.error}")

def main():
    tests = [
        TestUHFDecoding(),
        TestLFConversion(),
        TestEncryption()
    ]

    passed = 0
    for test in tests:
        if test.run():
            passed += 1
        test.print_result()

    print(f"\n--- Results: {passed}/{len(tests)} ---")
    return 0 if passed == len(tests) else 1

if __name__ == "__main__":
    sys.exit(main())
```

### 2.2 Requisitos de Dependências

Todas as dependências devem ser documentadas:
```txt
# requirements.txt
pycryptodomex>=3.18.0
paho-mqtt>=1.6.1
```

---

## 3. Diretrizes de Teste

### 3.1 Cobertura Mínima

- **Unitários**: Parsers de protocolo, funções de conversão
- **Integração**: Pipeline completo (RFID → JSON → Criptografia → MQTT)
- **Regressão**: Após cada feature implementada

### 3.2 Critérios de Qualidade

| Métrica | Meta |
|---------|------|
| Cobertura de código | > 80% para módulos críticos |
| Tempo de execução | < 5 minutos para suite completa |
| Determinismo | 100% determinístico (sem flaky tests) |

### 3.3 Automação CI/CD

- Scripts devem ser executáveis via linha de comando
- Retornar código de saída: `0` (sucesso), `1` (falha)
- Saída formatada para parsing por ferramentas de CI

---

## 4. Localização dos Scripts

```
teste_automatizado/
├── validate_rfid.py              # Validação de decodificação RFID
├── verify_encryption.py         # Validação AES-256-CBC
├── verify_mqtt_payload.py       # Pipeline MQTT completo
├── verify_ble_gatt.py           # GATT Server e características
├── verify_ota.py                # Comando e trigger OTA
├── verify_offline_cache.py      # Cache FIFO SPIFFS
├── verify_redundancy.py        # Chaveamento Wi-Fi/Celular
├── verify_business_enrichment.py# Banco de dados local
└── requirements.txt             # Dependências Python
```

---

## 5. Execução de Testes

### 5.1 Execução Local

```bash
# Executar todos os testes
cd teste_automatizado
pip install -r requirements.txt
python -m pytest -v

# Executar teste específico
python validate_rfid.py
python verify_encryption.py
```

### 5.2 Integração com ESP-IDF

Para testes de firmware que requerem ferramentas ESP:

```bash
# Validação de compilação
idf.py build

# Flash e monitor
idf.py -p /dev/ttyUSB0 flash monitor
```

---

## 6. Boas Práticas de Teste

1. **Isolamento**: Cada teste deve ser independente
2. **Determinismo**: Resultados consistentes entre execuções
3. **Nomenclatura**: Nome descritivo (`test_rfid_yrm100_frame_valid`)
4. **Documentação**: Docstring explicando o cenário testado
5. **Asserts claros**: Mensagens informativas em falhas
6. **Cleanup**: Liberar recursos após execução (arquivos, conexões)

---

## 7. Referências

- Scripts existentes em: [teste_automatizado/](file:///d:/git/Bastao/Bastão-ESP/teste_automatizado)
- Requisitos do sistema: [ROADMAP.md](file:///d:/git/Bastao/Bastão-ESP/ROADMAP.md)
---
name: testing
description: Diretrizes de testes automatizados para o Bastao-ESP, cobrindo RFID, criptografia, conectividade (MQTT, BLE, OTA), resiliencia e redundancia.
---

# Automated Testing Skill

Esta skill rege o desenvolvimento, manutencao e execucao de rotinas de testes automatizados para validacao do firmware STM32 e ESP32, garantindo a integridade do sistema antes de cada release.

---

## 1. Escopo de Testes

### 1.1 Testes de Validacao de Protocolos RFID

Scripts Python para validacao de decodificacao de tags:

| Script | Descricao | Entrada | Saida Esperada |
|--------|-----------|---------|----------------|
| `validate_rfid.py` | Validacao de frames YRM100 e WL-134 | Frames brutos hex | EPCs decodificados em decimal/hex |
| `verify_lsb_conversion.py` | Teste da logica LSB-first do WL-134 | Strings hex brutas | Codigo do pais e cartao em decimal |

### 1.2 Testes de Criptografia e Seguranca

| Script | Descricao | Entrada | Saida Esperada |
|--------|-----------|---------|----------------|
| `verify_encryption.py` | Validacao AES-256-CBC com PKCS#7 | JSON plaintext | Payload cifrado hex, decifravel com chave |

### 1.3 Testes de Conectividade

| Script | Descricao | Validacao |
|--------|-----------|-----------|
| `verify_mqtt_payload.py` | Pipeline MQTT completo | Payload RFID + battery + GPS → MQTT broker |
| `verify_ble_gatt.py` | Caracteristicas e pareamento GATT | UUIDs unicos, payload < 512 bytes |
| `verify_ota.py` | Comando OTA via MQTT | Extracao de URL e trigger de download |

### 1.4 Testes de Resiliencia e Armazenamento

| Script | Descricao | Validacao |
|--------|-----------|-----------|
| `verify_offline_cache.py` | FIFO cache SPIFFS | Ordenacao FIFO, transmissao apos reconexao |
| `verify_redundancy.py` | Chaveamento Wi-Fi/Celular | Suspensao automatica, reativacao via watchdog |
| `verify_business_enrichment.py` | Lookup de animal no banco local | JSON enriquecido com nome, peso, lote |

---

## 2. Estrutura dos Scripts de Teste

### 2.1 Padrao de Arquitetura

```python
#!/usr/bin/env python3
"""
Script de validacao: [Nome do Teste]
Descricao breve do que e testado.
"""
import sys
from Crypto.Cipher import AES
import json

# Constantes de configuracao
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

### 2.2 Requisitos de Dependencias

Todas as dependencias devem ser documentadas:
```txt
# requirements.txt
pycryptodomex>=3.18.0
paho-mqtt>=1.6.1
```

---

## 3. Diretrizes de Teste

### 3.1 Cobertura Minima

- **Unitarios**: Parsers de protocolo, funcoes de conversao
- **Integracao**: Pipeline completo (RFID → JSON → Criptografia → MQTT)
- **Regressao**: Apos cada feature implementada

### 3.2 Criterios de Qualidade

| Metrica | Meta |
|---------|------|
| Cobertura de codigo | > 80% para modulos criticos |
| Tempo de execucao | < 5 minutos para suite completa |
| Determinismo | 100% deterministico (sem flaky tests) |

### 3.3 Automacao CI/CD

- Scripts devem ser executaveis via linha de comando
- Retornar codigo de saida: `0` (sucesso), `1` (falha)
- Saida formatada para parsing por ferramentas de CI

---

## 4. Localizacao dos Scripts

```
teste_automatizado/
├── validate_rfid.py              # Validacao de decodificacao RFID
├── verify_encryption.py         # Validacao AES-256-CBC
├── verify_mqtt_payload.py       # Pipeline MQTT completo
├── verify_ble_gatt.py           # GATT Server e caracteristicas
├── verify_ota.py                # Comando e trigger OTA
├── verify_offline_cache.py      # Cache FIFO SPIFFS
├── verify_redundancy.py        # Chaveamento Wi-Fi/Celular
├── verify_business_enrichment.py# Banco de dados local
└── requirements.txt             # Dependencias Python
```

---

## 5. Execucao de Testes

### 5.1 Execucao Local

```bash
# Executar todos os testes
cd teste_automatizado
pip install -r requirements.txt
python -m pytest -v

# Executar teste especifico
python validate_rfid.py
python verify_encryption.py
```

### 5.2 Integracao com ESP-IDF

Para testes de firmware que requerem ferramentas ESP:

```bash
# Validacao de compilacao
idf.py build

# Flash e monitor
idf.py -p /dev/ttyUSB0 flash monitor
```

---

## 6. Boas Praticas de Teste

1. **Isolamento**: Cada teste deve ser independente
2. **Determinismo**: Resultados consistentes entre execucoes
3. **Nomenclatura**: Nome descritivo (`test_rfid_yrm100_frame_valid`)
4. **Documentacao**: Docstring explicando o cenario testado
5. **Asserts claros**: Mensagens informativas em falhas
6. **Cleanup**: Liberar recursos apos execucao (arquivos, conexoes)

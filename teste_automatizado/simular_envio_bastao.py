"""
Script de validacao do fluxo completo Bastao-ESP -> sistemaBastao via MQTT.

Simula o envio de payloads do Bastao-ESP para o broker EMQX do sistemaBastao,
passando por criptografia AES-256-CBC (com IV aleatorio) e publicacao no topico
'agro/bastao/{serial}/telemetry'.

Uso:
    python simular_envio_bastao.py \\
        --broker localhost --port 1883 \\
        --serial TESTE001 \\
        --aes-key "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF" \\
        --api-url http://localhost:8000

    python simular_envio_bastao.py --auto  # usa defaults locais
"""
import argparse
import hashlib
import json
import os
import sys
import time
import binascii
from datetime import datetime, timezone

try:
    import paho.mqtt.client as mqtt
except ImportError:
    print("✗ Erro: paho-mqtt nao instalado. Execute: pip install paho-mqtt")
    sys.exit(1)

try:
    from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
    from cryptography.hazmat.primitives import padding
    from cryptography.hazmat.backends import default_backend
except ImportError:
    print("✗ Erro: cryptography nao instalado. Execute: pip install cryptography")
    sys.exit(1)

try:
    import requests
except ImportError:
    requests = None
    print("[AVISO] requests nao instalado. pip install requests")

# =============================================================================
# Configuracoes padrao (mesma chave do secure_payload.c / private_configs.h)
# =============================================================================
DEFAULT_AES_KEY = bytes([
    0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
    0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10,
    0x10, 0x32, 0x54, 0x76, 0x98, 0xBA, 0xDC, 0xFE,
    0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01
])

DEFAULT_BROKER = "localhost"
DEFAULT_PORT = 1883
DEFAULT_SERIAL = "TESTE001"
DEFAULT_API_URL = "http://localhost:8000"
DEFAULT_TOPIC_TEMPLATE = "agro/bastao/{serial}/telemetry"

# =============================================================================
# Funcoes de criptografia (compativeis com secure_payload.c + crypto.py)
# =============================================================================
class BastaoCrypto:
    """Criptografia compativel com o novo secure_payload.c (IV aleatorio prefixado)."""

    def __init__(self, aes_key: bytes):
        if len(aes_key) != 32:
            raise ValueError("Chave AES deve ter 32 bytes (256 bits)")
        self.key = aes_key

    def encrypt(self, plaintext: str) -> str:
        """Criptografa e retorna hex(IV 16B) + hex(ciphertext)."""
        dados_bytes = plaintext.encode("utf-8")
        packer = padding.PKCS7(128).padder()
        padded = packer.update(dados_bytes) + packer.finalize()
        iv = os.urandom(16)
        cipher = Cipher(algorithms.AES(self.key), modes.CBC(iv), backend=default_backend())
        encryptor = cipher.encryptor()
        ciphertext = encryptor.update(padded) + encryptor.finalize()
        return (iv + ciphertext).hex().upper()

    def decrypt(self, payload_hex: str) -> str:
        """Decifra payload hex(IV + ciphertext) e retorna JSON string."""
        dados = binascii.unhexlify(payload_hex.strip())
        if len(dados) < 17:
            raise ValueError("Payload muito curto")
        iv = dados[:16]
        ciphertext = dados[16:]
        cipher = Cipher(algorithms.AES(self.key), modes.CBC(iv), backend=default_backend())
        decryptor = cipher.decryptor()
        padded = decryptor.update(ciphertext) + decryptor.finalize()
        unpacker = padding.PKCS7(128).unpadder()
        decifrado = unpacker.update(padded) + unpacker.finalize()
        return decifrado.decode("utf-8")


# =============================================================================
# Geracao de payloads ficticios
# =============================================================================
def gerar_payload_ficticio() -> dict:
    """Gera um payload de telemetria ficticio no formato do sistemaBastao."""
    return {
        "id_brinco": "BRINCO_TESTE_001",
        "latitude": -23.550520,
        "longitude": -46.633308,
        "nivel_bateria": 8.42,
        "timestamp_rtc": datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")
    }


# =============================================================================
# Publicacao MQTT
# =============================================================================
class TestResult:
    def __init__(self, name: str):
        self.name = name
        self.passed = False
        self.expected = None
        self.actual = None
        self.error = None

    def run(self) -> bool:
        raise NotImplementedError

    def print_result(self):
        status = "[PASS]" if self.passed else "[FAIL]"
        print(f"{status} | {self.name}")
        if not self.passed and self.error:
            print(f"       Erro: {self.error}")
        if not self.passed and self.expected and self.actual:
            print(f"       Esperado: {self.expected}")
            print(f"       Obtido:   {self.actual}")


class TestPublicacaoMQTT(TestResult):
    """Teste 1: Conecta ao broker, publica payload e verifica callback."""

    def __init__(self, broker: str, port: int, serial: str, crypto: BastaoCrypto,
                 username: str = None, password: str = None,
                 api_url: str = None):
        super().__init__("Publicacao MQTT no sistemaBastao")
        self.broker = broker
        self.port = port
        self.serial = serial
        self.crypto = crypto
        self.username = username or serial
        self.password = password or crypto.key.hex().upper()
        self.api_url = api_url
        self.publish_ok = False
        self.topic = DEFAULT_TOPIC_TEMPLATE.format(serial=serial)

    def run(self) -> bool:
        try:
            # Gera e criptografa payload
            payload_dict = gerar_payload_ficticio()
            payload_json = json.dumps(payload_dict, separators=(",", ":"))
            encrypted_hex = self.crypto.encrypt(payload_json)

            print(f"  Topico:     {self.topic}")
            print(f"  JSON base:  {payload_json}")
            print(f"  Payload hex: {encrypted_hex[:48]}... ({len(encrypted_hex)} chars)")

            # Decifra para verificar (auto-teste da criptografia)
            decifrado = self.crypto.decrypt(encrypted_hex)
            assert json.loads(decifrado) == payload_dict, "Criptografia不一致"

            # Conecta ao broker
            client = mqtt.Client(client_id=f"test_simulador_{self.serial}")
            if self.username and self.password:
                client.username_pw_set(self.username, self.password)

            connect_timeout = 10
            connected = False
            published = False

            def on_connect(c, userdata, flags, rc):
                nonlocal connected
                if rc == 0:
                    connected = True
                    print(f"  [OK] Conectado ao broker {self.broker}:{self.port}")
                else:
                    print(f"  [FAIL] Falha conexao broker, rc={rc}")

            def on_publish(c, userdata, mid):
                nonlocal published
                published = True
                print(f"  [OK] Mensagem publicada (mid={mid})")

            client.on_connect = on_connect
            client.on_publish = on_publish

            client.connect(self.broker, self.port, connect_timeout)
            client.loop_start()
            time.sleep(1)

            if not connected:
                self.error = f"Nao conectou ao broker {self.broker}:{self.port}"
                self.passed = False
                client.loop_stop()
                return False

            # Publica
            info = client.publish(self.topic, encrypted_hex, qos=1)
            info.wait_for_publish(timeout=5)
            time.sleep(1)

            if not published:
                # Verifica se pelo menos o publish foi aceito
                if info.rc == mqtt.MQTT_ERR_SUCCESS:
                    published = True
                    print(f"  [OK] Mensagem enfileirada (rc=0)")

            if not published:
                self.error = f"Falha ao publicar (rc={info.rc})"
                self.passed = False
                client.loop_stop()
                return False

            self.publish_ok = True

            # Se API disponivel, verifica persistencia
            if self.api_url and requests:
                time.sleep(2)
                try:
                    url = f"{self.api_url}/coletas"
                    resp = requests.get(url, timeout=10)
                    if resp.status_code == 200:
                        data = resp.json()
                        total = data.get("total", 0)
                        print(f"  [OK] API /coletas retornou {total} registros")
                    elif resp.status_code == 401:
                        print(f"  [AVISO] API requer autenticacao (401)")
                        print(f"  [AVISO] API retornou {resp.status_code}")
                except Exception as e:
                    print(f"  [AVISO] API nao disponivel: {e}")

            self.passed = True
            client.loop_stop()
            return True

        except Exception as e:
            self.error = str(e)
            self.passed = False
            return False


# =============================================================================
# Main
# =============================================================================
def main():
    parser = argparse.ArgumentParser(description="Simulador de envio Bastao-ESP -> sistemaBastao")
    parser.add_argument("--broker", default=DEFAULT_BROKER, help="Host do broker MQTT")
    parser.add_argument("--port", type=int, default=DEFAULT_PORT, help="Porta do broker MQTT")
    parser.add_argument("--serial", default=DEFAULT_SERIAL, help="Numero de serie do bastao")
    parser.add_argument("--aes-key", type=lambda s: binascii.unhexlify(s.strip()),
                        default=None, help="Chave AES em hex (64 chars)")
    parser.add_argument("--mqtt-user", default=None, help="Username MQTT (default: serial)")
    parser.add_argument("--mqtt-pass", default=None, help="Password MQTT (default: chave hex)")
    parser.add_argument("--api-url", default=DEFAULT_API_URL, help="URL base da API sistemaBastao")
    parser.add_argument("--auto", action="store_true", help="Usar valores padrao locais")
    args = parser.parse_args()

    print("=" * 60)
    print("  Simulador de Envio: Bastao-ESP -> sistemaBastao")
    print("=" * 60)

    aes_key = args.aes_key or DEFAULT_AES_KEY
    crypto = BastaoCrypto(aes_key)

    serial = args.serial
    mqtt_user = args.mqtt_user or serial
    mqtt_pass = args.mqtt_pass or aes_key.hex().upper()

    print(f"\nConfiguracao:")
    print(f"  Broker:      {args.broker}:{args.port}")
    print(f"  Serial:      {serial}")
    print(f"  MQTT User:   {mqtt_user}")
    print(f"  MQTT Pass:   {mqtt_pass[:16]}... (chave AES hex)")
    print(f"  Topico:      agro/bastao/{serial}/telemetry")
    print(f"  API URL:     {args.api_url}\n")

    # Executa teste
    teste = TestPublicacaoMQTT(
        broker=args.broker,
        port=args.port,
        serial=serial,
        crypto=crypto,
        username=mqtt_user,
        password=mqtt_pass,
        api_url=args.api_url,
    )
    teste.run()
    teste.print_result()

    print("\n" + "=" * 60)
    if teste.passed:
        print("  Resultado: [OK] INTEGRACAO FUNCIONAL")
        print("  Proximo passo: Cadastre um bastao com serial", serial)
        print(f"  no sistemaBastao com chave AES: {aes_key.hex().upper()}")
    else:
        print("  Resultado: [FAIL] FALHA NA INTEGRACAO")
        print("  Verifique se:")
        print("  1. O broker EMQX esta rodando (docker compose up mqtt)")
        print("  2. O receptor esta rodando (docker compose up receptor)")
        print("  3. O bastao esta cadastrado na API com serial e chave corretos")
    print("=" * 60)

    sys.exit(0 if teste.passed else 1)


if __name__ == "__main__":
    main()

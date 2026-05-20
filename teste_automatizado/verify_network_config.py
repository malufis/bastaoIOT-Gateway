#!/usr/bin/env python3
"""
Script de teste para validacao de configuracao de rede via BLE GATT.

Este script valida o formato JSON de configuracao de rede que o app
envia para o Bastao-ESP via GATT Server (CHAR_CONFIG_WRITE).
"""

import json
import sys

def test_wifi_config():
    """Teste de configuracao Wi-Fi."""
    print("\n=== Teste: Wi-Fi Config ===")

    valid_configs = [
        {
            "wifi": {
                "ssid": "bastaoIOT",
                "password": "3spB@st@0",
                "enabled": True
            }
        },
        {
            "wifi": {
                "ssid": "RedeFazenda",
                "password": "senha123",
                "enabled": True
            }
        }
    ]

    for cfg in valid_configs:
        json_str = json.dumps(cfg)
        print(f"  Validando: {json_str}")
        assert len(json_str) < 512, "Config Wi-Fi excede tamanho maximo!"
        parsed = json.loads(json_str)
        assert "wifi" in parsed
        assert "ssid" in parsed["wifi"]
        assert "password" in parsed["wifi"]
        print(f"  ✓ Validado: SSID={parsed['wifi']['ssid']}")

    return True

def test_cellular_config():
    """Teste de configuracao celular/APN."""
    print("\n=== Teste: Celular Config ===")

    valid_configs = [
        {
            "cellular": {
                "apn": "zap.vivo.com.br",
                "user": "vivo",
                "password": "vivo",
                "enabled": True
            }
        },
        {
            "cellular": {
                "apn": "claro.com.br",
                "user": "claro",
                "password": "claro",
                "enabled": True
            }
        }
    ]

    for cfg in valid_configs:
        json_str = json.dumps(cfg)
        print(f"  Validando: {json_str}")
        assert len(json_str) < 512, "Config Celular excede tamanho maximo!"
        parsed = json.loads(json_str)
        assert "cellular" in parsed
        assert "apn" in parsed["cellular"]
        print(f"  ✓ Validado: APN={parsed['cellular']['apn']}")

    return True

def test_mqtt_config():
    """Teste de configuracao MQTT."""
    print("\n=== Teste: MQTT Config ===")

    valid_configs = [
        {
            "mqtt": {
                "broker_uri": "mqtt://broker.example.com:1883",
                "client_id": "bastao-esp-001",
                "topic_telemetry": "bastao/telemetria",
                "topic_gps": "bastao/gps"
            }
        }
    ]

    for cfg in valid_configs:
        json_str = json.dumps(cfg)
        print(f"  Validando: {json_str}")
        assert len(json_str) < 512, "Config MQTT excede tamanho maximo!"
        parsed = json.loads(json_str)
        assert "mqtt" in parsed
        assert "broker_uri" in parsed["mqtt"]
        assert "client_id" in parsed["mqtt"]
        print(f"  ✓ Validado: Broker={parsed['mqtt']['broker_uri']}")

    return True

def test_network_mode():
    """Teste de modo de rede."""
    print("\n=== Teste: Network Mode ===")

    modes = [
        {"network_mode": 0},  # WIFI_ONLY
        {"network_mode": 1},  # CELLULAR_ONLY
        {"network_mode": 2},  # WIFI_CELLULAR
        {"network_mode": 3},  # AUTO
    ]

    for mode in modes:
        json_str = json.dumps(mode)
        print(f"  Validando: {json_str}")
        parsed = json.loads(json_str)
        assert "network_mode" in parsed
        assert 0 <= parsed["network_mode"] <= 3
        print(f"  ✓ Validado: Mode={parsed['network_mode']}")

    return True

def test_full_config():
    """Teste de configuracao completa."""
    print("\n=== Teste: Full Config ===")

    full_config = {
        "wifi": {
            "ssid": "bastaoIOT",
            "password": "3spB@st@0",
            "enabled": True
        },
        "cellular": {
            "apn": "zap.vivo.com.br",
            "user": "vivo",
            "password": "vivo",
            "enabled": True
        },
        "mqtt": {
            "broker_uri": "mqtt://broker.example.com:1883",
            "client_id": "bastao-esp-001",
            "topic_telemetry": "bastao/telemetria",
            "topic_gps": "bastao/gps"
        },
        "network_mode": 2
    }

    json_str = json.dumps(full_config)
    print(f"  Config completo: {json_str}")
    assert len(json_str) < 512, "Config completo excede tamanho maximo!"

    parsed = json.loads(json_str)
    assert "wifi" in parsed
    assert "cellular" in parsed
    assert "mqtt" in parsed
    assert "network_mode" in parsed

    print(f"  ✓ Validado: Configuracao completa ({len(json_str)} bytes)")
    return True

def main():
    print("=" * 50)
    print("Teste de Configuracao de Rede via BLE GATT")
    print("=" * 50)

    tests = [
        ("Wi-Fi Config", test_wifi_config),
        ("Celular Config", test_cellular_config),
        ("MQTT Config", test_mqtt_config),
        ("Network Mode", test_network_mode),
        ("Full Config", test_full_config),
    ]

    passed = 0
    for name, test_func in tests:
        try:
            if test_func():
                passed += 1
                print(f"✓ PASS | {name}")
        except Exception as e:
            print(f"✗ FAIL | {name}: {e}")

    print("\n" + "=" * 50)
    print(f"Resultado: {passed}/{len(tests)} testes passaram")
    print("=" * 50)

    return 0 if passed == len(tests) else 1

if __name__ == "__main__":
    sys.exit(main())
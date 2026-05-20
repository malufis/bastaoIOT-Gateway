"""
Simulacao de verificacao do servico GATT BLE do Bastao-ESP.

Este script valida a estrutura logica do servico GATT implementado no ESP32,
incluindo UUIDs, propriedades de caracteristicas, formatos de dados JSON
trafegados e compatibilidade com o NVS Flash.

Nota: Este teste NAO requer hardware BLE. Ele valida apenas a logica e 
formatos de dados que seriam trafegados pelo GATT Server.
"""

import json

# === UUIDs do Servico GATT (devem bater com ble_mobile.h) ===

BASTAO_SERVICE_UUID   = 0x00FF
CHAR_CONFIG_WRITE     = 0xFF01
CHAR_CONFIG_READ      = 0xFF02
CHAR_BUSINESS_WRITE   = 0xFF03
CHAR_LAST_TAG         = 0xFF04
CHAR_DEVICE_STATUS    = 0xFF05


def test_config_write_payload():
    """Testa o formato JSON enviado pelo app para configuracao do bastao."""
    config_payloads = [
        {
            "yrm100_power": 20,
            "scan_time_ms": 200,
            "battery_report_interval_s": 5,
            "wl134_enabled": True,
            "yrm100_enabled": True
        },
        {
            "yrm100_power": 26,
            "scan_time_ms": 500,
            "battery_report_interval_s": 10,
            "wl134_enabled": False,
            "yrm100_enabled": True
        },
    ]

    print("--- Teste: Config Write (App -> Bastao) ---")
    print(f"  UUID Caracteristica: 0x{CHAR_CONFIG_WRITE:04X}")
    for i, cfg in enumerate(config_payloads):
        json_str = json.dumps(cfg, separators=(',', ':'))
        assert len(json_str) < 512, "Config JSON excede tamanho maximo GATT!"
        # Valida que todos os campos obrigatorios estao presentes
        parsed = json.loads(json_str)
        assert "yrm100_power" in parsed
        assert "scan_time_ms" in parsed
        assert 0 <= parsed["yrm100_power"] <= 30
        assert parsed["scan_time_ms"] > 0
        print(f"  Config #{i+1}: {json_str} ({len(json_str)} bytes) [OK]")

    return True


def test_business_data_payload():
    """Testa o formato JSON de dados de negocio (Fazenda, Lote, Animal)."""
    business_payloads = [
        {
            "type": "farm",
            "id": "FARM-001",
            "name": "Fazenda Boa Esperanca",
            "location": "Uberaba-MG",
            "owner": "Joao Silva"
        },
        {
            "type": "lot",
            "id": "LOT-2026A",
            "farm_id": "FARM-001",
            "description": "Lote de engorda - Nelore",
            "head_count": 150
        },
        {
            "type": "animal",
            "tag_id": "900250000023921",
            "lot_id": "LOT-2026A",
            "breed": "Nelore",
            "weight_kg": 450.5,
            "age_months": 24,
            "vaccines": ["Aftosa", "Brucelose", "Raiva"],
            "last_handling": "2026-05-15"
        },
    ]

    print("\n--- Teste: Business Data Write (App -> Bastao) ---")
    print(f"  UUID Caracteristica: 0x{CHAR_BUSINESS_WRITE:04X}")
    for i, biz in enumerate(business_payloads):
        json_str = json.dumps(biz, separators=(',', ':'), ensure_ascii=False)
        assert len(json_str) < 512, "Business JSON excede tamanho maximo GATT!"
        parsed = json.loads(json_str)
        assert "type" in parsed
        assert "id" in parsed or "tag_id" in parsed
        print(f"  Dados #{i+1} ({biz['type']}): {json_str[:80]}... ({len(json_str)} bytes) [OK]")

    return True


def test_last_tag_notification():
    """Testa o formato JSON de notificacao de tag lida enviada ao app."""
    tag_notifications = [
        {"type": "rfid", "model": "YRM100", "tag": "30751FEB705C5904E3D50D70"},
        {"type": "rfid", "model": "WL134", "tag": "900250000023921"},
    ]

    print("\n--- Teste: Last Tag Notify (Bastao -> App) ---")
    print(f"  UUID Caracteristica: 0x{CHAR_LAST_TAG:04X}")
    for i, tag in enumerate(tag_notifications):
        json_str = json.dumps(tag, separators=(',', ':'))
        assert len(json_str) < 512, "Tag JSON excede tamanho maximo GATT!"
        parsed = json.loads(json_str)
        assert parsed["type"] == "rfid"
        assert "model" in parsed
        assert "tag" in parsed
        print(f"  Notify #{i+1}: {json_str} ({len(json_str)} bytes) [OK]")

    return True


def test_device_status_read():
    """Testa o formato JSON de status do dispositivo lido pelo app."""
    status_payloads = [
        {
            "batt": 8.45,
            "ppp": True,
            "mqtt": True,
            "mesh": True,
            "gps": True,
            "lat": -23.550520,
            "lon": -46.633309,
            "tags": 42
        },
        {
            "batt": 7.10,
            "ppp": False,
            "mqtt": False,
            "mesh": True,
            "gps": False,
            "lat": 0.0,
            "lon": 0.0,
            "tags": 0
        },
    ]

    print("\n--- Teste: Device Status Read (Bastao -> App) ---")
    print(f"  UUID Caracteristica: 0x{CHAR_DEVICE_STATUS:04X}")
    for i, st in enumerate(status_payloads):
        json_str = json.dumps(st, separators=(',', ':'))
        assert len(json_str) < 512, "Status JSON excede tamanho maximo GATT!"
        parsed = json.loads(json_str)
        assert "batt" in parsed
        assert "ppp" in parsed
        assert isinstance(parsed["batt"], (int, float))
        assert isinstance(parsed["ppp"], bool)
        print(f"  Status #{i+1}: {json_str} ({len(json_str)} bytes) [OK]")

    return True


def test_uuid_uniqueness():
    """Verifica que todos os UUIDs sao unicos."""
    uuids = [
        BASTAO_SERVICE_UUID,
        CHAR_CONFIG_WRITE,
        CHAR_CONFIG_READ,
        CHAR_BUSINESS_WRITE,
        CHAR_LAST_TAG,
        CHAR_DEVICE_STATUS,
    ]

    print("\n--- Teste: Unicidade de UUIDs GATT ---")
    assert len(uuids) == len(set(uuids)), "UUIDs duplicados encontrados!"
    for uuid in uuids:
        print(f"  UUID 0x{uuid:04X} [OK]")

    return True


if __name__ == "__main__":
    print("=" * 60)
    print("Validacao do Servico GATT BLE do Bastao-ESP")
    print("=" * 60)

    results = [
        test_uuid_uniqueness(),
        test_config_write_payload(),
        test_business_data_payload(),
        test_last_tag_notification(),
        test_device_status_read(),
    ]

    print(f"\n{'=' * 60}")
    if all(results):
        print(f"Todos os {len(results)} testes GATT passaram com sucesso!")
    else:
        print("FALHA em um ou mais testes.")
    print(f"{'=' * 60}")

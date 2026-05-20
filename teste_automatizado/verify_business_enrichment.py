"""
Simulação de verificação de Associação e Enriquecimento de Dados de Negócio (Animal DB).

Este script valida:
1. Parseamento correto do banco de dados em JSON estruturado vindo da NVS.
2. Associação por busca linear correspondente ao ID da tag RFID.
3. Formatação correta do JSON enriquecido (incluindo nome, peso e lote).
4. Tratamento de fallback para tags não cadastradas (gera JSON básico sem falhar).
"""

import json

class SimulatedAnimalDB:
    def __init__(self, biz_json_str):
        self.db = []
        if biz_json_str:
            try:
                self.db = json.loads(biz_json_str)
            except Exception as e:
                print(f"[ERR] Erro ao carregar JSON: {e}")
                
    def lookup(self, tag):
        for item in self.db:
            if item.get("tag") == tag:
                return {
                    "tag": item.get("tag"),
                    "name": item.get("name", ""),
                    "weight": float(item.get("weight", 0.0)),
                    "lot": item.get("lot", "")
                }
        return None

def format_dispatcher_payload(tag, model, db_manager):
    record = db_manager.lookup(tag)
    if record:
        # JSON Enriquecido
        payload = {
            "type": "rfid",
            "model": model,
            "tag": tag,
            "name": record["name"],
            "weight": round(record["weight"], 2),
            "lot": record["lot"]
        }
    else:
        # JSON Básico
        payload = {
            "type": "rfid",
            "model": model,
            "tag": tag
        }
    return json.dumps(payload, separators=(',', ':'))

def test_business_enrichment():
    print("=" * 70)
    print("Iniciando Validação de Enriquecimento de Dados de Negócio (Animal DB)")
    print("=" * 70)
    
    # 1. Base de dados de negócio simulada
    biz_data = """[
        {"tag": "123456789012345", "name": "Mimoso", "weight": 450.5, "lot": "Lote A"},
        {"tag": "987654321098765", "name": "Estrela", "weight": 420.25, "lot": "Lote B"}
    ]"""
    
    db_manager = SimulatedAnimalDB(biz_data)
    
    # Teste 1: Busca de tag cadastrada (Mimoso)
    rec1 = db_manager.lookup("123456789012345")
    assert rec1 is not None
    assert rec1["name"] == "Mimoso"
    assert rec1["weight"] == 450.5
    assert rec1["lot"] == "Lote A"
    print("[TEST 1] Busca de tag cadastrada (Mimoso) OK.")
    
    # Teste 2: Busca de outra tag cadastrada (Estrela)
    rec2 = db_manager.lookup("987654321098765")
    assert rec2 is not None
    assert rec2["name"] == "Estrela"
    assert rec2["weight"] == 420.25
    assert rec2["lot"] == "Lote B"
    print("[TEST 2] Busca de tag cadastrada (Estrela) OK.")
    
    # Teste 3: Busca de tag não cadastrada (Fallback)
    rec3 = db_manager.lookup("000000000000000")
    assert rec3 is None
    print("[TEST 3] Busca de tag não cadastrada (Fallback = None) OK.")
    
    # Teste 4: Formatação de payload enriquecido
    payload_enriched = format_dispatcher_payload("123456789012345", "YRM100", db_manager)
    expected_enriched = '{"type":"rfid","model":"YRM100","tag":"123456789012345","name":"Mimoso","weight":450.5,"lot":"Lote A"}'
    assert payload_enriched == expected_enriched
    print(f"[TEST 4] Payload enriquecido gerado: {payload_enriched} (OK)")
    
    # Teste 5: Formatação de payload básico (fallback)
    payload_basic = format_dispatcher_payload("000000000000000", "WL134", db_manager)
    expected_basic = '{"type":"rfid","model":"WL134","tag":"000000000000000"}'
    assert payload_basic == expected_basic
    print(f"[TEST 5] Payload básico de fallback gerado: {payload_basic} (OK)")
    
    print("\n" + "=" * 70)
    print("Validação do Enriquecimento de Dados Concluída com Sucesso!")
    print("=" * 70)

if __name__ == "__main__":
    test_business_enrichment()

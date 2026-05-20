"""
Simulação de verificação de Redundância e Comutação Automática do Bastão-ESP.

Este script valida:
1. Priorização de rota (Wi-Fi sobre Celular/PPP).
2. Suspensão correta do celular quando o Wi-Fi obtém IP.
3. Reativação automática do celular quando o Wi-Fi é desconectado.
4. Prevenção de conflitos de reconexão no watchdog celular.
"""

class SimulatedRedundancyManager:
    def __init__(self):
        self.wifi_connected = False
        self.cellular_suspended = False
        self.cellular_connected = False
        self.watchdog_runs = 0
        
    def start_state(self):
        # Estado inicial: celular tenta conectar
        self.wifi_connected = False
        self.cellular_suspended = False
        self.cellular_connected = True
        print("[INIT] Estado inicial: Wi-Fi desconectado, Celular/PPP ativo.")
        
    def on_wifi_got_ip(self):
        print("\n--- [EVENTO] Wi-Fi conectado! IP alocado. ---")
        self.wifi_connected = True
        # Lógica do driver: suspende celular
        self.set_cellular_suspended(True)
        
    def on_wifi_disconnected(self):
        print("\n--- [EVENTO] Wi-Fi desconectado! ---")
        self.wifi_connected = False
        # Lógica do driver: reativa celular
        self.set_cellular_suspended(False)
        
    def set_cellular_suspended(self, suspend):
        self.cellular_suspended = suspend
        if suspend:
            print("[CELL] Conectividade celular SUSPENSA. Desconectando PPP...")
            self.cellular_connected = False
        else:
            print("[CELL] Conectividade celular REATIVADA. Permitindo conexões.")
            
    def run_cellular_watchdog(self):
        self.watchdog_runs += 1
        print(f"[WATCHDOG #{self.watchdog_runs}] Executando verificação do modem...")
        if self.cellular_suspended:
            print("  Watchdog: Celular suspenso (Wi-Fi ativo). Pulando reconexão.")
            return
        if not self.cellular_connected:
            print("  Watchdog: Celular desconectado. Restabelecendo sessão PPP...")
            self.cellular_connected = True
        else:
            print("  Watchdog: Celular já conectado e ativo. Nada a fazer.")

def test_redundancy_switching():
    print("=" * 70)
    print("Iniciando Verificação de Redundância e Comutação Automática")
    print("=" * 70)
    
    manager = SimulatedRedundancyManager()
    
    # 1. Estado inicial
    manager.start_state()
    assert manager.cellular_connected is True
    assert manager.cellular_suspended is False
    
    # Executa watchdog
    manager.run_cellular_watchdog()
    assert manager.cellular_connected is True
    
    # 2. Conecta Wi-Fi
    manager.on_wifi_got_ip()
    assert manager.wifi_connected is True
    assert manager.cellular_suspended is True
    assert manager.cellular_connected is False
    
    # Executa watchdog (deve pular reconexão)
    manager.run_cellular_watchdog()
    assert manager.cellular_connected is False
    
    # 3. Desconecta Wi-Fi
    manager.on_wifi_disconnected()
    assert manager.wifi_connected is False
    assert manager.cellular_suspended is False
    # Conexão celular deve ser reativada, mas ainda não conectada até que o watchdog rode
    assert manager.cellular_connected is False
    
    # Executa watchdog (deve reconectar)
    manager.run_cellular_watchdog()
    assert manager.cellular_connected is True
    
    print("\n" + "=" * 70)
    print("Validação do Gerenciador de Redundância Concluída com Sucesso!")
    print("=" * 70)

if __name__ == "__main__":
    test_redundancy_switching()

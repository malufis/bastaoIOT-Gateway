#!/usr/bin/env python3
"""
Script de teste de estresse da rede BLE Mesh do Bastão-ESP.

Este script simula cenários de estresse para validar a resiliência da
comunicação Mesh entre o ESP32 (Coordenador) e a Tela K10 (Nó).

Cenários testados:
1. Envio rápido de múltiplas mensagens (flood)
2. Simulação de perda de pacotes aleatória
3. Reconexão após perda de conectividade
4. Verificação de ordenação de mensagens
"""

import random
import json
import time
from dataclasses import dataclass
from typing import List, Optional

@dataclass
class MeshMessage:
    seq: int
    payload: str
    timestamp: float

class MeshNetworkSimulator:
    def __init__(self, loss_rate: float = 0.1):
        self.loss_rate = loss_rate
        self.sent_messages: List[MeshMessage] = []
        self.received_messages: List[MeshMessage] = []
        self.connection_active = True
        self.seq_counter = 0

    def send_message(self, payload: str) -> bool:
        """Simula envio de mensagem pela rede Mesh."""
        if not self.connection_active:
            print(f"  [NET] Conexão inativa - mensagem descartada")
            return False

        if random.random() < self.loss_rate:
            print(f"  [NET] Pacote perdido (simulação {self.loss_rate*100:.0f}% loss)")
            self.sent_messages.append(MeshMessage(
                seq=self.seq_counter,
                payload=payload,
                timestamp=time.time()
            ))
            self.seq_counter += 1
            return False

        msg = MeshMessage(
            seq=self.seq_counter,
            payload=payload,
            timestamp=time.time()
        )
        self.sent_messages.append(msg)
        self.received_messages.append(msg)
        self.seq_counter += 1
        return True

    def simulate_disconnect(self):
        """Simula perda de conexão com a rede Mesh."""
        self.connection_active = False
        print(f"  [NET] Conexão perdida com a Tela K10")

    def simulate_reconnect(self):
        """Simula restabelecimento de conexão."""
        self.connection_active = True
        print(f"  [NET] Conexão restabelecida")

    def get_stats(self) -> dict:
        """Retorna estatísticas da simulação."""
        return {
            "sent": len(self.sent_messages),
            "received": len(self.received_messages),
            "lost": len(self.sent_messages) - len(self.received_messages),
            "loss_rate": (len(self.sent_messages) - len(self.received_messages)) / max(1, len(self.sent_messages)),
            "connection_active": self.connection_active
        }


class MeshStressTest:
    def __init__(self):
        self.results = []

    def run_flood_test(self, count: int = 50) -> bool:
        """Teste 1: Envio rápido de múltiplas mensagens."""
        print(f"\n=== Teste 1: Flood Test ({count} mensagens) ===")
        net = MeshNetworkSimulator(loss_rate=0.05)

        for i in range(count):
            payload = json.dumps({
                "type": "rfid",
                "model": "YRM100",
                "tag": f"TEST{i:08X}",
                "seq": i
            })
            success = net.send_message(payload)
            if not success:
                print(f"  Mensagem {i} perdida")

        stats = net.get_stats()
        print(f"  Resultado: {stats['received']}/{stats['sent']} recebidas")

        passed = stats['loss_rate'] < 0.15
        self.results.append(("Flood Test", passed))
        return passed

    def run_packet_loss_test(self, count: int = 100, loss_rate: float = 0.2) -> bool:
        """Teste 2: Simulação de perda de pacotes."""
        print(f"\n=== Teste 2: Packet Loss Test ({count} msg, {loss_rate*100:.0f}% loss) ===")
        net = MeshNetworkSimulator(loss_rate=loss_rate)

        for i in range(count):
            payload = json.dumps({"seq": i, "data": f"msg_{i}"})
            net.send_message(payload)

        stats = net.get_stats()
        print(f"  Resultado: {stats['received']}/{stats['sent']} ({stats['loss_rate']*100:.1f}% perda)")

        expected_loss = loss_rate
        passed = abs(stats['loss_rate'] - expected_loss) < 0.05
        self.results.append(("Packet Loss", passed))
        return passed

    def run_reconnection_test(self, msg_before: int = 10, msg_after: int = 10) -> bool:
        """Teste 3: Reconexão após perda de conectividade."""
        print(f"\n=== Teste 3: Reconnection Test ===")
        net = MeshNetworkSimulator(loss_rate=0.0)

        for i in range(msg_before):
            net.send_message(json.dumps({"seq": i, "phase": "before"}))

        net.simulate_disconnect()
        for i in range(3):
            net.send_message(json.dumps({"seq": i, "phase": "disconnected"}))

        net.simulate_reconnect()

        for i in range(msg_after):
            net.send_message(json.dumps({"seq": i, "phase": "after"}))

        stats = net.get_stats()
        print(f"  Resultado: {stats['received']}/{stats['sent']} mensagens")

        passed = stats['received'] == msg_before + msg_after
        self.results.append(("Reconnection", passed))
        return passed

    def run_ordering_test(self, count: int = 30) -> bool:
        """Teste 4: Verificação de ordenação de mensagens."""
        print(f"\n=== Teste 4: Ordering Test ({count} mensagens) ===")
        net = MeshNetworkSimulator(loss_rate=0.1)

        for i in range(count):
            payload = json.dumps({"seq": i, "timestamp": time.time()})
            net.send_message(payload)

        received_seqs = [m.seq for m in net.received_messages]
        expected_seqs = list(range(count))

        received_seqs_sorted = sorted(received_seqs) == expected_seqs[:len(received_seqs)]

        print(f"  Recebidas: {len(received_seqs)} - Ordenadas: {received_seqs_sorted}")

        self.results.append(("Ordering", received_seqs_sorted))
        return received_seqs_sorted

    def run_interference_test(self, count: int = 50) -> bool:
        """Teste 5: Simulação de interferência Wi-Fi/BLE coexistente."""
        print(f"\n=== Teste 5: Interference Test ({count} mensagens) ===")
        net = MeshNetworkSimulator(loss_rate=0.25)

        successful = 0
        for i in range(count):
            payload = json.dumps({"type": "rfid", "seq": i, "interference": True})
            if net.send_message(payload):
                successful += 1

        stats = net.get_stats()
        print(f"  Resultado: {stats['received']}/{stats['sent']} ({stats['loss_rate']*100:.1f}% perda)")
        print(f"  Recomendacao: Em campo com interferencia, usar retry automatico")

        passed = stats['loss_rate'] < 0.35
        self.results.append(("Interference", passed))
        return passed

    def run_retry_test(self, count: int = 50, max_retries: int = 3) -> bool:
        """Teste 6: Mecanismo de retry com confirmação."""
        print(f"\n=== Teste 6: Retry Test ({count} msg, {max_retries} retries) ===")
        net = MeshNetworkSimulator(loss_rate=0.15)
        acked_messages = []

        for i in range(count):
            payload = json.dumps({"seq": i, "retry": True})
            for retry in range(max_retries):
                if net.send_message(payload):
                    acked_messages.append(i)
                    break

        stats = net.get_stats()
        final_received = len(acked_messages)
        print(f"  Resultado: {final_received}/{stats['sent']} com ate {max_retries} retries")
        print(f"  Aprovado: {final_received >= stats['sent'] * 0.95}")

        passed = final_received >= stats['sent'] * 0.95
        self.results.append(("Retry Mechanism", passed))
        return passed

    def run_latency_test(self, count: int = 20) -> bool:
        """Teste 7: Latência de comunicação Mesh."""
        print(f"\n=== Teste 7: Latency Test ({count} mensagens) ===")
        net = MeshNetworkSimulator(loss_rate=0.0)
        latencies = []

        for i in range(count):
            start = time.time()
            payload = json.dumps({"seq": i, "timestamp": start})
            net.send_message(payload)
            end = time.time()
            latencies.append((end - start) * 1000)

        avg_latency = sum(latencies) / len(latencies)
        max_latency = max(latencies)
        print(f"  Latencia media: {avg_latency:.2f} ms")
        print(f"  Latencia maxima: {max_latency:.2f} ms")

        passed = avg_latency < 100
        self.results.append(("Latency", passed))
        return passed

    def print_summary(self):
        print("\n" + "=" * 50)
        print("RESUMO DOS TESTES DE ESTRESSE MESH")
        print("=" * 50)

        passed = 0
        for test_name, result in self.results:
            status = "[PASS]" if result else "[FAIL]"
            print(f"{status} | {test_name}")
            if result:
                passed += 1

        print(f"\nTotal: {passed}/{len(self.results)} testes passaram")
        return passed == len(self.results)


def main():
    print("=" * 50)
    print("Testes de Estresse - Rede BLE Mesh")
    print("=" * 50)

    test_suite = MeshStressTest()

    test_suite.run_flood_test(50)
    test_suite.run_packet_loss_test(100, 0.2)
    test_suite.run_reconnection_test(10, 10)
    test_suite.run_ordering_test(30)
    test_suite.run_interference_test(50)
    test_suite.run_retry_test(50, 3)
    test_suite.run_latency_test(20)

    success = test_suite.print_summary()
    return 0 if success else 1


if __name__ == "__main__":
    exit(main())
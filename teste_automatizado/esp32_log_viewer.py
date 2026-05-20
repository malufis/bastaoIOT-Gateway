#!/usr/bin/env python3
"""
Script de visualizacao de logs do Bastão-ESP via Telnet.

Coneeta ao servidor Telnet do dispositivo e exibe os logs
em tempo real com cores e formatacao.

Uso:
    python esp32_log_viewer.py <IP_DO_BASTAO> [porta]
"""

import socket
import sys
import threading
import time
import argparse
import os

os.system("")  # Enable ANSI colors on Windows

COLOR_RESET = "\033[0m"
COLOR_VERBOSE = "\033[37m"
COLOR_DEBUG = "\033[36m"
COLOR_INFO = "\033[32m"
COLOR_WARN = "\033[33m"
COLOR_ERROR = "\033[31m"
COLOR_BOLD = "\033[1m"

LEVEL_COLORS = {
    "V": COLOR_VERBOSE,
    "D": COLOR_DEBUG,
    "I": COLOR_INFO,
    "W": COLOR_WARN,
    "E": COLOR_ERROR,
}

class LogViewer:
    def __init__(self, host, port=23):
        self.host = host
        self.port = port
        self.running = False
        self.sock = None
        self.stats = {
            "total": 0,
            "verbose": 0,
            "debug": 0,
            "info": 0,
            "warn": 0,
            "error": 0,
        }
        self.start_time = time.time()

    def format_log_line(self, line):
        """Formata linha de log com cores."""
        if len(line) < 10:
            return line

        parts = line.strip().split("][")
        if len(parts) < 2:
            return line

        try:
            time_part = parts[0][1:]
            level_part = parts[1]
            tag_part = parts[2] if len(parts) > 2 else ""
            msg_part = parts[3] if len(parts) > 3 else ""

            level = level_part[0] if level_part else "I"
            color = LEVEL_COLORS.get(level, COLOR_RESET)

            return (f"{COLOR_BOLD}{COLOR_DEBUG}{time_part}{COLOR_RESET} "
                    f"[{color}{level_part}{COLOR_RESET}] "
                    f"[{COLOR_WARN}{tag_part}{COLOR_RESET}] "
                    f"{msg_part}")

        except (IndexError, ValueError):
            return line

    def print_banner(self):
        """Imprime banner inicial."""
        print(f"{COLOR_BOLD}{COLOR_INFO}")
        print("=" * 60)
        print(f"   Bastão-ESP Log Viewer - {self.host}:{self.port}")
        print("=" * 60)
        print(f"{COLOR_RESET}")
        print("Pressione Ctrl+C para sair\n")

    def print_stats(self):
        """Imprime estatísticas."""
        elapsed = time.time() - self.start_time
        print(f"\n{COLOR_BOLD}--- Estatisticas ({elapsed:.0f}s) ---{COLOR_RESET}")
        print(f"  Total: {self.stats['total']} | "
              f"D: {self.stats['debug']} | "
              f"I: {self.stats['info']} | "
              f"W: {self.stats['warn']} | "
              f"E: {self.stats['error']}")

    def update_stats(self, line):
        """Atualiza estatísticas."""
        self.stats["total"] += 1

        if "]V]" in line:
            self.stats["verbose"] += 1
        elif "]D]" in line:
            self.stats["debug"] += 1
        elif "]I]" in line:
            self.stats["info"] += 1
        elif "]W]" in line:
            self.stats["warn"] += 1
        elif "]E]" in line:
            self.stats["error"] += 1

    def receive_logs(self):
        """Thread para receber logs do Telnet."""
        buffer = ""

        while self.running:
            try:
                data = self.sock.recv(4096)
                if not data:
                    break

                buffer += data.decode("utf-8", errors="replace")

                while "\n" in buffer:
                    line, buffer = buffer.split("\n", 1)
                    if line.strip():
                        self.update_stats(line)
                        formatted = self.format_log_line(line)
                        print(formatted)

            except socket.timeout:
                continue
            except Exception as e:
                if self.running:
                    print(f"\n{COLOR_ERROR}Erro: {e}{COLOR_RESET}")
                break

    def connect(self):
        """Conecta ao servidor Telnet."""
        try:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.sock.settimeout(5)
            self.sock.connect((self.host, self.port))
            self.sock.settimeout(1)
            print(f"{COLOR_INFO}Conectado ao Bastão-ESP{COLOR_RESET}")
            return True

        except socket.timeout:
            print(f"{COLOR_ERROR}Timeout ao conectar{COLOR_RESET}")
            return False
        except socket.error as e:
            print(f"{COLOR_ERROR}Erro de conexao: {e}{COLOR_RESET}")
            return False

    def send_command(self, cmd):
        """Envia comando para o dispositivo."""
        if self.sock:
            try:
                self.sock.sendall((cmd + "\r\n").encode())
                time.sleep(0.2)
            except Exception as e:
                print(f"{COLOR_ERROR}Erro ao enviar: {e}{COLOR_RESET}")

    def start(self):
        """Inicia o visualizador de logs."""
        if not self.connect():
            return False

        self.running = True
        self.print_banner()

        receive_thread = threading.Thread(target=self.receive_logs)
        receive_thread.daemon = True
        receive_thread.start()

        try:
            while self.running:
                time.sleep(1)
                if self.stats["total"] > 0 and self.stats["total"] % 50 == 0:
                    self.print_stats()

        except KeyboardInterrupt:
            print(f"\n{COLOR_WARN}Encerrando...{COLOR_RESET}")
        finally:
            self.stop()

        self.print_stats()
        return True

    def stop(self):
        """Para o visualizador."""
        self.running = False
        if self.sock:
            self.sock.close()
        print(f"{COLOR_INFO}Desconectado{COLOR_RESET}")


def main():
    parser = argparse.ArgumentParser(
        description="Bastao-ESP Log Viewer via Telnet",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Exemplos:
    python esp32_log_viewer.py 192.168.1.100
    python esp32_log_viewer.py 192.168.1.100 23
    python esp32_log_viewer.py 192.168.1.100 -p 24
        """
    )

    parser.add_argument("host", help="Endereco IP do Bastão-ESP")
    parser.add_argument("port", nargs="?", type=int, default=23,
                        help="Porta Telnet (padrao: 23)")

    args = parser.parse_args()

    viewer = LogViewer(args.host, args.port)
    viewer.start()


if __name__ == "__main__":
    main()
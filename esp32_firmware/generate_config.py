#!/usr/bin/env python3
"""
Gerador de private_configs.h a partir de private_configs.env.

Uso:
    python generate_config.py                    # usa private_configs.env
    python generate_config.py --env meu.env      # usa arquivo customizado

Saida: main/private_configs.h
"""
import argparse
import os
import re
import sys

HEADER = """/**
 * @file private_configs.h
 * @brief Configuracoes privadas geradas automaticamente.
 * @note GERADO AUTOMATICAMENTE por generate_config.py.
 *       NAO edite manualmente. Edite private_configs.env e regenere.
 */
#ifndef PRIVATE_CONFIGS_H
#define PRIVATE_CONFIGS_H

#include <stdint.h>

/*
 * Configuracoes geradas a partir de private_configs.env
 * Variaveis lidas:
"""

FOOTER = """

#endif /* PRIVATE_CONFIGS_H */
"""


def parse_env(filepath: str) -> dict:
    """Le um arquivo .env e retorna dict com as variaveis."""
    config = {}
    if not os.path.exists(filepath):
        print(f"[ERRO] Arquivo nao encontrado: {filepath}")
        sys.exit(1)
    with open(filepath, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            if "=" not in line:
                continue
            key, _, value = line.partition("=")
            key = key.strip()
            value = value.strip().strip('"').strip("'")
            config[key] = value
    return config


def aes_key_to_bytes_array(hex_key: str) -> str:
    """Converte chave hex de 64 chars para array de bytes C."""
    hex_key = hex_key.strip().upper()
    if len(hex_key) != 64:
        print(f"[AVISO] Chave AES tem {len(hex_key)} chars (esperado 64). Usando padrao.")
        hex_key = "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"
    bytes_list = [f"0x{hex_key[i:i+2]}" for i in range(0, 64, 2)]
    return "{" + ", ".join(bytes_list[:8]) + ",           " + \
           ", ".join(bytes_list[8:16]) + ",           " + \
           ", ".join(bytes_list[16:24]) + ",           " + \
           ", ".join(bytes_list[24:32]) + "}"


def generate(config: dict) -> str:
    """Gera o conteudo do arquivo private_configs.h."""
    lines = [HEADER]
    for k in sorted(config.keys()):
        lines.append(f" *   {k}={config[k]}")
    lines.append(" */\n")

    # --- AES Key ---
    aes_hex = config.get("BASTAO_AES_KEY", "")
    aes_array = aes_key_to_bytes_array(aes_hex)
    lines.append("/* --- Chave de Criptografia AES-256 --- */")
    lines.append("#define PRIVATE_AES_KEY " + aes_array)  # no newline in macro
    # We need to format it properly with backslash continuation
    # Let me redo this
    lines = [l for l in lines if not l.startswith("#define PRIVATE_AES_KEY")]
    # Remove the temporary line
    lines = [HEADER]
    for k in sorted(config.keys()):
        lines.append(f" *   {k}={config[k]}")
    lines.append(" */\n")
    
    # Properly format the AES key macro with continuation
    hex_key = aes_hex.strip().upper()
    if len(hex_key) != 64:
        hex_key = "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF"
    bytes_list = [f"0x{hex_key[i:i+2]}" for i in range(0, 64, 2)]
    
    lines.append("/* --- Chave de Criptografia AES-256 --- */")
    lines.append(f"#define PRIVATE_AES_KEY \\")
    lines.append(f"    {{{bytes_list[0]}, {bytes_list[1]}, {bytes_list[2]}, {bytes_list[3]}, {bytes_list[4]}, {bytes_list[5]}, {bytes_list[6]}, {bytes_list[7]}, \\")
    lines.append(f"      {bytes_list[8]}, {bytes_list[9]}, {bytes_list[10]}, {bytes_list[11]}, {bytes_list[12]}, {bytes_list[13]}, {bytes_list[14]}, {bytes_list[15]}, \\")
    lines.append(f"      {bytes_list[16]}, {bytes_list[17]}, {bytes_list[18]}, {bytes_list[19]}, {bytes_list[20]}, {bytes_list[21]}, {bytes_list[22]}, {bytes_list[23]}, \\")
    lines.append(f"      {bytes_list[24]}, {bytes_list[25]}, {bytes_list[26]}, {bytes_list[27]}, {bytes_list[28]}, {bytes_list[29]}, {bytes_list[30]}, {bytes_list[31]}}}")
    lines.append(f'#define PRIVATE_AES_KEY_HEX "{hex_key}"')

    # --- APN Celular ---
    lines.append("\n/* --- Configuracoes da APN Celular (SIMCom) --- */")
    apn = config.get("BASTAO_APN_NAME", "")
    apn_user = config.get("BASTAO_APN_USER", "")
    apn_pass = config.get("BASTAO_APN_PASS", "")
    lines.append(f'#define PRIVATE_APN_NAME "{apn}"')
    lines.append(f'#define PRIVATE_APN_USER "{apn_user}"')
    lines.append(f'#define PRIVATE_APN_PASS "{apn_pass}"')

    # --- MQTT Broker ---
    lines.append("\n/* --- Configuracoes do Broker MQTT (sistemaBastao) --- */")
    mqtt_uri = config.get("BASTAO_MQTT_URI", "mqtt://localhost:1883")
    mqtt_client = config.get("BASTAO_MQTT_CLIENT_ID", "bastao-esp-001")
    topic_tele = config.get("BASTAO_TOPIC_TELEMETRY", "agro/bastao/{MAC}/telemetry").replace("{MAC}", "000000000000")
    topic_gps = config.get("BASTAO_TOPIC_GPS", "agro/bastao/{MAC}/gps").replace("{MAC}", "000000000000")
    lines.append(f'#define PRIVATE_MQTT_URI "{mqtt_uri}"')
    lines.append(f'#define PRIVATE_MQTT_CLIENT_ID "{mqtt_client}"')
    lines.append(f'#define PRIVATE_MQTT_TOPIC_TELE "{topic_tele}"')
    lines.append(f'#define PRIVATE_MQTT_TOPIC_GPS "{topic_gps}"')
    lines.append("/** @brief Credenciais MQTT geradas dinamicamente em main.c */")

    # --- Wi-Fi ---
    lines.append("\n/* --- Configuracoes de Wi-Fi --- */")
    wifi_ssid = config.get("BASTAO_WIFI_SSID", "")
    wifi_pass = config.get("BASTAO_WIFI_PASS", "")
    wifi_enabled = config.get("BASTAO_WIFI_ENABLED", "false")
    lines.append(f'#define PRIVATE_WIFI_SSID "{wifi_ssid}"')
    lines.append(f'#define PRIVATE_WIFI_PASS "{wifi_pass}"')

    # --- Modo de Rede ---
    lines.append("\n/* --- Modo de Rede --- */")
    net_mode = config.get("BASTAO_NET_MODE", "auto")
    mode_map = {"auto": 3, "wifi_only": 0, "cellular_only": 1}
    mode_val = mode_map.get(net_mode, 3)
    lines.append(f"#define PRIVATE_NET_MODE {mode_val}")

    lines.append(FOOTER)
    return "\n".join(lines) + "\n"


def main():
    parser = argparse.ArgumentParser(description="Gera private_configs.h a partir de .env")
    parser.add_argument("--env", default=os.path.join(os.path.dirname(__file__), "private_configs.env"),
                        help="Caminho do arquivo .env (default: private_configs.env)")
    parser.add_argument("--output", default=None,
                        help="Caminho de saida (default: main/private_configs.h)")
    args = parser.parse_args()

    env_path = args.env
    script_dir = os.path.dirname(os.path.abspath(__file__))
    if args.output is None:
        output_path = os.path.join(script_dir, "main", "private_configs.h")
    else:
        output_path = args.output

    print(f"[generate_config] Lendo:  {env_path}")
    config = parse_env(env_path)
    print(f"[generate_config] Variaveis lidas: {len(config)}")
    for k, v in sorted(config.items()):
        print(f"  {k}={v[:50]}{'...' if len(v) > 50 else ''}")

    content = generate(config)
    with open(output_path, "w", encoding="utf-8") as f:
        f.write(content)
    print(f"[generate_config] Gerado: {output_path}")
    print(f"[generate_config] Tamanho: {len(content)} bytes")


if __name__ == "__main__":
    main()

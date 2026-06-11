/**
 * @file private_configs.template.h
 * @brief Template de configuracoes privadas de rede e seguranca.
 * @details NAO edite este arquivo. Edite 'private_configs.env' na raiz
 *          do esp32_firmware e execute 'python generate_config.py' para
 *          gerar 'private_configs.h' automaticamente.
 *
 * Variaveis esperadas no .env:
 *   BASTAO_MQTT_URI       URI do broker MQTT
 *   BASTAO_MQTT_CLIENT_ID Client ID MQTT
 *   BASTAO_WIFI_SSID      SSID Wi-Fi (fallback)
 *   BASTAO_WIFI_PASS      Senha Wi-Fi
 *   BASTAO_WIFI_ENABLED   Habilitar Wi-Fi (true/false)
 *   BASTAO_APN_NAME       APN da operadora 4G
 *   BASTAO_APN_USER       Usuario da APN
 *   BASTAO_APN_PASS       Senha da APN
 *   BASTAO_CELL_ENABLED   Habilitar celular (true/false)
 *   BASTAO_AES_KEY        Chave AES-256 em hex (64 chars)
 *   BASTAO_NET_MODE       auto | wifi_only | cellular_only
 *   BASTAO_TOPIC_TELEMETRY Topico telemetria (fallback)
 *   BASTAO_TOPIC_GPS      Topico GPS (fallback)
 *
 * @note As credenciais MQTT (username e password) sao geradas dinamicamente:
 *       username = MAC do ESP32 (numero_serie)
 *       password = Chave AES em hexadecimal (64 chars)
 */
#ifndef PRIVATE_CONFIGS_H
#define PRIVATE_CONFIGS_H

#include <stdint.h>

/* --- Chave de Criptografia AES-256 --- */
#define PRIVATE_AES_KEY \
    { \
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, \
        0x98, 0x76, 0x54, 0x32, 0x10, 0x10, 0x32, 0x54, 0x76, 0x98, 0xBA, \
        0xDC, 0xFE, 0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01 \
    }

/* --- Configuracoes da APN Celular (SIMCom) --- */
#define PRIVATE_APN_NAME "iot.datatem.com.br"
#define PRIVATE_APN_USER "datatem"
#define PRIVATE_APN_PASS "datatem"

/* --- Configuracoes do Broker MQTT (sistemaBastao) --- */
#define PRIVATE_MQTT_URI "mqtt://209.50.240.55:1883"
#define PRIVATE_MQTT_CLIENT_ID "bastao-esp-001"
#define PRIVATE_MQTT_TOPIC_TELE "agro/bastao/000000000000/telemetry"
#define PRIVATE_MQTT_TOPIC_GPS "agro/bastao/000000000000/gps"

/* --- Configuracoes de Wi-Fi Padrao --- */
#define PRIVATE_WIFI_SSID ""
#define PRIVATE_WIFI_PASS ""

#endif /* PRIVATE_CONFIGS_H */

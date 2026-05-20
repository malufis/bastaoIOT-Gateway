/**
 * @file cmd_parser.h
 * @brief Módulo de parsing de comandos remotos via MQTT
 *
 * Processa comandos recebidos no tópico `bastao/cmd` para controle
 * remoto do hardware (potência RF, leitores, buzzer, etc).
 */
#ifndef CMD_PARSER_H
#define CMD_PARSER_H

#include "esp_err.h"
#include "mqtt_publisher.h"

esp_err_t cmd_parser_init(void);
void cmd_parser_process_message(const char *topic, const char *payload);

#endif // CMD_PARSER_H
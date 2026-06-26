#ifndef RFID_PARSER_H
#define RFID_PARSER_H

#include <stdint.h>

#define RFID_BUFFER_SIZE 512

typedef struct {
    uint8_t raw_data[RFID_BUFFER_SIZE];
    uint16_t head;
    uint16_t tail;
} RFID_Buffer_t;

void RFID_Init(void);
void RFID_StoreByte(uint8_t byte, uint8_t is_yrm100);
uint8_t RFID_HasData(uint8_t is_yrm100);

void RFID_Process_YRM100(void);
void RFID_Process_WL134(void);

/**
 * @brief Inicializa o modulo YRM100 com configuracao de modo, regiao e potencia.
 *
 * Sequencia:
 *   1. Set Mode -> 0xF5
 *   2. Set Region -> 0x02 (US/America, 902-928 MHz)
 *   3. Set TX Power -> 20 dBm
 *   4. Save Config -> 0x09
 *   5. Start Continuous Read -> 0x27
 *
 * Deve ser chamada apos power-on do modulo e HAL_Delay(100).
 */
void YRM100_Init(void);

/**
 * @brief Configura o modo de operacao do YRM100 (0xF5).
 */
void YRM100_SetMode(void);

/**
 * @brief Configura a regiao de frequencia do YRM100.
 * @param region 0x01=China900, 0x02=USA, 0x03=Europe, 0x04=China800
 */
void YRM100_SetRegion(uint8_t region);

/**
 * @brief Define a potencia de transmissao RF do YRM100.
 * @param dbm Potencia em dBm (ex: 20 = 20.00 dBm).
 *            Será convertido para centesimos de dBm internamente.
 */
void YRM100_SetTXPower(uint8_t dbm);

/**
 * @brief Salva a configuracao atual na flash do modulo (0x09).
 */
void YRM100_SaveConfig(void);

/**
 * @brief Inicia o inventario continuo (0x27).
 *        O YRM100 fica varrendo tags continuamente e envia notificacoes.
 *        Nao precisa de polling periodico.
 */
void YRM100_StartContinuousRead(void);

/**
 * @brief Para o inventario continuo (0x28).
 */
void YRM100_StopContinuousRead(void);

/**
 * @brief Poll unico do YRM100 (comando 0x22 single inventory).
 * Chamado a cada 100ms no loop principal.
 */
void YRM100_SinglePoll(void);

/**
 * @brief Le a versao do firmware do modulo YRM100 (comando 0x26).
 * @param buf Buffer para string da versao (ex: "2.1.3")
 * @param max_len Tamanho maximo do buffer (incluindo null terminator)
 */
void YRM100_ReadFirmwareVersion(char *buf, uint8_t max_len);

/**
 * @brief Le a regiao de frequencia configurada no YRM100 (comando 0x07 read).
 * @return 0x01=China900, 0x02=USA, 0x03=Europe, 0x04=China800, 0xFF=erro
 */
uint8_t YRM100_ReadRegion(void);

/**
 * @brief Le a potencia TX configurada no YRM100 (comando 0xB6 read).
 * @return Potencia em dBm, 0xFF=erro
 */
uint8_t YRM100_ReadTXPower(void);

/**
 * @brief Le todas as configuracoes do YRM100 e envia um JSON
 *        para o ESP32 via UART (huart2) com o status.
 *
 * JSON format: {"type":"yrm100_cfg","fw":"x.x","region":2,"power":20,"mode":1}
 * Enviado apos YRM100_Init() para debug.
 */
void YRM100_ReportConfig(void);

/**
 * @brief Esvazia o buffer YRM100 e retorna quantos bytes foram descartados.
 * Util para teste: descarta dados acumulados antes de comecar novo ciclo.
 */
void YRM100_FlushBuffer(void);

/**
 * @brief Le bytes brutos do buffer YRM100 sem interpretar frames.
 * @param buf Buffer de saida
 * @param max_len Tamanho maximo
 * @return Numero de bytes copiados
 */
uint16_t YRM100_ReadRawBuffer(uint8_t *buf, uint16_t max_len);

#endif

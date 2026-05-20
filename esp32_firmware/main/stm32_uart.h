/**
 * @file stm32_uart.h
 * @brief Cabeçalho do módulo de recepção UART para comunicação com o microcontrolador STM32.
 * @details Este módulo é responsável por definir os tipos de dados recebidos do STM32,
 *          definir a pinagem e parâmetros da interface serial UART1 do ESP32 e declarar
 *          as rotinas públicas para inicialização do driver e ativação da task de leitura.
 * 
 * @author Antigravity Agent
 * @date 2026-05-19
 */

#ifndef STM32_UART_H
#define STM32_UART_H

#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --- Definições de Configuração Física --- */

/** @brief Porta UART utilizada para comunicação com o STM32. */
#define STM32_UART_PORT      UART_NUM_1

/** 
 * @brief Pino GPIO do ESP32 conectado ao transmissor TX (PA2) do STM32.
 * @note Ligado fisicamente como RX no ESP32 (Linha: STM32_TX-ESP).
 */
#define STM32_UART_RX_PIN    GPIO_NUM_13

/** 
 * @brief Pino GPIO do ESP32 conectado ao receptor RX (PA3) do STM32.
 * @note Ligado fisicamente como TX no ESP32 (Linha: STM32_RX-ESP).
 */
#define STM32_UART_TX_PIN    GPIO_NUM_14

/** @brief Velocidade da comunicação serial (Baud Rate). */
#define STM32_UART_BAUD_RATE 115200

/** @brief Tamanho do buffer de recepção serial em bytes. */
#define STM32_UART_BUF_SIZE  1024

/* --- Estruturas de Dados e Tipos --- */

/**
 * @brief Enumeração representando os tipos de dados reportados pelo STM32.
 */
typedef enum {
    DATA_TYPE_RFID,     /**< Leitura física de tag RFID (LF ou UHF) */
    DATA_TYPE_BATTERY,  /**< Medição de telemetria analógica da bateria */
    DATA_TYPE_ACCEL,    /**< Dados do acelerômetro da K10 */
    DATA_TYPE_RFID_WITH_ACCEL /**< RFID com dados de movimento */
} data_type_t;

/**
 * @brief Estrutura contendo o payload decodificado e validado do STM32.
 */
typedef struct {
    data_type_t type;
    char model[16];
    char tag[64];
    float battery_v;
    float accel_x;
    float accel_y;
    float accel_z;
    uint8_t movement;
} stm32_data_t;

/* --- Variáveis Globais Compartilhadas --- */

/**
 * @brief Handle global da fila de dados recebidos do STM32.
 * @details Outras partes do sistema (como a task de criptografia ou rede BLE Mesh) consomem
 *          itens do tipo stm32_data_t inseridos nesta fila.
 */
extern QueueHandle_t stm32_data_queue;

/* --- Funções de Interface Pública --- */

/**
 * @brief Inicializa a porta serial UART1 e instala o driver correspondente.
 * @details Configura a baud rate, tamanho de palavra, stop bits, paridade e os pinos
 *          físicos do ESP32 cruzando com as linhas de transmissão do STM32.
 * 
 * @pre O hardware do ESP32 deve estar alimentado e com pinos IO13/IO14 livres de bootstrapping.
 * @post O driver de UART1 é registrado no sistema operacional e o buffer de hardware é alocado.
 * 
 * @return esp_err_t Código de erro clássico do ESP-IDF.
 *         - ESP_OK: Inicialização realizada com sucesso.
 *         - ESP_FAIL ou outros códigos: Falha na alocação de recursos ou na configuração dos pinos.
 */
esp_err_t stm32_uart_init(void);

/**
 * @brief Cria e inicia a tarefa assíncrona do FreeRTOS encarregada de ler a UART.
 * @details Instancia a task de leitura em background, que escutará a UART continuamente,
 *          decodificará strings em formato JSON terminadas por '\n' e alimentará a fila global.
 *
 * @pre A fila stm32_data_queue e o driver de UART já devem estar previamente inicializados.
 *
 * @param[in] priority Prioridade de execução da task FreeRTOS. Recomenda-se prioridade intermediária (ex: 5).
 *
 * @return BaseType_t Retorna pdPASS se a criação da task foi bem sucedida, ou código de erro do FreeRTOS.
 */
BaseType_t stm32_uart_rx_task_start(UBaseType_t priority);

/**
 * @brief Envia uma string de comando para o STM32 via UART.
 * @details Envia dados serialmente para o microcontrolador STM32.
 *
 * @param[in] str String terminada em nulo a ser enviada.
 * @return esp_err_t ESP_OK em caso de sucesso.
 */
esp_err_t stm32_uart_send_string(const char *str);

#ifdef __cplusplus
}
#endif

#endif /* STM32_UART_H */

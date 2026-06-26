/**
 * @file stm32_uart.h
 * @brief Cabecalho do modulo de recepcao UART para comunicacao com o microcontrolador STM32.
 * @details Este modulo e responsavel por definir os tipos de dados recebidos do STM32,
 *          definir a pinagem e parametros da interface serial UART1 do ESP32 e declarar
 *          as rotinas publicas para inicializacao do driver e ativacao da task de leitura.
 * 
 * @author Antigravity Agent
 * @date 2026-05-19
 */

#ifndef STM32_UART_H
#define STM32_UART_H

#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#ifdef __cplusplus
extern "C" {
#endif

/* --- Definicoes de Configuracao Fisica --- */

/** @brief Porta UART utilizada para comunicacao com o STM32. */
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

/** @brief Velocidade da comunicacao serial (Baud Rate). */
#define STM32_UART_BAUD_RATE 115200

/** @brief Tamanho do buffer de recepcao serial em bytes. */
#define STM32_UART_BUF_SIZE  1024

/** @brief Pino GPIO do ESP32 usado para reset do STM32 via transistor NPN (lógica invertida).
 * 
 *  @warning A lógica é INVERTIDA por causa do transistor:
 *           GPIO19 HIGH → transistor ON  → NRST LOW  → STM32 em reset
 *           GPIO19 LOW  → transistor OFF → NRST HIGH → STM32 roda
 * 
 *  @note Configurado como Push-Pull (sem pull-up) para acionar o transistor corretamente.
 *        O NRST do STM32 tem pull-up interno e externo que mantém HIGH quando o transistor OFF. */
#define STM32_RESET_PIN      GPIO_NUM_19

/* --- Estruturas de Dados e Tipos --- */

/**
 * @brief Enumeracao representando os tipos de dados reportados pelo STM32.
 */
typedef enum {
    DATA_TYPE_RFID,     /**< Leitura fisica de tag RFID (LF ou UHF) */
    DATA_TYPE_BATTERY,  /**< Medicao de telemetria analogica da bateria */
    DATA_TYPE_ACCEL,    /**< Dados do acelerometro da K10 */
    DATA_TYPE_RFID_WITH_ACCEL, /**< RFID com dados de movimento */
    DATA_TYPE_ALERT,    /**< Alerta do STM32 (bateria critica/baixa) */
    DATA_TYPE_HEARTBEAT, /**< Heartbeat periodico do STM32 */
    DATA_TYPE_POWER     /**< Reservado (nao utilizado) */
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
    char alert_code[24];
} stm32_data_t;

/* --- Variaveis Globais Compartilhadas --- */

/**
 * @brief Handle global da fila de dados recebidos do STM32.
 * @details Outras partes do sistema (como a task de criptografia ou rede BLE Mesh) consomem
 *          itens do tipo stm32_data_t inseridos nesta fila.
 */
extern QueueHandle_t stm32_data_queue;

/* --- Funcoes de Interface Publica --- */

/**
 * @brief Inicializa a porta serial UART1 e instala o driver correspondente.
 * @details Configura a baud rate, tamanho de palavra, stop bits, paridade e os pinos
 *          fisicos do ESP32 cruzando com as linhas de transmissao do STM32.
 * 
 * @pre O hardware do ESP32 deve estar alimentado e com pinos IO13/IO14 livres de bootstrapping.
 * @post O driver de UART1 e registrado no sistema operacional e o buffer de hardware e alocado.
 * 
 * @return esp_err_t Codigo de erro classico do ESP-IDF.
 *         - ESP_OK: Inicializacao realizada com sucesso.
 *         - ESP_FAIL ou outros codigos: Falha na alocacao de recursos ou na configuracao dos pinos.
 */
esp_err_t stm32_uart_init(void);

/**
 * @brief Cria e inicia a tarefa assincrona do FreeRTOS encarregada de ler a UART.
 * @details Instancia a task de leitura em background, que escutara a UART continuamente,
 *          decodificara strings em formato JSON terminadas por '\n' e alimentara a fila global.
 *
 * @pre A fila stm32_data_queue e o driver de UART ja devem estar previamente inicializados.
 *
 * @param[in] priority Prioridade de execucao da task FreeRTOS. Recomenda-se prioridade intermediaria (ex: 5).
 *
 * @return BaseType_t Retorna pdPASS se a criacao da task foi bem sucedida, ou codigo de erro do FreeRTOS.
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

/**
 * @brief Verifica se o STM32 esta vivo (heartbeat recebido nos ultimos 60s).
 * @return 1 se STM32 responsivo, 0 se timeout.
 */
uint8_t stm32_uart_is_stm32_alive(void);

/**
 * @brief Verifica se o STM32 ja enviou ao menos um heartbeat desde o boot.
 * @return 1 se ja recebeu heartbeat, 0 se nunca.
 */
uint8_t stm32_uart_has_ever_been_alive(void);

/**
 * @brief Reseta o watchdog do STM32.
 */
void stm32_uart_watchdog_reset(void);

/**
 * @brief Configura o pino GPIO19 como Push-Pull para controle de reset do STM32.
 * @details O GPIO19 aciona um transistor NPN que controla o NRST do STM32.
 *          A logica e invertida: nivel LOW mantem o STM32 rodando,
 *          nivel HIGH poe o STM32 em reset.
 *          
 * @note O pino e inicializado em LOW para que o STM32 saia do reset e rode.
 * @return esp_err_t ESP_OK se configurado com sucesso.
 */
esp_err_t stm32_uart_reset_init(void);

/**
 * @brief Gera pulso de reset no STM32 via transistor.
 * @details Sequencia: GPIO19 HIGH (transistor ON, NRST LOW) por 100ms,
 *          depois GPIO19 LOW (transistor OFF, NRST HIGH).
 *          O STM32 e reiniciado apos o pulso.
 * 
 * @warning A logica e invertida pelo transistor NPN:
 *          - HIGH = reset (NRST LOW)
 *          - LOW  = operacao normal (NRST HIGH)
 */
void stm32_uart_reset_stm32(void);

#ifdef __cplusplus
}
#endif

#endif /* STM32_UART_H */

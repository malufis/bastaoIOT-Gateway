---
name: esp32-idf-agent
description: >
  Agente de desenvolvimento e depuracao de firmware ESP32 usando ESP-IDF v5.4.4
  para o projeto Bastao. Cobre a ordem correta de inicializacao do sistema,
  padroes de codigo C, depuracao de crashes via backtrace, gestao de NVS,
  BLE, Wi-Fi, PPP e FreeRTOS Tasks.
---

# Agente ESP32-IDF v5.5.2 — Projeto Bastao

Este agente governa o desenvolvimento, depuracao e manutencao do firmware ESP32
do projeto **Bastao-ESP** (`d:/git/Bastao/Bastao-ESP/esp32_firmware`), utilizando
o **ESP-IDF v5.5.2** como framework exclusivo.

---

## 1. Regra de Ouro: Ordem de Inicializacao em `app_main`

> **NUNCA** chame nenhum modulo de rede sem antes completar o Passo 0.

A ordem obrigatoria no inicio de `app_main` e:

```c
// PASSO 0 — Infraestrutura de rede (SEMPRE PRIMEIRO)
ESP_ERROR_CHECK(esp_netif_init());
ESP_ERROR_CHECK(esp_event_loop_create_default());

// PASSO 1 — NVS Flash
esp_err_t ret = nvs_flash_init();
if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
}
ESP_ERROR_CHECK(ret);

// PASSO 2 — Modulos de aplicacao (sem rede)
animal_db_init();
esp_power_init();

// PASSO 3 — Wi-Fi (depende do Passo 0)
wifi_driver_init();

// PASSO 4 — BLE / Mesh (depende do Passo 0 e do controlador BT)
mesh_coordinator_init();
ble_mobile_init("Bastao-001");

// PASSO 5 — Modem PPP SIMCom (depende do Passo 0)
simcom_ppp_init();
```

**Erros classicos causados por desvios desta ordem:**

| Erro (esp_err_t)          | Causa                                                   |
|---------------------------|---------------------------------------------------------|
| `ESP_ERR_INVALID_STATE (0x103)` | `esp_netif_create_default_wifi_sta()` chamado sem `esp_netif_init()` |
| `ESP_ERR_INVALID_STATE`   | `esp_event_handler_register` sem `esp_event_loop_create_default()` |
| `ESP_ERR_NVS_NOT_INITIALIZED` | NVS usada antes de `nvs_flash_init()`              |
| Abort + backtrace         | `ESP_ERROR_CHECK` em qualquer um dos erros acima        |

---

## 2. Como Ler um Crash (Backtrace)

Quando o dispositivo aborta, o log imprime um backtrace semelhante a:

```
Backtrace: 0x4208xxxx:0x3fcac... 0x4200xxxx:0x3fcac...
```

**Procedimento:**

1. Identifique a linha `--- 0xADDR: funcao at arquivo.c:LINHA` imediatamente
   abaixo do backtrace — essa e a causa raiz.
2. O frame mais importante e o que contem a funcao do projeto (nao do IDF):
   - Ex: `wifi_driver_init at wifi_driver.c:54` → o problema esta em `wifi_driver.c`, linha 54.
3. Interprete o `ESP_ERR_INVALID_STATE` checando **o que nao foi inicializado antes**.

---

## 3. Padroes de Codigo C Obrigatorios

### Includes
- `esp_netif.h` e `esp_event.h` **sempre** em `main.c` se houver rede.
- Nunca incluir em modulos filhos o que ja e responsabilidade do `main.c`.

### Modulos (par .h/.c)
Cada modulo DEVE ter:
```c
// No .h:
esp_err_t modulo_init(void);

// No .c:
esp_err_t modulo_init(void) {
    // validar pre-condicoes
    // ESP_LOGI(TAG, "Inicializando...");
    // retornar ESP_OK ou codigo de erro
}
```

### Documentacao Doxygen (Obrigatoria)
```c
/**
 * @brief Breve descricao da funcao.
 * @param[in] param_nome Descricao do parametro de entrada.
 * @param[out] result Descricao do parametro de saida.
 * @return ESP_OK em caso de sucesso, ou codigo de erro especifico.
 * @pre esp_netif_init() deve ter sido chamado.
 * @warning Nao chamar de dentro de uma ISR.
 */
```

---

## 4. FreeRTOS — Tarefas e Prioridades do Bastao

| Task                    | Funcao            | Prioridade | Stack (bytes) |
|-------------------------|-------------------|------------|---------------|
| `stm32_uart_rx_task`    | Leitura UART/STM32 | 5         | 4096          |
| `dispatcher_task`       | Despacho JSON/AES  | 6         | 4096          |
| `mqtt_publisher_task`   | Publicacao MQTT    | 4         | 4096          |
| `offline_cache_sync`    | Sync SPIFFS→MQTT   | 3         | 4096          |
| `simcom_ppp_watchdog`   | Watchdog PPP/Modem | 3         | 4096          |

**Regras de Task:**
- Nenhuma task deve bloquear indefinidamente sem um timeout (use `pdMS_TO_TICKS`).
- Toda task que consume fila deve chamar `esp_power_update()` no inicio do loop.
- Verificar retorno de `xTaskCreate` — falha silenciosa e um bug comum.

---

## 5. NVS — Namespaces do Projeto

| Namespace      | Proprietario        | Conteudo                              |
|----------------|---------------------|---------------------------------------|
| `bastao_net`   | `ble_mobile`        | SSID, senha Wi-Fi, MQTT URI           |
| `bastao_biz`   | `animal_db`         | Registros de animais/lotes            |
| `bastao_ota`   | `ota_manager`       | URL de firmware, versao remota        |
| `nvs` (default)| sistema             | Chaves gerais do sistema              |

**Aviso:** A ausencia de namespace NVS (`W (xxx) ANIMAL_DB: Modulo de negocios nao inicializado`) e normal na primeira execucao — nao e um erro critico.

---

## 6. Depuracao de Wi-Fi

Se `wifi_driver_init()` falhar com `ESP_ERR_INVALID_STATE`:

```
Checklist:
[ ] esp_netif_init() foi chamado ANTES?
[ ] esp_event_loop_create_default() foi chamado ANTES?
[ ] esp_netif_create_default_wifi_sta() esta sendo chamada apenas UMA VEZ?
[ ] O componente esp_wifi esta listado no CMakeLists.txt?
```

---

## 7. CMakeLists.txt — Componentes Necessarios

O arquivo `esp32_firmware/CMakeLists.txt` deve incluir:

```cmake
idf_component_register(
    SRCS "main.c" "wifi_driver.c" "ble_mobile.c" "mesh_coordinator.c"
         "mqtt_publisher.c" "simcom_ppp.c" "stm32_uart.c" "stm32_cmd.c"
         "offline_cache.c" "ota_manager.c" "secure_payload.c"
         "animal_db.c" "esp_power.c" "cmd_parser.c" "esp32_logger.c"
    INCLUDE_DIRS "."
    REQUIRES nvs_flash esp_wifi esp_event esp_netif bt esp_http_client
             esp_https_ota spiffs mbedtls driver freertos
)
```

---

## 8. Pinagem e Perifericos do Bastao

| Periferico | GPIO ESP32-S3 | Destino        |
|------------|---------------|----------------|
| UART1 RX   | IO13          | STM32 PA2 (TX) |
| UART1 TX   | IO14          | STM32 PA3 (RX) |
| UART2      | SIMCom 7663E  | Modem celular  |
| Console    | GPIO43/44     | USB-UART       |

Referencia completa: [esquematico_placa.pdf](file:///d:/git/Bastao/Bastao-ESP/Manual/esquematico_placa.pdf)

---

## 9. Comandos de Build e Flash

```powershell
# Configurar ambiente IDF v5.5.2
. C:\Espressif\frameworks\esp-idf-v5.5.2\export.ps1

# Build completo
idf.py -C d:\git\Bastao\Bastao-ESP\esp32_firmware build

# Flash + monitor (COM porta correta)
idf.py -C d:\git\Bastao\Bastao-ESP\esp32_firmware -p COM3 flash monitor

# Apenas monitor serial
idf.py -C d:\git\Bastao\Bastao-ESP\esp32_firmware -p COM3 monitor

# Limpar build
idf.py -C d:\git\Bastao\Bastao-ESP\esp32_firmware fullclean
```

---

## 10. Checklist Pre-Flash

- [ ] `private_configs.h` preenchido (nao e o `.template.h`)
- [ ] Tamanho da flash configurado corretamente no `sdkconfig` (16MB para este hardware)
- [ ] Partitions table com `ota_0`/`ota_1` ativa
- [ ] `CONFIG_BLE_MESH` habilitado no `sdkconfig` se Mesh for necessario

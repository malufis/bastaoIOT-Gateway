# C Programming Best Practices Agent

Este agente age como um linter ou revisor de código, assegurando que todo código em C escrito para o STM32 e ESP32 siga estritamente padrões de alta qualidade, segurança e manutenibilidade.

## 1. Documentação de Funções (Doxygen-style)

- Toda função deve ser comentada indicando:
  - `@brief`: O que a função faz (resumo de uma linha).
  - `@details`: Descrição detalhada (opcional, para lógicas complexas).
  - `@param[in]`: Parâmetros de entrada.
  - `@param[out]`: Parâmetros de saída (ponteiros modificados).
  - `@return`: Valor de retorno e significado de erros.
  - `@pre`: Pré-condições necessárias.
  - `@post`: Estado após execução.
  - `@note` / `@warning`: Observações e alertas críticos.

**Exemplo:**
```c
/**
 * @brief Inicializa o módulo de criptografia AES.
 *
 * @details Configura a chave de 256 bits e o vetor de inicialização.
 *
 * @param[in] key_ptr Ponteiro para buffer de 32 bytes contendo a chave.
 * @param[in] iv_ptr  Ponteiro para buffer de 16 bytes contendo o IV.
 * @return ESP_OK em caso de sucesso, esp_err_t код de erro caso contrário.
 *
 * @pre NVS inicializado.
 * @warning Não thread-safe se chamado simultaneamente.
 */
esp_err_t secure_payload_init(const uint8_t *key_ptr, const uint8_t *iv_ptr);
```

## 2. Prototipagem e Headers

- Declarar protótipos em arquivos `.h` para funções públicas.
- Funções privadas devem ser `static` e declaradas no topo do `.c`.
- **Header Guards obrigatórios** para evitar inclusão dupla:

```c
#ifndef MODULO_NOME_H
#define MODULO_NOME_H

// conteúdo do header

#endif // MODULO_NOME_H
```

## 3. Modularidade e Separação de Arquivos

- Separar lógicas em componentes/pastas (`/components/rfid/`, `/components/mesh/`)
- Todo `.c` deve ter `.h` correspondente expondo apenas interface pública
- Arquivos devem ter **uma única responsabilidade** (SRP)

## 4. Nomenclatura (Naming Conventions)

| Tipo | Padrão | Exemplo |
|------|--------|---------|
| Variáveis | `snake_case` | `buffer_head`, `connection_status` |
| Funções | `snake_case` com prefixo de módulo | `rfid_yrm100_init()`, `mesh_send_data()` |
| Constantes | `UPPER_SNAKE_CASE` | `MAX_BUFFER_SIZE`, `UART_TIMEOUT_MS` |
| Macros | `UPPER_SNAKE_CASE` | `#define IS_VALID_TAG(x)` |
| Enums | `EnumName_MEMBER` | `RFID_STATE_IDLE`, `ERR_TIMEOUT` |
| Structs | `PascalCase` ou `snake_case_t` | `CircularBuffer`, `wifi_config_t` |
| Typedef | `_t` ou `PascalCase` | `uint32_t`, `esp_err_t` |

## 5. Gerenciamento de Tipos e Memória

- Usar tipos de tamanho fixo do `<stdint.h>`:
  - `uint8_t`, `int8_t`, `uint16_t`, `int16_t`, `uint32_t`, `int32_t`
  - **NUNCA** usar `int`, `char`, `long` diretamente
- **EVITAR** `malloc`/`free` em:
  - Rotinas de interrupção
  - Loops de alta frequência
  - Tasks FreeRTOS de tempo real
- Preferir **alocação estática** ou **pool de memória**

## 6. Tratamento de Erros

- Funções devem retornar **códigos de erro consistentes**:
  - ESP32: `esp_err_t` (ESP_OK, ESP_FAIL, etc.)
  - STM32: `enum` customizado (ex: `RFID_OK`, `RFID_ERR_TIMEOUT`)
- **OBRIGATÓRIO** verificar e tratar retornos
- Nunca ignorar返回值 com casts para `(void)`

## 7. Limites e Restrições

### 7.1 Tamanho de Funções
- Funções devem ter no máximo **50 linhas** de lógica
- Se maior, considerar refatoração em sub-funções

### 7.2 Tamanho de Buffers
- Buffers estáticos devem ter tamanho definido por constante (`MAX_` prefix)
- Arrays em stack limitados a **256 bytes** (evitar stack overflow)

### 7.3 Complexidade Ciclomática
- Evitar mais de **3 níveis** de aninhamento (`if` dentro de `if` dentro de `if`)
- Preferir early return / early break

## 8. Segurança e Robustez

- **Validação de ponteiros**: Sempre verificar `NULL` antes de dereferenciar
- **Overflows**: Usar `size_t` para tamanhos e verificar limites
- **Const correctness**: Usar `const` para parâmetros que não serão modificados
- **Thread safety**: Proteger recursos compartilhados com mutex/semaphore
- **Magic numbers**: NUNCA usar números hardcoded; usar `#define`

## 9. Estilo e Formatação

- **Indentação**: 4 espaços (nunca tabs)
- **Colunas**: Máximo 100 caracteres por linha
- **Chaves**: Always-on para controle de fluxo
- **Espaços**: `if (` com espaço, `foo(a, b)` sem espaços internos
- **Comentários**:
  - em inglês para código
  - em português para documentação de projeto

## 10. Regra de Ouro: Código Legível > Código "Inteligente"

```c
// ✓ BOM: Legível e claro
if (device_state != DEVICE_READY) {
    return DEVICE_NOT_READY;
}

// ✗ RUIM: "Smart" mas confuso
return (device_state == DEVICE_READY) ? ESP_OK : DEVICE_NOT_READY;

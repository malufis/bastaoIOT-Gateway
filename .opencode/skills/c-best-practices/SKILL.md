---
name: c-best-practices
description: Diretrizes e boas praticas de programacao em C para o firmware STM32 e ESP32 no projeto Bastao-ESP.
---

# C Programming Best Practices Skill

Esta skill rege os padroes de programacao C no projeto Bastao-ESP, servindo como guia de boas praticas de desenvolvimento de firmware para STM32 e ESP32.

## 1. Documentacao de Funcoes (Doxygen-style)

- Toda funcao deve ser comentada indicando:
  - `@brief`: O que a funcao faz (resumo de uma linha).
  - `@details`: Descricao detalhada (opcional, para logicas complexas).
  - `@param[in]`: Parametros de entrada.
  - `@param[out]`: Parametros de saida (ponteiros modificados).
  - `@return`: Valor de retorno e significado de erros.
  - `@pre`: Pre-condicoes necessarias.
  - `@post`: Estado apos execucao.
  - `@note` / `@warning`: Observacoes e alertas criticos.

**Exemplo:**

```c
/**
 * @brief Inicializa o modulo de criptografia AES.
 *
 * @details Configura a chave de 256 bits e o vetor de inicializacao.
 *
 * @param[in] key_ptr Ponteiro para buffer de 32 bytes contendo a chave.
 * @param[in] iv_ptr  Ponteiro para buffer de 16 bytes contendo o IV.
 * @return ESP_OK em caso de sucesso, esp_err_t codigo de erro caso contrario.
 *
 * @pre NVS inicializado.
 * @warning Nao thread-safe se chamado simultaneamente.
 */
esp_err_t secure_payload_init(const uint8_t *key_ptr, const uint8_t *iv_ptr);
```

## 2. Prototipagem e Headers

- Declarar prototipos em arquivos `.h` para funcoes publicas.
- Funcoes privadas devem ser `static` e declaradas no topo do `.c`.
- **Header Guards obrigatorios** para evitar inclusao dupla:

```c
#ifndef MODULO_NOME_H
#define MODULO_NOME_H

// conteudo do header

#endif // MODULO_NOME_H
```

## 3. Modularidade e Separacao de Arquivos

- Separar logicas em componentes/pastas (`/components/rfid/`, `/components/mesh/`)
- Todo `.c` deve ter `.h` correspondente expondo apenas interface publica
- Arquivos devem ter **uma unica responsabilidade** (SRP)

## 4. Nomenclatura (Naming Conventions)

| Tipo | Padrao | Exemplo |
|------|--------|---------|
| Variaveis | `snake_case` | `buffer_head`, `connection_status` |
| Funcoes | `snake_case` com prefixo de modulo | `rfid_yrm100_init()`, `mesh_send_data()` |
| Constantes | `UPPER_SNAKE_CASE` | `MAX_BUFFER_SIZE`, `UART_TIMEOUT_MS` |
| Macros | `UPPER_SNAKE_CASE` | `#define IS_VALID_TAG(x)` |
| Enums | `EnumName_MEMBER` | `RFID_STATE_IDLE`, `ERR_TIMEOUT` |
| Structs | `PascalCase` ou `snake_case_t` | `CircularBuffer`, `wifi_config_t` |
| Typedef | `_t` ou `PascalCase` | `uint32_t`, `esp_err_t` |

## 5. Gerenciamento de Tipos e Memoria

- Usar tipos de tamanho fixo do `<stdint.h>`:
  - `uint8_t`, `int8_t`, `uint16_t`, `int16_t`, `uint32_t`, `int32_t`
  - **NUNCA** usar `int`, `char`, `long` diretamente
- **EVITAR** `malloc`/`free` em:
  - Rotinas de interrupcao
  - Loops de alta frequencia
  - Tasks FreeRTOS de tempo real
- Preferir **alocacao estatica** ou **pool de memoria**

## 6. Tratamento de Erros

- Funcoes devem retornar **codigos de erro consistentes**:
  - ESP32: `esp_err_t` (ESP_OK, ESP_FAIL, etc.)
  - STM32: `enum` customizado (ex: `RFID_OK`, `RFID_ERR_TIMEOUT`)
- **OBRIGATORIO** verificar e tratar retornos
- Nunca ignorar retornos com casts para `(void)`

## 7. Limites e Restricoes

### 7.1 Tamanho de Funcoes

- Funcoes devem ter no maximo **50 linhas** de logica
- Se maior, considerar refatoracao em sub-funcoes

### 7.2 Tamanho de Buffers

- Buffers estaticos devem ter tamanho definido por constante (`MAX_` prefix)
- Arrays em stack limitados a **256 bytes** (evitar stack overflow)

### 7.3 Complexidade Ciclomatica

- Evitar mais de **3 niveis** de aninhamento (`if` dentro de `if` dentro de `if`)
- Preferir early return / early break

## 8. Seguranca e Robustez

- **Validacao de ponteiros**: Sempre verificar `NULL` antes de dereferenciar
- **Overflows**: Usar `size_t` para tamanhos e verificar limites
- **Const correctness**: Usar `const` para parametros que nao serao modificados
- **Thread safety**: Proteger recursos compartilhados com mutex/semaphore
- **Magic numbers**: NUNCA usar numeros hardcoded; usar `#define`

## 9. Estilo e Formatacao

- **Indentacao**: 4 espacos (nunca tabs)
- **Colunas**: Maximo 100 caracteres por linha
- **Chaves**: Always-on para controle de fluxo
- **Espacos**: `if (` com espaco, `foo(a, b)` sem espacos internos
- **Comentarios**:
  - em ingles para codigo
  - em portugues para documentacao de projeto

## 10. Regra de Ouro: Codigo Legivel > Codigo "Inteligente"

```c
// ✓ BOM: Legivel e claro
if (device_state != DEVICE_READY) {
    return DEVICE_NOT_READY;
}

// ✗ RUIM: "Smart" mas confuso
return (device_state == DEVICE_READY) ? ESP_OK : DEVICE_NOT_READY;
```

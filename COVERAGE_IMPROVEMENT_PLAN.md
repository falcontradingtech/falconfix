# 📋 Plano de Ação para Melhoria de Code Coverage

**Data**: 2026-05-09  
**Cobertura Atual**: 28% (61.703 de 219.701 linhas)  
**Objetivo**: Melhorar cobertura de componentes críticos não-engine para **>80%**

---

## 📍 Convenção de Arquivos de Teste

Padrão utilizado no projeto:
```
src/cpp/socket/boost/boost_tcp_socket.cpp
	↓↓ TESTES ↓↓
tests/socket/boost/boost_tcp_socket_tests.cpp
```

---

## 🔴 CRÍTICAS - Prioridade 1 (< 50%)

### 1. **error_codes.h** - 34%

**Localização**:
- Fonte: `src/include/utils/error_codes.h`
- Testes: `tests/utils/error_codes_tests.cpp` (**CRIAR**)

**Problema**: Arquivo de constantes praticamente sem testes

**Plano de Ação**:
```cpp
// tests/utils/error_codes_tests.cpp (CRIAR)
#include <gtest/gtest.h>
#include "utils/error_codes.h"

class ErrorCodesTests : public ::testing::Test {
protected:
	// Setup fixtures se necessário
};

TEST_F(ErrorCodesTests, AllErrorCodesAreDefined) {
	// Verificar se todas as constantes estão definidas
}

TEST_F(ErrorCodesTests, ErrorCodesAreUnique) {
	// Garantir que não há duplicatas de códigos
}

TEST_F(ErrorCodesTests, ErrorCodesHaveValidValues) {
	// Testar range válido de valores
}

TEST_F(ErrorCodesTests, ErrorCodesStringRepresentations) {
	// Testar conversão para string (se houver)
}

TEST_F(ErrorCodesTests, ErrorCodesMapping) {
	// Testar mapeamento entre códigos e mensagens
}
```

**Casos de Teste a Cobrir**:
- ✅ Definição de cada constante de erro
- ✅ Valores numéricos corretos
- ✅ Ausência de conflitos/duplicatas
- ✅ Conversão para string (se aplicável)
- ✅ Mapeamento correto com mensagens de erro

---

### 2. **null_message_store.h** - 33%

**Localização**:
- Fonte: `src/include/store/null_message_store.h`
- Testes: `tests/store/null_message_store_tests.cpp` (**CRIAR/EXPANDIR**)

**Problema**: Implementação "null" (padrão Null Object) sem testes

**Plano de Ação**:
```cpp
// tests/store/null_message_store_tests.cpp (EXPANDIR)
#include <gtest/gtest.h>
#include "store/null_message_store.h"

class NullMessageStoreTests : public ::testing::Test {
protected:
	NullMessageStore store;
};

TEST_F(NullMessageStoreTests, ConstructorDoesNotThrow) {
	EXPECT_NO_THROW(NullMessageStore());
}

TEST_F(NullMessageStoreTests, StoreMessageDoesNothing) {
	// Verificar que não lança exception
	EXPECT_NO_THROW(store.store("key", "message"));
}

TEST_F(NullMessageStoreTests, RetrieveMessageReturnsEmpty) {
	auto result = store.retrieve("key");
	EXPECT_TRUE(result.empty());
}

TEST_F(NullMessageStoreTests, ClearDoesNothing) {
	EXPECT_NO_THROW(store.clear());
}

TEST_F(NullMessageStoreTests, SizeAlwaysZero) {
	EXPECT_EQ(store.size(), 0);
}

TEST_F(NullMessageStoreTests, IsEmptyAlwaysTrue) {
	EXPECT_TRUE(store.is_empty());
}

TEST_F(NullMessageStoreTests, AllOperationsWithMultipleKeys) {
	for (int i = 0; i < 100; ++i) {
		EXPECT_NO_THROW(store.store("key_" + std::to_string(i), "msg"));
	}
	EXPECT_EQ(store.size(), 0);
}
```

**Casos de Teste a Cobrir**:
- ✅ Construtor não lança exceção
- ✅ Store não armazena (apenas descarta)
- ✅ Retrieve sempre retorna vazio/nulo
- ✅ Clear não faz nada
- ✅ Size sempre retorna 0
- ✅ IsEmpty sempre retorna true
- ✅ Múltiplas operações sequenciais

---

## 🟠 ALTAS PRIORIDADES - Prioridade 2 (50-75%)

### 3. **boost_tcp_socket_ssl.cpp** - 67%

**Localização**:
- Fonte: `src/cpp/socket/boost/boost_tcp_socket_ssl.cpp`
- Header: `src/include/socket/boost/boost_tcp_socket_ssl.h` (62%)
- Testes: `tests/socket/boost/boost_tcp_socket_ssl_tests.cpp`

**Problema**: Cobertura baixa em operações SSL/TLS

**Plano de Ação**:
```cpp
// tests/socket/boost/boost_tcp_socket_ssl_tests.cpp (EXPANDIR)

class BoostTcpSocketSSLTests : public ::testing::Test {
protected:
	// Usar fixtures para setup SSL context
};

// FALHAS DE CONEXÃO
TEST_F(BoostTcpSocketSSLTests, ConnectionFailureHandling) {
	// Testar erro ao conectar em porta inválida
}

TEST_F(BoostTcpSocketSSLTests, SSLHandshakeFailure) {
	// Simular falha no handshake SSL
}

TEST_F(BoostTcpSocketSSLTests, CertificateValidationError) {
	// Testar rejeição de certificado inválido
}

// OPERAÇÕES NORMAIS
TEST_F(BoostTcpSocketSSLTests, SendDataOverSSL) {
	// Enviar dados e verificar encriptação
}

TEST_F(BoostTcpSocketSSLTests, ReceiveDataOverSSL) {
	// Receber e descriptografar dados
}

// EDGE CASES
TEST_F(BoostTcpSocketSSLTests, CloseSocketWhileSSLActive) {
	// Fechamento durante operação SSL
}

TEST_F(BoostTcpSocketSSLTests, DisconnectWithPendingData) {
	// Desconectar com dados pendentes
}

TEST_F(BoostTcpSocketSSLTests, ReuseSSLSocket) {
	// Reutilizar socket após desconexão
}
```

**Casos de Teste a Cobrir**:
- ✅ Falha de conexão (port, timeout, refused)
- ✅ Falha de handshake SSL/TLS
- ✅ Validação de certificado (expired, invalid, self-signed)
- ✅ Envio/recebimento sobre SSL
- ✅ Fechamento durante operação ativa
- ✅ Desconexão com dados pendentes
- ✅ Reutilização de socket
- ✅ Timeout durante SSL handshake

---

### 4. **boost_tcp_socket.cpp** - 68%

**Localização**:
- Fonte: `src/cpp/socket/boost/boost_tcp_socket.cpp`
- Testes: `tests/socket/boost/boost_tcp_socket_tests.cpp`

**Problema**: Casos de erro não cobertos em conexão TCP

**Plano de Ação**:
```cpp
// tests/socket/boost/boost_tcp_socket_tests.cpp (EXPANDIR)

// FALHAS DE CONEXÃO
TEST_F(BoostTcpSocketTests, ConnectToInvalidHost) {
	socket_->connect("300.300.300.300", 12345);
	EXPECT_FALSE(socket_->is_connected());
}

TEST_F(BoostTcpSocketTests, ConnectToClosedPort) {
	// Porta que não está escutando
	EXPECT_FALSE(socket_->connect("127.0.0.1", 54321));
}

TEST_F(BoostTcpSocketTests, ConnectTimeout) {
	// Executar connect com timeout
}

// ENVIO/RECEBIMENTO
TEST_F(BoostTcpSocketTests, SendLargeBuffer) {
	// Buffer > 64KB
}

TEST_F(BoostTcpSocketTests, SendAfterDisconnect) {
	// Tentar enviar após desconectar
}

TEST_F(BoostTcpSocketTests, ReceiveAfterDisconnect) {
	// Tentar receber após desconectar
}

// CASOS EXTREMOS
TEST_F(BoostTcpSocketTests, SendEmptyBuffer) {
	EXPECT_TRUE(socket_->send("", 0));
}

TEST_F(BoostTcpSocketTests, MultipleConsecutiveSends) {
	// N operações send seguidas
}

TEST_F(BoostTcpSocketTests, ReceiveWithShortTimeout) {
	// Receive com timeout muito curto
}
```

**Casos de Teste a Cobrir**:
- ✅ Conexão a host inválido
- ✅ Conexão a porta fechada
- ✅ Timeout de conexão
- ✅ Envio de buffer grande
- ✅ Envio após desconexão
- ✅ Recebimento após desconexão
- ✅ Envio/recebimento de buffer vazio
- ✅ Múltiplos sends/receives sequenciais

---

### 5. **fast_buffer.h** - 71%

**Localização**:
- Fonte: `src/include/utils/fast_buffer.h` (header-only)
- Testes: `tests/utils/fast_buffer_tests.cpp`

**Problema**: Operações edge case não cobertas

**Plano de Ação**:
```cpp
// tests/utils/fast_buffer_tests.cpp (EXPANDIR)

class FastBufferTests : public ::testing::Test {
protected:
	FastBuffer<1024> buffer;
};

// ESCRITA E LEITURA
TEST_F(FastBufferTests, WriteRead) {
	buffer.write("test", 4);
	auto data = buffer.read();
	EXPECT_EQ(data.size(), 4);
}

// ESTOURO
TEST_F(FastBufferTests, WriteExceedsCapacity) {
	// Escrever mais que capacidade
	EXPECT_THROW(buffer.write(large_data), std::overflow_error);
}

// ESTADO
TEST_F(FastBufferTests, IsEmptyAfterConstruction) {
	EXPECT_TRUE(buffer.empty());
}

TEST_F(FastBufferTests, SizeAfterWrite) {
	buffer.write("abc", 3);
	EXPECT_EQ(buffer.size(), 3);
}

// LIMPEZA
TEST_F(FastBufferTests, ClearResets) {
	buffer.write("test", 4);
	buffer.clear();
	EXPECT_TRUE(buffer.empty());
}

// ITERADORES
TEST_F(FastBufferTests, IteratorTraversal) {
	buffer.write("abc", 3);
	int count = 0;
	for (auto it = buffer.begin(); it != buffer.end(); ++it) {
		count++;
	}
	EXPECT_EQ(count, 3);
}

// TIPOS DIFERENTES
TEST_F(FastBufferTests, DifferentSizes) {
	FastBuffer<256> small;
	FastBuffer<8192> large;
	// Testar ambos
}

// FILL PATTERNS
TEST_F(FastBufferTests, FillWithPattern) {
	buffer.fill('X');
	// Verificar padrão
}
```

**Casos de Teste a Cobrir**:
- ✅ Escrita e leitura normais
- ✅ Buffer vazio
- ✅ Overflow (escrita > capacidade)
- ✅ Size após operações
- ✅ Clear e limpeza
- ✅ Iteradores (begin, end)
- ✅ Diferentes tamanhos de buffer
- ✅ Fill com padrão
- ✅ Múltiplas escritas sequenciais

---

### 6. **string_utils.h** - 71%

**Localização**:
- Fonte: `src/include/utils/string_utils.h`
- Testes: `tests/utils/string_utils_tests.cpp`

**Problema**: Funções utilitárias com casos extremos não testados

**Plano de Ação**:
```cpp
// tests/utils/string_utils_tests.cpp (EXPANDIR)

class StringUtilsTests : public ::testing::Test {};

// TRIM
TEST_F(StringUtilsTests, TrimLeadingWhitespace) {
	EXPECT_EQ(trim_left("  hello"), "hello");
}

TEST_F(StringUtilsTests, TrimTrailingWhitespace) {
	EXPECT_EQ(trim_right("hello  "), "hello");
}

TEST_F(StringUtilsTests, TrimBothSides) {
	EXPECT_EQ(trim("  hello  "), "hello");
}

TEST_F(StringUtilsTests, TrimEmptyString) {
	EXPECT_EQ(trim(""), "");
}

TEST_F(StringUtilsTests, TrimOnlyWhitespace) {
	EXPECT_EQ(trim("   "), "");
}

// SPLIT
TEST_F(StringUtilsTests, SplitByDelimiter) {
	auto parts = split("a,b,c", ",");
	EXPECT_EQ(parts.size(), 3);
}

TEST_F(StringUtilsTests, SplitWithoutDelimiter) {
	auto parts = split("abc", ",");
	EXPECT_EQ(parts.size(), 1);
}

TEST_F(StringUtilsTests, SplitEmptyString) {
	auto parts = split("", ",");
	EXPECT_TRUE(parts.empty());
}

// JOIN
TEST_F(StringUtilsTests, JoinStrings) {
	EXPECT_EQ(join({"a", "b", "c"}, ","), "a,b,c");
}

TEST_F(StringUtilsTests, JoinEmptyList) {
	EXPECT_EQ(join({}, ","), "");
}

// CASE
TEST_F(StringUtilsTests, ToLowercase) {
	EXPECT_EQ(to_lower("HELLO"), "hello");
}

TEST_F(StringUtilsTests, ToUppercase) {
	EXPECT_EQ(to_upper("hello"), "HELLO");
}

TEST_F(StringUtilsTests, IsNumeric) {
	EXPECT_TRUE(is_numeric("123"));
	EXPECT_FALSE(is_numeric("abc"));
}

TEST_F(StringUtilsTests, Contains) {
	EXPECT_TRUE(contains("hello world", "world"));
	EXPECT_FALSE(contains("hello", "world"));
}

// REPLACE
TEST_F(StringUtilsTests, ReplaceSubstring) {
	EXPECT_EQ(replace("hello world", "world", "there"), "hello there");
}

TEST_F(StringUtilsTests, ReplaceNotFound) {
	EXPECT_EQ(replace("hello", "xyz", "abc"), "hello");
}
```

**Casos de Teste a Cobrir**:
- ✅ Trim (esquerda, direita, ambos)
- ✅ Trim com string vazia
- ✅ Trim com espaços em branco
- ✅ Split por delimitador
- ✅ Split sem delimitador
- ✅ Join de arrays/vetores
- ✅ Join vazio
- ✅ Conversão lowercase/uppercase
- ✅ Checksum numérico
- ✅ Contains substring
- ✅ Replace substring

---

## 🟡 MÉDIAS PRIORIDADES - Prioridade 3 (75-80%)

### 7. **boost_tcp_server_ssl.cpp** - 77%

**Localização**:
- Fonte: `src/cpp/socket/boost/boost_tcp_server_ssl.cpp`
- Testes: `tests/socket/boost/boost_tcp_server_ssl_tests.cpp`

**Plano de Ação**:
```cpp
// tests/socket/boost/boost_tcp_server_ssl_tests.cpp (EXPANDIR)

TEST_F(BoostTcpServerSSLTests, BindToInvalidPort) {
	// Porta < 1 ou > 65535
}

TEST_F(BoostTcpServerSSLTests, BindToPrivilegedPort) {
	// Porta < 1024 sem permissão
}

TEST_F(BoostTcpServerSSLTests, AcceptWithSSLHandshake) {
	// Aceitar conexão e completar handshake
}

TEST_F(BoostTcpServerSSLTests, AcceptClientTimeout) {
	// Timeout aguardando cliente
}

TEST_F(BoostTcpServerSSLTests, RejectedCertificateClient) {
	// Cliente com certificado inválido
}

TEST_F(BoostTcpServerSSLTests, MultipleClientConnections) {
	// N clientes simultâneos
}

TEST_F(BoostTcpServerSSLTests, ServerStop) {
	// Parar servidor e validar estado
}
```

**Casos de Teste a Cobrir**:
- ✅ Bind a porta inválida
- ✅ Bind a porta já em uso
- ✅ Accept com handshake completo
- ✅ Timeout aguardando cliente
- ✅ Rejeição de certificado inválido
- ✅ Múltiplos clients simultâneos
- ✅ Stop/shutdown do servidor

---

### 8. **memory_message_store.h** - 78%

**Localização**:
- Fonte: `src/include/store/memory_message_store.h`
- Testes: `tests/store/memory_message_store_tests.cpp`

**Plano de Ação**:
```cpp
// tests/store/memory_message_store_tests.cpp (EXPANDIR)

TEST_F(MemoryMessageStoreTests, RetrieveNonExistentKey) {
	EXPECT_TRUE(store.retrieve("nonexistent").empty());
}

TEST_F(MemoryMessageStoreTests, OverwriteExistingKey) {
	store.store("key", "message1");
	store.store("key", "message2");
	EXPECT_EQ(store.retrieve("key"), "message2");
}

TEST_F(MemoryMessageStoreTests, ClearEmptiesStore) {
	store.store("key1", "msg1");
	store.store("key2", "msg2");
	store.clear();
	EXPECT_EQ(store.size(), 0);
}

TEST_F(MemoryMessageStoreTests, LargeMessageStorage) {
	std::string large(1MB, 'X');
	store.store("large", large);
	EXPECT_EQ(store.retrieve("large"), large);
}

TEST_F(MemoryMessageStoreTests, ManyMessages) {
	for (int i = 0; i < 10000; ++i) {
		store.store("key_" + std::to_string(i), "msg_" + std::to_string(i));
	}
	EXPECT_EQ(store.size(), 10000);
}

TEST_F(MemoryMessageStoreTests, IterateAllMessages) {
	store.store("k1", "m1");
	store.store("k2", "m2");
	auto all = store.get_all();
	EXPECT_EQ(all.size(), 2);
}
```

**Casos de Teste a Cobrir**:
- ✅ Retrieve de chave não existente
- ✅ Sobrescrever chave existente
- ✅ Clear esvazia completamente
- ✅ Armazenar mensagens grandes (> 1MB)
- ✅ Muitas mensagens (10k+)
- ✅ Iterar sobre todas as mensagens

---

### 9. **boost_socket_context.cpp** - 79%

**Localização**:
- Fonte: `src/cpp/socket/boost/boost_socket_context.cpp`
- Testes: `tests/socket/boost/boost_socket_context_tests.cpp`

**Plano de Ação**:
```cpp
// tests/socket/boost/boost_socket_context_tests.cpp (EXPANDIR)

TEST_F(BoostSocketContextTests, CreateMultipleContexts) {
	auto ctx1 = factory.create_context();
	auto ctx2 = factory.create_context();
	EXPECT_NE(ctx1, ctx2);
}

TEST_F(BoostSocketContextTests, InvalidSSLVersion) {
	EXPECT_THROW(factory.create_context_ssl("invalid_version"), std::runtime_error);
}

TEST_F(BoostSocketContextTests, ConfigureSocketOption) {
	auto ctx = factory.create_context();
	ctx->set_option("SO_REUSEADDR", true);
	// Verificar se foi configurado corretamente
}

TEST_F(BoostSocketContextTests, GetSocketOption) {
	auto ctx = factory.create_context();
	auto value = ctx->get_option("SO_REUSEADDR");
	// Verificar
}

TEST_F(BoostSocketContextTests, ContextLifecycle) {
	{
		auto ctx = factory.create_context();
		// Use context
	} // Destrutor deve liberar recursos
}
```

**Casos de Teste a Cobrir**:
- ✅ Criar múltiplos contextos
- ✅ Versão SSL inválida
- ✅ Configurar opções de socket
- ✅ Obter opções de socket
- ✅ Lifecycle completo

---

## 📊 Resumo de Ações

| Arquivo | Atual | Meta | Prioridade | Esforço | Arquivos |
|---------|-------|------|------------|---------|----------|
| error_codes.h | 34% | >80% | 🔴 1 | Médio | 1 novo |
| null_message_store.h | 33% | >80% | 🔴 1 | Médio | 1 expand |
| boost_tcp_socket_ssl.cpp | 67% | >85% | 🟠 2 | Alto | 1-2 expand |
| boost_tcp_socket.cpp | 68% | >85% | 🟠 2 | Alto | 1-2 expand |
| fast_buffer.h | 71% | >85% | 🟠 2 | Médio | 1 expand |
| string_utils.h | 71% | >85% | 🟠 2 | Médio | 1 expand |
| boost_tcp_server_ssl.cpp | 77% | >85% | 🟡 3 | Médio | 1 expand |
| memory_message_store.h | 78% | >85% | 🟡 3 | Médio | 1 expand |
| boost_socket_context.cpp | 79% | >90% | 🟡 3 | Baixo | 1 expand |

---

## 🎯 Roadmap de Execução

### **Semana 1: CRÍTICAS**
1. Criar `tests/utils/error_codes_tests.cpp`
2. Expandir `tests/store/null_message_store_tests.cpp`
3. Executar relatório de cobertura

### **Semana 2: ALTAS PRIORIDADES**
1. Expandir testes Socket SSL (`boost_tcp_socket_ssl_tests.cpp`)
2. Expandir testes Socket TCP (`boost_tcp_socket_tests.cpp`)
3. Expandir testes Utils (`fast_buffer_tests.cpp`, `string_utils_tests.cpp`)

### **Semana 3: MÉDIAS PRIORIDADES**
1. Expandir testes Server SSL (`boost_tcp_server_ssl_tests.cpp`)
2. Expandir testes Memory Store (`memory_message_store_tests.cpp`)
3. Expandir testes Context (`boost_socket_context_tests.cpp`)

### **Semana 4: VALIDAÇÃO**
1. Executar OpenCppCoverage novamente
2. Validar that all targets atingiram >80%
3. Documentation e cleanup

---

## ✅ Checklist de Conclusão

- [ ] Todos os arquivos críticos têm cobertura >80%
- [ ] Todos os testes passam (1756 testes)
- [ ] Novo relatório de cobertura gerado
- [ ] Documentação atualizada
- [ ] Commit com mensagem: "test: improve code coverage for non-engine components"


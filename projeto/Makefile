# Nome do compilador
CC = gcc

# Opções de compilação
CFLAGS = -Wall -Wextra

# Nome do executável
TARGET = projeto

# Lista de arquivos fonte
SRCS = ficheiros_valid.c operacoes.c projeto.c

# Regra de compilação do alvo final
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS)

# Regra para limpar os arquivos objetos e o executável
clean:
	rm -f $(TARGET)

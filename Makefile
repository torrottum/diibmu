CC := gcc
SRC := diibmu.c
BIN := diibmu
CFLAGS := -Wall -std=c99
LDFLAGS := -lraylib -lm -lGL -lpthread -ldl -lrt -lX11

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN) $(LDFLAGS)

run: all
	./$(BIN)

clean:
	rm -f $(BIN)

.PHONY: all run clean

CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm -I./include
./bin/noodle: ./src/*.c
	${CC} -o ./bin/noodle ./src/*.c $(CFLAGS) $(LDFLAGS)

.PHONY: test clean

test: ./bin/noodle
	./bin/noodle

clean: ./bin/noodle
	rm -f ./bin/noodle


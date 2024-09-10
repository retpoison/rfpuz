CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Wextra -Wno-unused-parameter -O2
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

OBJ = rfpuz.o \
			external/utils.o \
			external/generator.o

all: rfpuz

$(OBJ): %.o: %.c 
	$(CC) -o $@ -c $(CFLAGS) $^

rfpuz: $(OBJ)
	$(CC) $(CFLAGS) -o $@ -s $(OBJ) $(LDFLAGS) 

clean:
	rm -vf rfpuz *.o $(OBJ)


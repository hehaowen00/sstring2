CC = gcc
OBJS = sstring.c main.c
TARGET = main
CFLAGS = -Wall -Werror -Wfloat-equal -Wundef -Wshadow \
		 -Wpointer-arith -g
LFLAGS = -I./include/

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)


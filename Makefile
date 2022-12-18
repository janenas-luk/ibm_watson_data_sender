SRCS:=$(wildcard *.c)
OBJS:=$(SRCS:.c=.o)

BIN:=watsond
CFLAGS:=-I../iot-c/src/wiotp/sdk -I../iot-c/paho.mqtt.c/src
LDFLAGS:=-L../iot-c/paho.mqtt.c/build/output -L../iot-c/build -liotp-as-device -lpaho-mqtt3as

.PHONY: all clean

all: $(BIN)

$(BIN):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) $(BIN) $(OBJS)
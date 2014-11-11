CFLAGS  += `pkg-config --cflags direct`
LDFLAGS += `pkg-config --libs direct`

CFLAGS  += -Wall -O3
LDFLAGS += -lpthread

all: calls latency throughput throughput_pipe

clean:
	rm -f calls latency throughput throughput_pipe

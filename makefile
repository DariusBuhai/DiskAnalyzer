GCC=gcc
FLAGS=-fsanitize=address,undefined,signed-integer-overflow

DAEMON_CFILES:=$(wildcard Daemon/*.c)
WORKER_CFILES:=$(wildcard Worker/*.c)
SHARED_CFILES:=$(wildcard Shared/*.c)
LIB_HFILES:=-IShared -IDaemon -IWorker -pthread

all: daemon_release da
debug: daemon_debug da

daemon_release: $(DAEMON_CFILES) $(WORKER_CFILES) $(SHARED_CFILES)
	$(GCC) -g $(DAEMON_CFILES) $(WORKER_CFILES) $(SHARED_CFILES) -o daemon_runner $(LIB_HFILES) $(FLAGS)

daemon_debug: $(DAEMON_CFILES) $(WORKER_CFILES) $(SHARED_CFILES)
	$(GCC) -g $(DAEMON_CFILES) $(WORKER_CFILES) $(SHARED_CFILES) -o daemon_runner $(LIB_HFILES) $(FLAGS) -DDEBUG


daemon_stef:
	gcc -g Daemon/*.c Shared/*.c Worker/*.c -lrt -o daemon -IShared -IDaemon -IWorker -pthread -fsanitize=address,undefined,signed-integer-overflow

daemon_stef_debug:
	gcc -g Daemon/*.c Shared/*.c Worker/*.c -lrt -o daemon -IShared -IDaemon -IWorker -pthread -fsanitize=address,undefined,signed-integer-overflow -DDEBUG


da: da.c
	$(GCC) -o da da.c

clean:
	rm da
	rm daemon_runner
	rm -r daemon_runner.*

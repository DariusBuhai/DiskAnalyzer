
all:
debug:
    gcc -g Daemon/*.c Shared/*.c Worker/*.c -lrt -o daemon -IShared -IDaemon -IWorker -pthread -fsanitize=address,undefined,signed-integer-overflow
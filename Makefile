all:
	gcc -include include/semaphores.h src/semaphores.c -o bin/semaphores -lpthread
	gcc -include include/monitor.h src/monitor.c -o bin/monitor -lpthread

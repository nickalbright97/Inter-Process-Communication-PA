all:
	gcc -pthread wrappers.c factory_line.c -o factory_line
	gcc -pthread wrappers.c parent.c -o parent
	gcc -pthread wrappers.c supervisor.c -o supervisor
clean:
	rm -f *.o parent factory supervisor *.log;
	ipcrm -a;
	rm -f /dev/shm/sem*albrigne;

all:
	gcc -pthread wrappers.c factory_line.c -o factory_line
	gcc -pthread wrappers.c parent.c -o parent

clean:
	rm *.o ;

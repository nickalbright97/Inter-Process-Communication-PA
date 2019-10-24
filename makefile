all:
	parent.o factory_line.o

factory_line.o: factory_line.c
	gcc factory_line.c -o factory_line

parent.o: parent.c
	gcc parent.c -o parent

clean:
	rm *.o ;

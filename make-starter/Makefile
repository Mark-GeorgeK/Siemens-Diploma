
calc : add.o divide.o main.o multiply.o subtract.o ops.h
	cc -o calc add.c divide.c multiply.c main.c subtract.c 

main.o : main.c ops.h
	cc -c main.c

add.o: add.c
	cc -c add.c

subtract.o: subtract.c
	cc -c subtract.c 

multiply.o : multiply.c 
	cc -c multiply.c

divide.o : divide.c ops.h
	cc -c divide.c


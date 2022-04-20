all: user calculator

user: user.o
	gcc -o user user.o

user.o: user.c
	gcc -c user.c

calculator: calculator.o
	gcc -o calculator calculator.o

calculator.o: calculator.c
	gcc -c calculator.c

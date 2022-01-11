#Makefile

all: balcao.o cliente.o medico.o
	gcc -o balcao.o cliente.o medico.o

balcao.o: balcao.c
	gcc balcao.c -c -pthread

cliente.o: cliente.c
	gcc cliente.c -c

medico.o: medico.c
	gcc medico.c -c 

clean:
	rm balcao
	rm cliente
	rm medico
	rm -rf *.o
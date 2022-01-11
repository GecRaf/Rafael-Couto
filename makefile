#Makefile

all: balcao.o cliente.o medico.o
	gcc -o balcao.o cliente.o medico.o

balcao.o: balcao.c
	gcc arbitro.c -c -pthread

cliente.o: cliente.c
	gcc cliente.c -c

medico.o: medico.c
	gcc medico.c -c 

exec:

clean:
	rm balcao
	rm cliente
	rm medico
	rm -rf *.o
#Makefile

LIBS=		-pthread

all: BALCAO CLIENTE MEDICO
	export MAXCLIENTES=10
	export MAXMEDICOS=10
	export MAXESPECIALIDADES=6
	export MAXLUGARESFILA=5	

balcao.o: balcao.c utils.h
	gcc -c balcao.c -o balcao.o $(LIBS)

funcoes.o: funcoes.c utils.h
	gcc -c funcoes.c -o funcoes.o

BALCAO: balcao.o funcoes.o
	gcc -o BALCAO balcao.o funcoes.o $(LIBS)

cliente.o: cliente.c utils.h
	gcc -c cliente.c -o cliente.o $(LIBS)

CLIENTE: cliente.o
	gcc -o CLIENTE cliente.o $(LIBS)

medico.o: medico.c utils.h
	gcc -c medico.c -o medico.o $(LIBS)

MEDICO: medico.o
	gcc -o MEDICO medico.o $(LIBS)

clean:
	rm -rf *.o
	rm BALCAO
	rm CLIENTE
	rm MEDICO
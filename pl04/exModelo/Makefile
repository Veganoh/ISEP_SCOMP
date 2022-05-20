exModelo: main.c
	gcc -Wall -o exModelo main.c -lpthread -lrt
	
contaVotos: contaVotos.c
	gcc -Wall -o contaVotos contaVotos.c -lpthread -lrt
	
votaSocio: votaSocio.c
	gcc -Wall -o votaSocio votaSocio.c -lpthread -lrt


run: exModelo contaVotos votaSocio

	./exModelo

clean:
	rm /dev/shm/*

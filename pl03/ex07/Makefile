writer: writer.c
	gcc -Wall -o writer writer.c -lrt
	
reader: reader.c
	gcc -Wall -o reader reader.c -lrt	
	
run: writer reader
	./writer
	./reader

clean:
	rm /dev/shm/*


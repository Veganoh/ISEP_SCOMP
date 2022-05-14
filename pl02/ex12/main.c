#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "spawnChilds.h"

#define NUM_CHILD 5
#define NUM_PIPES 6

typedef struct {
    char name[20];
    float price;
} Product;

typedef struct {
    int reader_id;
    int product_id;
} Data_bc_r;

void close_pipes(int *fd, int ignore, int rw) {
    int i;

    for(i = rw; i < NUM_PIPES; i += 2) {
        //printf("\tPipe %d, rw: %d, ignore: %d\n", i, rw, ignore);
        if (i != ignore + rw) { 
            //printf("\tClosing pipe %d, rw: %d, ignore: %d\n", i, rw, ignore);

            close(*(fd + i));

            //printf(" - Closed it! \n\n");
        }
    }
}

int main() {
    int fd[NUM_PIPES * 2];
    int i;
    int idx;
    
    Data_bc_r bc_data;

    for (i = 0; i < NUM_PIPES; i++) {
        if (pipe(fd + i * 2) == -1) {
            printf("Pipe %d Failed!", i);
            exit(1);
        }
    }

    idx = spawn_childs(NUM_CHILD);

    if (idx == 0) {
        Product productDb[5];
        char buf[20];

        close(fd[1]);
        close_pipes(fd, 0, 0);

        for (i = 0; i < 5; i++) {
            snprintf(buf, 20, "Product_%d", i + 1);
            strcpy(productDb[i].name, buf);
            productDb[i].price = i + 0.99;

            printf("Name: %s\nPrice: %.2f€\n\n", productDb[i].name, productDb[i].price);
        }

        printf("----------------------------------------\n\n");

        while(read(fd[0], &bc_data, sizeof(Data_bc_r))) {
            //printf("Writing to bc: %d - pipe: %d...\n", bc_data.reader_id, bc_data.reader_id * 2 + 1);
            write(fd[bc_data.reader_id * 2 + 1], &productDb[bc_data.product_id], sizeof(Product));
        }

        //printf("Closing pipes for Parent process...\n");
        close_pipes(fd, 0, 1);
        close(fd[0]);
        exit(0);
    } else {
        sleep(idx);
        Product product;
        //printf("\nClosing pipes for process %d...\n", idx);
        close_pipes(fd, idx * 2, 0);
        close_pipes(fd, 0, 1);

        bc_data.reader_id = idx;
        bc_data.product_id = idx - 1;

        printf("Barcode Reader %d asking for Product %d info...\n", idx, idx);
        write(fd[1], &bc_data, sizeof(Data_bc_r));

        read(fd[idx * 2], &product, sizeof(Product));
        printf("Barcode Reader %d received info for Product %d:\n\t->Product Name: %s\n\t->Price: %.2f€\n\n", idx, idx, product.name, product.price);
        
        //printf("Closing pipes for process %d...\n\n\n", idx);
        close(fd[idx * 2]);
        close(fd[1]);
        exit(0);
    }

    return 0;
}
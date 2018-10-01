#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "../pthread.h"

define_list(int);

int main(int argc, char* argv[]){
        List(int)* b = new_list(int);
        push(b,1);
        push(b,2);
        push(b,3);
        push(b,4);
        push(b,5);
        push(b,5);
        printf("Size: %zu\n",size(b));
        printf("Poped: %d\n",pop(b));
        printf("Poped: %d\n",pop(b));
        printf("Poped: %d\n",pop(b));
        printf("Poped: %d\n",pop(b));
        printf("Poped: %zu\n",size(b));
        return 0;

    }

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "queue_macro_simple.h"

define_list(int)

int main(int argc, char* argv[]){
        
        List(int)* b = new_list_int();
        push(b,1);
        push(b,2);
        push(b,3);
        push(b,4);
        push(b,5);
        printf("%zu\n",size(b));
        printf("%d\n",pop(b));
        printf("%d\n",pop(b));
        push(b,5);
        printf("%d\n",pop(b));
        printf("%d\n",pop(b));
        printf("%zu\n",size(b));
        return 0;
    
    }

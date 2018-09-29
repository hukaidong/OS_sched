#include "my_pthread"
  //first you need to change define_list() to be  define_list(int)

int main() {
  
    List(int)* b = new_list_int();
    push(b,1);
    push(b,2);
    push(b,3);
    push(b,4);
    printf("%zu\n",size(b));
    printf("%d\n",pop(b));
    printf("%d\n",pop(b));
    push(b,5);
    printf("%d\n",pop(b));
    printf("%d\n",pop(b));
    printf("%zu\n",size(b));
  return 0;
}

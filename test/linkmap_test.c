#define DEBUG
#include "utils/linked_map_new.h"



define_lmap(my_map, int, int);
int main(void)
{

    lm_head_t(my_map) map;
    lm_h_init(my_map, map);
    lm_insert(my_map, map, 1, 1);
    lm_insert(my_map, map, 2, 2);
    printf("first node key is %d\n",map->next->key);

    return 0;
}

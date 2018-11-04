#include "include/type.h"
#include "include/my_malloc.h"

void insert_key2head(node2_p *static_head, int key, node *head2){
  node2_p new_node = (node2_p)_lib_malloc(sizeof(node2));
  new_node->key = key;
  new_node->head = head2;
  new_node->next = *static_head;
  *static_head = new_node;
}

node* find_head(node2_p *static_head,int key){
    node2_p current = *static_head;
    while(current != NULL){
        if(current->key = key){
            return current->head;
        }
        current = current->next;
    }
    return NULL;
}

int init_page_num(node2_p *static_head,int key,int num){           //function to add value of page_num
    node2_p current = *static_head;
    while(current!= NULL){
        if(current->key = key){
            current->page_num += num;
            return 1;
        }
        current = current->next;
    }
    return -1;
}

int get_page_num(node2_p *static_head,int key){
    node2_p current = *static_head;
    while(current!=NULL){
        if(current->key = key){
            return current->page_num;
        }
        current->next;
    }
    return NULL;
}

int iscontainskey(node2_p *static_head,int key){
    node2_p current = *static_head;
    while(current !=NULL){
        if(current->key = key){
            return 1;
        }
        current = current->next;
    }
    return -1;
}
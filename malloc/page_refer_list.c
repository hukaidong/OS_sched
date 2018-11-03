#include "include/type.h"

void insert(Node **head_ref, int k, int value){
  Node *new_node = (Node*)malloc (sizeof(struct Node));
  new_node->k = k;
  new_node->v = value;
  new_node->next = *head_ref;
  *head_ref = new_node;
}

int pop(Node *head, int k){
  Node *new_node = (Node*)malloc (sizeof(struct Node));
  int val;
  if(head->k == k){
    val = head->v;
    head->k = head->next->k;
    head->v = head->next->v;
    new_node = head->next;
    head->next = head->next->next;
    free(new_node);
    return val;
  }
  else{
    Node *prev = head;
    while(prev->next!=NULL && prev->next->k != k)
      prev = prev->next;
    if(prev->next == NULL){
      return -1; //not k found in the linkedlist
    }

    val = prev->next->v;
    prev->next = prev->next->next;
    free(new_node);
    return val;
  }
}

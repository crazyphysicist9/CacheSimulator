/**
 * Source name     : list.c
 * Version         : 1.0
 * Created date    : 02/08/2017
 * Author          : I-Ting Angelina Lee and Joseph Li
 * Description     : This file define the list data type 
 *                   that contains list of int's and supports
 *                   clear, prepend, append, delete, size, and print.
 **/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "stdbool.h"
#include "list.h"

/* set it to 1 for calling check_list for debugging */
#define LIST_DEBUG 0

#if LIST_DEBUG
#define CHECK_LIST(l) check_list(l)
#else
#define CHECK_LIST(l)
#endif

/* list node data type */

/*
struct node_t {
    struct node_t *prev;
    struct node_t *next;
    line val;
};
typedef struct node_t node_t;
*/

/* A function that checks whether the list invariant is maintained properly */ 
void check_list(list_t *list) {
    assert(list);    
    int count = 0;
  
    /* check head */
    node_t *node = list->head;
    assert(node == NULL || node->prev == NULL); 
    /* traverse the list forward and check count */
    while(node) {
        count++;
        node = node->next;
    }
    assert(count == list->size);

    /* reinit and check tail */
    count = 0; 
    node = list->tail;
    assert(node == NULL || node->next == NULL); 
    /* traverse the list backward and check count */
    while(node) {
        count++;
        node = node->prev;
    }
    assert(count == list->size);
    /* if we didn't fail assert, prev / next didn't skip anything */
}

/**
 * Input: an allocated but uninitialized list
 * This function initializes the list.
 **/ 
int list_init(list_t *list) {
    if(!list) { return 1; }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return 0;
}

/**
 * Input: a list
 * This function clears the list (including deallocating memory used).
 * Return value: 1 if any error is encountered, 0 otherwise.
 **/ 
int list_clear(list_t *list) {
    if(!list) { return 1; }

    CHECK_LIST(list);
    while(list->head) {
        node_t *to_free = list->head;
        list->head = list->head->next;
        if(list->head) {
          list->head->prev = NULL;
        }
        free(to_free);  /* free old list head */
    }
    list_init(list);
    CHECK_LIST(list);

    return 0;
}

/**
 * Input: a list and a value to prepend to the list. 
 * This function prepends the new value onto the list.
 * Return value: 1 if any error occur (such as if the list is not properly
 *    initialized); 0 otherwise. 
 **/ 
int list_prepend(list_t *list, line val) {
    if(!list) { return 1; }

    /* fill me in */
    node_t *node = (node_t *) malloc(sizeof(node_t));
    node->val = val;
    if(list->size==0){
      node->prev = NULL;
      node->next = NULL;
      list->head = node;
      list->tail = node;
    }
    else{
      node->prev = NULL;
      node->next = list->head;
      (list->head)->prev = node;
      list->head = node;
    }
    ++list->size;

    return 0;
}

/**
 * Input: a list and a value to append to the list. 
 * This function appends the new value onto the list.
 * Return value: 1 if any error occur (such as if the list is not properly
 *    initialized); 0 otherwise. 
 **/ 
int list_append(list_t *list, line val) {
    if(!list) { return 1; }

    /* fill me in */
    node_t *node = (node_t *) malloc(sizeof(node_t));
    node->val = val;
    if(list->size==0){
      node->prev = NULL;
      node->next = NULL;
      list->head = node;
      list->tail = node;
    }
    else{
      node->prev = list->tail;
      node->next = NULL;
      (list->tail)->next = node;
      list->tail = node;
    }
    ++list->size;

    return 0;
}

/**
 * Input: a list and a value to delete 
 * This function searches the list to find the given value.
 * If the value is found, delete the value.
 * Return value: 1 if any error occur (such as value not found or if the 
 *    list is not properly initialized); 0 otherwise. 
 **/ 
int list_delete(list_t *list, line val) {
    if(!list) { return 1; }

    /* fill me in */
    node_t *a = list->head;
    while(a!=NULL){
      if(a->val.tag==val.tag){
        if(list->size == 1){
          list->head = NULL;
          list->tail = NULL;
        }
        else if(a == list->head){
          list->head = a->next;
          (a->next)->prev = NULL;
        }
        else if(a == list->tail){
          list->tail = a->prev;
          (a->prev)->next = NULL;
        }
        else{
          (a->prev)->next = a->next;
          (a->next)->prev = a->prev;
        }
        free(a);
        --list->size;
        return 0;
      }
      a = a->next;
      
    }

    return 1;
}

/**
 * Input: a list and a size_ptr (pointer to an int)
 * This function stores the size of the list in size_ptr. 
 * Return value: 1 if any error occur (if the list is not properly 
 *    initialized); 0 otherwise. 
 **/ 
int list_size(list_t *list, int *size_ptr) {
    if(!list) { return 1; }

    /* fill me in */
    (*size_ptr) = list->size;

    return 0;
}

/**
 * Input: a list
 * This function prints the content of the list 
 * Return value: 1 if any error occur (if the list is not properly 
 *    initialized); 0 otherwise. 
 **/ 
void list_print(list_t *list) {
    if(!list) { fprintf(stderr, "List not intialized.\n"); return; }

    CHECK_LIST(list);

    if(!list->head) { printf("List empty."); }
    else { printf("List: "); }
    node_t *tmp = list->head;
    while(tmp) {
        printf("%d ", tmp->val.tag);
        tmp = tmp->next;
    }
    printf("\n");

    return;
}

line* newline(int tag, bool dirty){
  line *a = (line *) malloc(sizeof(line));
  a->tag = tag;
  a->dirty = dirty;
  return a;
}

bool tagmatch(int ntag, Set *a){
  node_t * f = a->lines.head;
  while(f!=NULL){
    if(f->val.tag==ntag){
      return true;
    }
    f=f->next;
  }
  return false;
}

bool doubleref(int ntag, Set *a){
  if(a->lines.tail->val.tag == ntag)
    return true;
  return false;
}

void renew(int ntag, Set *a, bool load){
  bool dirty=true;
  if(load){
    node_t * f = a->lines.head;
    while(f!=NULL){
      if(f->val.tag==ntag){
        dirty = f->val.dirty;
        break;
      }
      f=f->next;
    }
  }

  line *b = newline(ntag, dirty);
  list_delete(&a->lines, *b);
  list_append(&a->lines, *b);
  free(b);
}

bool load(int ntag, bool dirty, Set *a){
  if(a->max>a->lines.size){
    line *b = newline(ntag, dirty);
    list_append(&a->lines, *b);
    free(b);
    return true;
  }
  return false;
}

bool evict(Set *a){
  node_t* head = a->lines.head;
  bool dirty = head->val.dirty;
  list_delete(&a->lines, head->val);
  return dirty;
}

node_t* list_head(list_t *list) { return list->head; }
node_t* list_tail(list_t *list) { return list->tail; }

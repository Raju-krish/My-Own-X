#include <stdio.h>
#include <stdlib.h>
#include "my_list.h"

struct doubly_ll {
    void *data;
    struct doubly_ll *next;
    struct doubly_ll *prev;
};

struct doubly_ll *head = NULL;
struct doubly_ll *tail = NULL;

int insert_list(void *data)
{
    struct doubly_ll *temp = (struct doubly_ll *) malloc(sizeof(struct doubly_ll));
    if(temp == NULL) {
        printf("%s:%d Error Allocating memory\n", __func__, __LINE__);
        return FAILURE;
    }
    temp->data = data;
    if(head == NULL) {
        head = temp;
        tail = head;
        head->prev = NULL;
    }
    else {
        temp->prev = tail;
        temp->next = NULL;
        tail->next = temp;
        tail = tail->next;
    }
    return SUCCESS;
}

int clear_list()
{
    struct doubly_ll *temp = tail;
    while(temp != NULL){
        temp = tail->prev;
        free(tail);
        tail = temp;
    }
    return SUCCESS;
}

void for_each_entry(void func(void *data))
{
    struct doubly_ll *temp = head;

    while(temp != NULL) {
        func(temp->data);
        temp = temp->next;
    }
}

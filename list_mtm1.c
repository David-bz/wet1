#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "list_mtm1.h"

#define NULL_PARAMETER -1
#define INIT_VAL 0
#define IS_NULL(param,return_value) if(param==NULL) return return_value;

static ListResult init_node(List node,ListElement element,List first, List next, int iterator);



struct list_t{
    ListElement* element;
    struct list_t *first;
    struct list_t *next_node;
    int iterator;
    CopyListElement copyFunction;
    FreeListElement freeFunction;
};

static ListResult init_node(List node,ListElement element,List first, List next, int iterator) {
    assert(node->copyFunction && node->freeFunction);
    if (element) {
        node->element = node->copyFunction(element);
        IS_NULL(node->element, LIST_OUT_OF_MEMORY);
    }
    node->first = first;
    node->next_node = next;
    node->iterator = iterator;
    return LIST_SUCCESS;
}

List listCreate(CopyListElement copyElement, FreeListElement freeElement) {
    IS_NULL(copyElement,NULL)
    IS_NULL(freeElement,NULL)
    List new_list = malloc(sizeof(*new_list));
    if (new_list == NULL) {
        return NULL;
    }
    new_list->first=new_list;
    new_list->element = NULL;
    new_list->next_node = NULL;
    new_list->first = NULL;
    new_list->iterator = 0;
    new_list->copyFunction=copyElement;
    new_list->freeFunction=freeElement;
    return new_list;
}


List listCopy(List list){
    IS_NULL(list,NULL)
    List new_list=listCreate(list->copyFunction,list->freeFunction);
    if (new_list == NULL) {
        return NULL;
    }
    List tmp_new=new_list;
    List tmp_old=list;
    while(tmp_old){
        tmp_new->next_node=listCreate(tmp_old->copyFunction,tmp_old->freeFunction);
        if (tmp_new->next_node == NULL) {
            return NULL;
        }
        tmp_new=tmp_new->next_node;
        tmp_new->first=new_list;
        tmp_new->element=tmp_old->copyFunction(tmp_old->element);
        tmp_new->iterator=tmp_old->iterator;
        tmp_new->copyFunction=tmp_old->copyFunction;
        tmp_new->freeFunction=tmp_old->freeFunction;
        tmp_new->next_node=NULL;
        tmp_old=tmp_old->next_node;
    }
    return new_list;
}

int listGetSize(List list){
    IS_NULL(list,-1)
    int size=INIT_VAL;
    List tmp=list->first;
    while(tmp){
        if(tmp->element){
            size++;
        }
        tmp=tmp->next_node;
    }
    return size;
}


ListElement listGetFirst(List list){
    IS_NULL(list,NULL)
    assert(list->first);
    list=list->first;
    return(list->first->element);
}

ListElement listGetNext(List list){
    IS_NULL(list,NULL)
    IS_NULL(list->next_node,NULL)
    list=list->next_node;
    return list->next_node->element;
}

ListElement listGetCurrent(List list){
    IS_NULL(list,NULL)
    return list->element;
}

ListResult listInsertFirst(List list, ListElement element) {
    IS_NULL(list, LIST_NULL_ARGUMENT);
    list = list->first;
    List new_node = listCreate(list->copyFunction, list->freeFunction);
    IS_NULL(new_node, LIST_OUT_OF_MEMORY);
    new_node->element = new_node->copyFunction(element);
    IS_NULL(new_node->element, LIST_OUT_OF_MEMORY);
    new_node->next_node = list;
    while (list) {
        list->first = new_node;
        (list->iterator)++;
        list = list->next_node;
    }
    list = new_node;
    return LIST_SUCCESS;
}

ListResult listInsertLast(List list, ListElement element){
    IS_NULL(list, LIST_NULL_ARGUMENT);
    List new_node = listCreate(list->copyFunction, list->freeFunction);
    IS_NULL(new_node, LIST_OUT_OF_MEMORY);
    new_node->element = new_node->copyFunction(element);
    IS_NULL(new_node->element, LIST_OUT_OF_MEMORY);
    List tmp=list;
    while(tmp->next_node){
        tmp=tmp->next_node;
    }
    tmp->next_node=new_node;
    new_node->first=list->first;
    new_node->iterator=tmp->iterator+1;
    return LIST_SUCCESS;
}

ListResult listInsertBeforeCurrent(List list, ListElement element){
    IS_NULL(list, LIST_NULL_ARGUMENT);
    IS_NULL(list->element, LIST_INVALID_CURRENT);
    int current=list->iterator;
    if(current==0) {
        return listInsertFirst(list,element);
    }
    assert(current>0);
    List new_node = listCreate(list->copyFunction, list->freeFunction);
    IS_NULL(new_node, LIST_OUT_OF_MEMORY);
    List tmp=list->first;
    while(current > 0  && tmp->iterator == current-1){
        tmp=tmp->next_node;
    }
    tmp->next_node=new_node;
    ListResult r=init_node(new_node,element,list->first,list,current);
    if(r!=LIST_SUCCESS) { return r; }
    while(tmp){
        (tmp->iterator)++;
        tmp=tmp->next_node;
    }
    return LIST_SUCCESS;
}

ListResult listInsertAfterCurrent(List list, ListElement element){
    IS_NULL(list, LIST_NULL_ARGUMENT);
    IS_NULL(list->element, LIST_INVALID_CURRENT);
    







}

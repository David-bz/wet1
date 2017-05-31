#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "list_mtm1.h"

#define NULL_PARAMETER -1
#define INIT_VAL 0
#define IS_NULL(param,return_value) if(param==NULL) return return_value;

typedef struct node_t{
    ListElement* element;
    struct node_t* next_node;
}*Node;

struct list_t{
    Node iterator;
    Node first;
    Node last;
    int size;
    CopyListElement copyFunction;
    FreeListElement freeFunction;
};



static ListResult init_node(List list,Node new,ListElement element, Node next) {
    new->element=list->copyFunction(element);
    if(!new->element){
        free(new);
        return LIST_OUT_OF_MEMORY;
    }
    new->next_node = next;
    return LIST_SUCCESS;
}
/*
static ListResult reset_node(List node) {
    assert(node->copyFunction && node->freeFunction);
    node->freeFunction(node->element);
    node->first = NULL;
    node->next_node = NULL;
    node->list_index = 0;
    return LIST_SUCCESS;
}
*/
List listCreate(CopyListElement copyElement, FreeListElement freeElement) {
    IS_NULL(copyElement,NULL)
    IS_NULL(freeElement,NULL)
    List new_list = malloc(sizeof(*new_list));
    if (new_list == NULL) {
        return NULL;
    }
    new_list->first=NULL;
    new_list->iterator=NULL;
    new_list->last=NULL;
    new_list->size=0;
    new_list->copyFunction=copyElement;
    new_list->freeFunction=freeElement;
    return new_list;
}

List listCopy(List list){
    IS_NULL(list,NULL)
    Node iterator=list->iterator;
    List new_list=listCreate(list->copyFunction,list->freeFunction);
    Node keep=NULL;
    Node tmp=list->first;
    while (tmp){
        listInsertLast(new_list,tmp->element);
        if(list->iterator==iterator){
            keep=new_list->iterator;
        }
        if(new_list->size==1){
            new_list->first=new_list->iterator;
        }
        if(new_list->size==list->size){
            new_list->last=new_list->iterator;
        }
        tmp=tmp->next_node;
    }
    new_list->iterator=keep;
    list->iterator=iterator;
    return new_list;
}

int listGetSize(List list){
    IS_NULL(list,-1)
    assert(list->size >= 0);
    return list->size;
}


ListElement listGetFirst(List list){
    IS_NULL(list,NULL)
    list->iterator=list->first;
    if(list->iterator==NULL){return NULL;}
    return(list->iterator->element);
}

ListElement listGetNext(List list){
    IS_NULL(list,NULL)
    IS_NULL(list->iterator,NULL)
    list->iterator=list->iterator->next_node;
    if(list->iterator == NULL)
        return NULL;
    return list->iterator->element;
}

ListElement listGetCurrent(List list){
    IS_NULL(list,NULL)
    IS_NULL(list->iterator,NULL)
    return list->iterator->element;
}

ListResult listInsertFirst(List list, ListElement element) {
    IS_NULL(list, LIST_NULL_ARGUMENT);
    Node new_node=malloc(sizeof(*new_node));
    IS_NULL(new_node,LIST_OUT_OF_MEMORY);
    ListResult r=init_node(list,new_node,element,list->first);
    if(r!=LIST_SUCCESS){
        free(new_node);
        return LIST_OUT_OF_MEMORY;
    }
    list->size++;
    list->first=new_node;
    if(list->size == 1){
        list->iterator=new_node;
        list->last=new_node;
        return LIST_SUCCESS;
    }
    return LIST_SUCCESS;
}

ListResult listInsertLast(List list, ListElement element){
    IS_NULL(list, LIST_NULL_ARGUMENT);
    Node new_node=malloc(sizeof(*new_node));
    IS_NULL(new_node,LIST_OUT_OF_MEMORY);
    ListResult r=init_node(list,new_node,element,NULL);
    if(r!=LIST_SUCCESS){
        free(new_node);
        return LIST_OUT_OF_MEMORY;
    }
    if(list->size == 0){
        list->iterator=new_node;
        list->last=new_node;
        list->size++;
        list->first=new_node;
        return LIST_SUCCESS;
    }
    list->last->next_node=new_node;
    list->last=new_node;
    list->size++;
    return LIST_SUCCESS;
}

ListResult listInsertBeforeCurrent(List list, ListElement element){
    IS_NULL(list, LIST_NULL_ARGUMENT);
    IS_NULL(list->iterator, LIST_INVALID_CURRENT);
    if(list->iterator==list->first){
        return listInsertFirst(list,element);
    }
    Node new_node=malloc(sizeof(*new_node));
    IS_NULL(new_node,LIST_OUT_OF_MEMORY);
    ListResult r=init_node(list,new_node,element,list->iterator);
    if(r!=LIST_SUCCESS){
        free(new_node);
        return LIST_OUT_OF_MEMORY;
    }
    list->size++;
    Node tmp=list->first;
    while(tmp->next_node!=list->iterator){
            tmp=tmp->next_node;
    }
    tmp->next_node=new_node;
    return LIST_SUCCESS;
}

ListResult listInsertAfterCurrent(List list, ListElement element){
    IS_NULL(list, LIST_NULL_ARGUMENT);
    IS_NULL(list->iterator, LIST_INVALID_CURRENT);
    if(list->iterator==list->last){
        return listInsertLast(list,element);
    }
    Node new_node=malloc(sizeof(*new_node));
    IS_NULL(new_node,LIST_OUT_OF_MEMORY);
    ListResult r=init_node(list,new_node,element,list->iterator->next_node);
    if(r!=LIST_SUCCESS){
        free(new_node);
        return LIST_OUT_OF_MEMORY;
    }
    list->size++;
    list->iterator->next_node=new_node;
    return LIST_SUCCESS;
}

ListResult listRemoveCurrent(List list){
    IS_NULL(list, LIST_NULL_ARGUMENT)
    IS_NULL(list->iterator,LIST_INVALID_CURRENT)
    Node tmp=list->first;
    list->size--;
    if(tmp==list->iterator){
        list->first=list->first->next_node;
        list->freeFunction(list->iterator->element);
        list->iterator=NULL;
        return LIST_SUCCESS;
    }
    while (tmp->next_node != list->iterator){
        tmp=tmp->next_node;
    }
    tmp->next_node=tmp->next_node->next_node;
    list->freeFunction(list->iterator->element);
    list->iterator=NULL;
    return LIST_SUCCESS;
}
/*
static ListResult swap_elements(List list, ListElement element1,ListElement element2){
    ListElement temp=list->copyFunction(element1);
    IS_NULL(temp,LIST_OUT_OF_MEMORY);
    list->freeFunction(element1);
    element1=list->copyFunction(element2);
    if(!element1){
        list->freeFunction(temp);
        return LIST_OUT_OF_MEMORY;
    }
    list->freeFunction(element2);
    element2=list->copyFunction(temp);
    if(!element2){
        list->freeFunction(temp);
        return LIST_OUT_OF_MEMORY;
    }
    list->freeFunction(temp);
    return LIST_SUCCESS;
}

ListResult bubble_sort(int stop_at, List list,CompareListElements compareElement,bool* sorted) {
    bool swapped=false;
    List tmp=list;
    while(tmp->next_node->list_index <= stop_at) {
        if(compareElement(tmp->element,tmp->next_node->element) > 0) {
            ListResult r=swap_elements(list,tmp->element,tmp->next_node->element);
            if(r != LIST_SUCCESS){
                return r;
            }
            swapped=true;
        }
        tmp=tmp->next_node;
    }
    *sorted=!swapped;
    return LIST_SUCCESS;
}

ListResult listSort(List list, CompareListElements compareElement){
    IS_NULL(list, LIST_NULL_ARGUMENT);
    IS_NULL(compareElement, LIST_NULL_ARGUMENT);
    int stop_index=listGetSize(list)-1;
    bool sorted = false;
    while(!sorted){
        ListResult r=bubble_sort(stop_index,list->first,compareElement,&sorted);
        if(r != LIST_SUCCESS){
            return r;
        }
        stop_index--;
    }
    return LIST_SUCCESS;
}

List listFilter(List list, FilterListElement filterElement, ListFilterKey key){
    IS_NULL(list,NULL);
    IS_NULL(filterElement,NULL);
    List filtered=listCreate(list->copyFunction,list->freeFunction);
    IS_NULL(filtered,NULL);
    List tmp=list->first;
    int i=0;
    while(tmp){
        if(filterElement(tmp->element,key)){
            filtered->element=tmp->copyFunction(tmp->element);
            if(filtered->element == NULL) {
                listDestroy(filtered);
                return NULL;
            }
            if(filtered->first == NULL) {
                filtered->first = filtered;
            }
            filtered->next_node=filtered;
            filtered->list_index=i++;
        }
        tmp=tmp->next_node;
    }
    return filtered->first;
}

ListResult listClear(List list) {
    IS_NULL(list, LIST_NULL_ARGUMENT);
    List tmp = list->first;
    while(tmp){
        list->freeFunction(tmp->element);
        tmp->element=NULL;
        tmp=tmp->next_node;
    }
    return LIST_SUCCESS;
}

void listDestroy(List list){
    List tmp=list->first;
    listClear(list);
    while(tmp){
        tmp->first=NULL;
        tmp->freeFunction=NULL;
        tmp->copyFunction=NULL;
        List next=tmp->next_node;
        tmp->next_node=NULL;
        tmp=next;
    }
    free(list);
}
*/
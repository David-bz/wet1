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
static ListResult reset_node(List node) {
    assert(node->copyFunction && node->freeFunction);
    node->freeFunction(node->element);
    node->first = NULL;
    node->next_node = NULL;
    node->iterator = 0;
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
 int listGetIndex(List list){
    IS_NULL(list,-1);
    return list->iterator;
}

ListResult listInsertFirst(List list, ListElement element) {
    IS_NULL(list, LIST_NULL_ARGUMENT);
    if(list->first->element == NULL){
        ListResult r=init_node(list,element,list,NULL,0);
        return r;
    }
    List tmp = list->first;
    List new_node = listCreate(list->copyFunction, list->freeFunction);
    IS_NULL(new_node, LIST_OUT_OF_MEMORY);
    new_node->element = new_node->copyFunction(element);
    IS_NULL(new_node->element, LIST_OUT_OF_MEMORY);
    new_node->next_node = tmp;
    while (tmp) {
        list->first = new_node;
        (tmp->iterator)++;
        tmp = tmp->next_node;
    }
    tmp = new_node;
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
    /* Invalid state check??? */
    int current=list->iterator;
    if(current==0) {
        return listInsertFirst(list,element);
    }
    assert(current>0);
    List new_node = listCreate(list->copyFunction, list->freeFunction);
    IS_NULL(new_node, LIST_OUT_OF_MEMORY);
    List tmp=list->first;
    while(current > 0  && tmp->iterator < current-1){
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
    /* Invalid state check??? */
    int current=list->iterator;
    if(current == listGetSize(list)-1) {
        return listInsertLast(list,element);
    }
    List new_node = listCreate(list->copyFunction, list->freeFunction);
    IS_NULL(new_node, LIST_OUT_OF_MEMORY);
    init_node(new_node,element,list->first,list->next_node,current+1);
    list->next_node=new_node;
    List tmp=new_node->next_node;
    while(tmp){
        (tmp->iterator)++;
        tmp=tmp->next_node;
    }
    return LIST_SUCCESS;
}

ListResult listRemoveCurrent(List list){
    IS_NULL(list, LIST_NULL_ARGUMENT);
    /* Invalid state check??? */
    List tmp=list->first;
    while (tmp->iterator < list->iterator - 1){
        tmp=tmp->next_node;
    }
    if(list->iterator==0){
        tmp=tmp->next_node;
        list->first=tmp;
    }
    else if(list->next_node==NULL){
        tmp->next_node=NULL;
    }
    else{
        tmp->next_node=list->next_node->next_node;
    }
    reset_node(list);
    free(list);
    while(tmp->next_node){
        tmp->next_node->iterator--;
        tmp->next_node->first=tmp->first;
    }
    return LIST_SUCCESS;
}

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
    while(tmp->next_node->iterator <= stop_at) {
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
            filtered->iterator=i++;
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

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "list_mtm1.h"

#define IS_NULL(param,return_value) if(param==NULL) return return_value;

#define ASSERT(test_number, test_condition)  \
   if (!(test_condition)) {printf("\nTEST %s FAILED", test_number); } \
   else printf("\nTEST %s OK", test_number);
/********************
NOTE:
1. check what does "the iterator is at an invalid state" mean.
11

********************/

ListElement copyString(ListElement str) {
    assert(str);
    char* copy = malloc(strlen(str) + 1);
    return copy ? strcpy(copy, str) : NULL;
}

void freeString(ListElement str){
    free(str);
}


int main(){
    List new_list1=listCreate(copyString,NULL);
    ASSERT("1",new_list1==NULL)
    List new_list2=listCreate(NULL,freeString);
    ASSERT("2",new_list2==NULL)
    List new_list3=listCreate(copyString,freeString);
    ASSERT("3",new_list3!=NULL)
   // char* string=(char*)malloc(sizeof(6));
   // scanf("%s",string);
    ListResult r=listInsertFirst(new_list2,"Ole");
    ASSERT("4",r==LIST_NULL_ARGUMENT)
    r=listInsertFirst(new_list3,"Ole");
    ASSERT("5",r==LIST_SUCCESS)
    ASSERT("6",listGetSize(new_list3) == 1)
    r=listInsertFirst(new_list3,"OOOO");
    ASSERT("7",r==LIST_SUCCESS)
    ASSERT("8",listGetSize(new_list3) == 2)
    ASSERT("9",listGetIndex(new_list3) == 1)
    printf("\n%s...\n",(char*)listGetFirst(new_list3));
    printf("\n%s!\n",(char*)listGetCurrent(new_list3));
    ASSERT("10",listGetNext(new_list3) == NULL)
    r=listInsertAfterCurrent(new_list3,"ooo");


    return 0;

}
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define IS_NULL(param,return_value) if(param==NULL) return return_value;
/********************
NOTE:
1. check what does "the iterator is at an invalid state" mean.
11




********************/

int main(){
    int* x=NULL;
    IS_NULL(x,-1);
    IS_NULL(x,-1);
    int y=50;
    x=&y;
}
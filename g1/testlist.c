#include <stdlib.h>
#include <stdio.h>
#include "list.h"

int main(){
    printf("test\n");
    
    Listnode *node;
    /*
    node = (Listnode*)malloc(sizeof(Listnode));
    node->content = "first element";
    node->next = NULL;
    
    printf("length:%d\n",length(node));
    printf("head:%s\n",(char*)head(node));
    */
    append(&node,"test0");
    
    printf("length:%d - sould be 1\n",length(node));
    printf("head:%s - sould be test0\n",(char*)head(node));
    
    append(&node,"test1");
    
    printf("length:%d - sould be 2\n",length(node));
    printf("head:%s - sould be test0\n",(char*)head(node));
    
    append(&node,"test2");
    
    printf("length:%d - sould be 3\n",length(node));
    printf("head:%s - sould be test0\n",(char*)head(node));
    
    
    return 0;
    
}
#include <stdlib.h>
#include <stdio.h>
#include "list.h"

int main(){
    printf("test\n");
    
    Listnode *node = NULL;
    /*
    node = (Listnode*)malloc(sizeof(Listnode));
    node->content = "first element";
    node->next = NULL;
    
    printf("length:%d\n",length(node));
    printf("head:%s\n",(char*)head(node));
    */
    append(&node,"test0");
    
    printf("head:%s - sould be test0\n",(char*)head(node));
    printf("length:%d - sould be 1\n",length(node));
    
    append(&node,"test1");
    
    printf("head:%s - sould be test0\n",(char*)head(node));
    printf("length:%d - sould be 2\n",length(node));
    
    prepend(&node,"test2");
    
    printf("head:%s - sould be test2\n",(char*)head(node));
    printf("length:%d - sould be 3\n",length(node));
    
    reverse(&node);
    
    printf("head:%s - sould be test1\n",(char*)head(node));
    printf("length:%d - sould be 3\n",length(node));
    
    
    return 0;
    
}

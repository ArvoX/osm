#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "list.h"

int rmTrue(Data data){
	return 1;
}

int rmTest1(Data data){
	int i;
    printf("test1 != %s ->",(char*)data);	
	i = !strcmp("test0",(char*)data);		
    printf("%d\n",i);	
	
	return i;

}


void printlist(Listnode *start){
	printf("{");
    while (start != NULL) {    	
	    printf("%s ",(char*)start->content);
        start = start->next;
    }
	printf("}\n");
}


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
    append(&node,"test1");
    
    printf("head:%s - sould be test0\n",(char*)head(node));
    printf("length:%d - sould be 1\n",length(node));
    
    append(&node,"test2");
    
    printf("head:%s - sould be test0\n",(char*)head(node));
    printf("length:%d - sould be 2\n",length(node));
    
    prepend(&node,"test0");
    
    printf("head:%s - sould be test2\n",(char*)head(node));
    printf("length:%d - sould be 3\n",length(node));
    
    // Nu er listen {test0, test1, test2}
    
    printlist(node);    
    remv(&node,&rmTest1);


    printlist(node);        
    printf("length:%d - sould be 2\n",length(node));
    
    remv(&node,&rmTest1);
    printlist(node);
    printf("length:%d - sould be 2\n",length(node));

    remv(&node,&rmTrue);
    printlist(node);
    printf("length:%d - sould be 1\n",length(node));

    remv(&node,&rmTrue);
    printlist(node);
    printf("length:%d - sould be 0\n",length(node));

    remv(&node,&rmTrue);
    printlist(node);
    printf("length:%d - sould be 0\n",length(node));
		
    prepend(&node,"test0");	
    printlist(node);
    printf("length:%d - sould be 1\n",length(node));
    
    return 0;
    
}


#include<stdio.h>
#include<stdlib.h>
#include<sys/queue.h>
 
struct element{
    int num;
    /*LIST_ENTRY 创建了一个以element为名的结构体定义
     这个结构体含有向后指针和向前指针
     le_next 和 le_prev
     并且声明了一个类型为element的变量elements
     */
    LIST_ENTRY(element) elements;
};
int main(void){
    /*LIST_HEAD 会自动构建一个名为listh的结构体，
      里面含有一个element指针 *lh_first
      所以 先调用LIST_HEAD之后，才会生成struct声明，后面才可以使用
      关于listh的指针
     */
    LIST_HEAD(listh,element) head; 
    struct listh *headp;
    struct element *n1,*np;
    int i=0;
    LIST_INIT(&head);
    printf("开始\n");
    do{
 
        printf("输入一个数字，输入0则结束\n");
        scanf("%d",&i);
 
        n1 = (struct element*)malloc(sizeof(struct element));
        n1->num = i;
 
        /*
       对于LIST_INSERT_HEAD为何需要用指针域作为参数，
       因为指针域的名字是用户自己定义的，函数内部并不知道，
       所以需要作为参数传入，并且在宏之内，可以连接字符串实现
       类似反射的功能。
       */
 
        LIST_INSERT_HEAD(&head,n1,elements);
    }while(i);
    for(np=(head).lh_first;np!=NULL;np=np->elements.le_next){
        printf("%d ",np->num);
    }   
    printf("\n");
    return 0;
}

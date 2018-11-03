#include<stdio.h>
#include<stdlib.h>
//#include "ListHeader.h"
int main()
{

int a =5, b = 15, c = 20, d = 25, e = 30, g = 35;
int pos = 2, delepos = 2;// f = 40, z = 10, h = 45;
int midVal = 50;

printf("\n Insert at Head : %d\n", a);
insert_At_head(a);
printList();

printf("\nInsert at Tail : %d, %d, %d\n", b, c, d);
insert_At_tail(b);
insert_At_tail(c);
insert_At_tail(d);
printList();

printf("\nDelete Head\n");
delete_head();
printList();

printf("\nInsert at Head : %d\n",e);
insert_At_head(e);
//printList();

 printf("Insert at POSITION: %d, VALUE: %d\n", pos, midVal);
 insert_At_middle(midVal, pos);
 printList();

 printf("Delete TAIL\n");
 delete_tail();
 printList();

 printf("Insert at tail ");
 insert_At_tail(100);
 //insert_At_tail(200);
 //insert_At_tail(300);
 printList();

  printf("Delete from POSITION: %d\n", delepos);
  delete_middle(delepos);
  printList();
}
#include<stdio.h>

extern "C" int mySum(int a,int b)
{
int result;
printf("InServer");
result = a + b;
return(result);
}

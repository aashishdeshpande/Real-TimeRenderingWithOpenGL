#include<stdio.h>
#include<dlfcn.h>
#include<stdlib.h>

typedef int(*myPfn)(int,int);
myPfn pfn = NULL;

int main(void)
{

void *lib_handle;
int result;
int a,b;

lib_handle = dlopen("libSum.so",RTLD_LAZY);
if(lib_handle == NULL)
{
printf("Handle not obtained\n");
exit(EXIT_FAILURE);
}

pfn=(myPfn)dlsym(lib_handle,"mySum");
if(pfn == NULL)
{
printf("Function pointer not obtained\n");
exit(EXIT_FAILURE);
}

a=10;
b=20;

result=pfn(a,b);

printf("Sum is %d\n",result);

return(0);

}

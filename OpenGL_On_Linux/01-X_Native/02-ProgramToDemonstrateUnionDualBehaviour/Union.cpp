#include<iostream>
#include<stdlib.h>

using namespace std;
int main()
{

union myUnion{

char charValue;
int integerValue;
double doubleValue;
}firstValue;

//mu->charValue='a';



firstValue.charValue='a';

//void *ptr;


cout <<"charValue is:" <<firstValue.charValue<<"\n";
cout <<"Integer Value is:" <<firstValue.integerValue<<"\n";
cout <<"Size of Union is:"<<sizeof(firstValue)<<"\n";

firstValue.integerValue=10;

cout <<"IntegerValue is:"<<firstValue.integerValue<<"\n";
cout<<"charValue is:"<<firstValue.charValue<<"\n";

union myUnion mu=secondValue;

return 0;

}

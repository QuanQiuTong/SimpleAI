#include<stdio.h>
#include<assert.h>
int main(){
	perror("Try one?");//没啥用，库函数或者errno_t
    assert(("Wrong",0));
    return 0;
}
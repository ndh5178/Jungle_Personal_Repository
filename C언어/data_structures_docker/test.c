#include <stdio.h>
int test(){
    return NULL;
}

int main() {
    int a=5,b=25;
    a=test();
    if(a<b){
        printf("%d\n",b);
    }
    

    
    return 0;
}
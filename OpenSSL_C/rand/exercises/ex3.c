#include <stdio.h>

#include <openssl/rand.h>
#include <openssl/err.h>

void handle_errors(){
    ERR_print_errors_fp(stderr);
    abort();
}

int main(){
    int rand1,rand2;

    if(RAND_load_file("/dev/random",64)!=64){
        handle_errors();
    }

    if(RAND_bytes((unsigned char*) &rand1,4)!=1){
        handle_errors();
    }

    if(RAND_bytes((unsigned char*)&rand2,4)!=1){
        handle_errors();
    }

    int result=rand1+rand2;
    
}
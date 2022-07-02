#include <stdio.h>

#include <openssl/rand.h>
#include <openssl/err.h>

#define L 128

void handle_error(){
    ERR_print_errors_fp(stderr);
    abort();
}

void print_random_string(unsigned char *r){
    for(int i=0;i<L;i++){
        printf("%02x-",r[i]);
    }
    printf("\n");
}

int main(){
    unsigned char random1[L],random2[L];

    if(RAND_load_file("/dev/random",64)!=64){
        handle_error();
    }

    if(RAND_bytes(random1,L)!=1){
        handle_error();
    }
    printf("Random 1: ");
    print_random_string(random1);

    if(RAND_bytes(random2,L)!=1){
        handle_error();
    }
    printf("Random 2: ");
    print_random_string(random2);

    unsigned char result[L];

    for(int i=0;i<L;i++){
        result[i]=random1[i]^random2[i];

    }
    printf("Result: ");
    print_random_string(result);


}
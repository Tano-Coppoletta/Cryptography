#include <stdio.h>

#include <openssl/rand.h>
#include <openssl/err.h>

#define MAX 128

void handle_errors(){
    ERR_print_errors_fp(stderr);
    abort();
}

int main(){
    unsigned char random1[MAX];
    unsigned char random2[MAX];
    unsigned char result[MAX];

   if(RAND_load_file("/dev/random",64) != 64) 
        handle_errors();
       

    //generate first string
    if(RAND_bytes(random1,MAX)!=1){
        handle_errors();
    }
    //generate second string
    if(RAND_bytes(random2,MAX)!=1){
        handle_errors();
    }

    printf("first string: ");
    for(int i=0;i<MAX;i++){
        printf("%02x",random1[i]);
    }
    printf("\nSecond string: ");
    for(int i=0;i<MAX;i++){
        printf("%02x",random2[i]);
    }

    //bitwise XOR
    printf("\nresult string: ");
    for(int i=0;i<MAX;i++){
        result[i]=random1[i]^random2[i];
    }

    for(int i=0;i<MAX;i++){
        printf("%02x",result[i]);
    }
}
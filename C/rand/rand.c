#include <stdio.h>

#include <openssl/rand.h>

#define MAX 128

int main(){
    unsigned char random_string[MAX];
    RAND_bytes(random_string,MAX);

    for(int i=0;i<MAX;i++)
        printf("%02x-",random_string[i]);
    printf("\n");

    return 0;
}

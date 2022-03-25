#include <stdio.h>
#include <openssl/evp.h>

#define ENCRYPT 1
#define DECRYPT 0

int main(){

    EVP_CIPHER_CTX *ctx= EVP_CIPHER_CTX_new();

    unsigned char key[]="1234567890abcdef"; //ASCII
    unsigned char iv[]="abcdef1234567890"; //ASCII

    EVP_CipherInit(ctx,EVP_aes_128_cbc(),key,iv,ENCRYPT);



    return 0;
}
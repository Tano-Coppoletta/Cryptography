#include <stdio.h>
#include <openssl/evp.h>
#include <string.h>

#define ENCRYPT 1
#define DECRYPT 0

int main(){
ssss
    EVP_CIPHER_CTX *ctx= EVP_CIPHER_CTX_new();

    unsigned char key[]="1234567890abcdef"; //ASCII
    unsigned char iv[]="abcdef1234567890"; //ASCII
    unsigned char ciphertext[]="13713c9b8081468892c518592730b3496d2c58ed3a9735d90788e7c24e8d324d75f6c9f5c6e43ee7dccad4a3221d697e";

    EVP_CipherInit(ctx,EVP_aes_128_cbc(),key,iv,DECRYPT);

    unsigned char plaintext[strlen(ciphertext)/2];
    unsigned char ciphertext_bin[strlen(ciphertext)/2];

    for(int i=0;i<strlen(ciphertext)/2;i++){
        sscanf(&ciphertext[2*i],"%2hhx",&ciphertext_bin[i]);
    }

    int lenght;
    int plaintext_len=0;
    //lenght contains the size of data already processed
    EVP_CipherUpdate(ctx, plaintext, &lenght,ciphertext_bin, strlen(ciphertext)/2);

    printf("After update: %d\n",lenght);
    plaintext_len+=lenght;

    EVP_CipherFinal(ctx, plaintext+plaintext_len, &lenght);

    printf("After final: %d\n",lenght);
    plaintext_len+=lenght;

    EVP_CIPHER_CTX_free(ctx);

    printf("size of ciphertext: %d\n",plaintext_len);

    plaintext[plaintext_len]='\0';
    printf("Plaintext: %s\n",plaintext);

    return 0;
}

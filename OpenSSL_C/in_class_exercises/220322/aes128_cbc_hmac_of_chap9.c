#include <stdio.h>
#include <openssl/err.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <string.h>



void handle_errors(){
    ERR_print_errors_fp(stderr);
    abort();
}

int main(){

    unsigned char key[16];
    unsigned char iv[16];
    unsigned char h_key[32];

    for(int i=0 ;i<16;i++){
        key[i]=0x11;
        iv[i]=0x22;
    }

    for(int i=0;i<32;i++){
        h_key[i]=0x33;
    }

    FILE *fin = fopen("chap9.pdf","r");
    if(fin==NULL){
        fprintf(stderr,"Error opening the file\n");
        exit(1);
    }

    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    EVP_MD_CTX *hmac_ctx = EVP_MD_CTX_new();

    EVP_PKEY *hkey = EVP_PKEY_new_mac_key(EVP_PKEY_HMAC,NULL,h_key, 16);

    if(!EVP_DigestSignInit(hmac_ctx,NULL,EVP_sha1(),NULL,hkey))
        handle_errors();
    
    size_t n;
    unsigned char buffer[1024];
    while((n = fread(buffer,1,1024,fin))>0){
        if(!EVP_DigestSignUpdate(hmac_ctx,buffer,n))
            handle_errors();
    }

    unsigned char hmac_value[EVP_MD_size(EVP_sha1())];
    size_t hmac_len;

    if(!EVP_DigestSignFinal(hmac_ctx,hmac_value,&hmac_len))
        handle_errors();
    
    EVP_MD_CTX_free(hmac_ctx);

    printf("The HMAC is : ");
    for(int i=0 ;i < hmac_len;i++){
        printf("%02x",hmac_value[i]);
    }
    printf("\n");


    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if(!EVP_CipherInit(ctx, EVP_aes_128_cbc(), key, iv, 1))
        handle_errors();

    int length;
    unsigned char ciphertext[EVP_MD_size(EVP_sha1())];

    fclose(fin);


    if(!EVP_CipherUpdate(ctx,ciphertext,&length,hmac_value,EVP_MD_size(EVP_sha1())))
        handle_errors();

    if(!EVP_CipherFinal(ctx,ciphertext, &length))
        handle_errors();

    printf("length: %d\n",length);

    EVP_CIPHER_CTX_free(ctx);

    printf("The ciphertext is: ");
    for(int i =0; i<length;i++){
        printf("%02x",ciphertext[i]);
    }
    printf("\n");

    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();

}
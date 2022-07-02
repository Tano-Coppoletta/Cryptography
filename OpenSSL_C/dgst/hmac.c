#include <stdio.h>
#include <string.h>
#include <openssl/err.h>
#include <openssl/hmac.h>

#include <openssl/evp.h>

#define MAXBUF 1024

void handle_errors(){
    ERR_print_errors_fp(stderr);
    abort();
}

int main(int argc, char *argv[]){

    if(argc!=2){
        fprintf(stderr,"Invalid parameters num. Usage: %s string_to_hash\n",argv[0]);
        exit(-1);
    }

    FILE *fp;
    if((fp=fopen(argv[1],"r"))==NULL){
        printf(stderr,"Could not open the file\n");
        exit(-1);

    }


    //Load the human readable error strings for libcrypto
    ERR_load_crypto_strings();
    //Load all digest and chiper algorithms
    OpenSSL_add_all_algorithms();

    unsigned char key[]="1234567887654321"; //16 ASCII char
    EVP_PKEY *hmac_key= EVP_PKEY_new_mac_key(EVP_PKEY_HMAC, NULL, key, strlen(key));

    //create the contex
    EVP_MD_CTX *hmac_ctx=EVP_MD_CTX_new();
    
    if(! EVP_DigestSignInit(hmac_ctx, NULL, EVP_sha1(),NULL,hmac_key)){
        handle_errors();
    }

    int n_read;
    unsigned char buffer[MAXBUF];
    while((n_read=fread(buffer,1,MAXBUF,fp))>0){
        if(! EVP_DigestSignUpdate(hmac_ctx, buffer, n_read))
            handle_errors();
    }

    unsigned char hmac_value[HMAC_size(hmac_ctx)];
    int hmac_len;

    if(! EVP_DigestSignFinal(hmac_ctx, hmac_value, &hmac_len))
        handle_errors();

    EVP_MD_CTX_free(hmac_ctx);

    printf("The HMAC is: ");

    for(int i=0;i<hmac_len;i++){
        printf("%02x",hmac_value[i]);
    }
    printf("\n");

    //VERIFICATION
    unsigned char hmac[]="f538b41ded1a72b0168dd8fb6c78d98b28b13016";
    unsigned char hmac_binary[strlen(hmac)/2];

    for(int i=0; i<strlen(hmac)/2; i++){
        sscanf(&hmac[2*i],"%2hhx",&hmac_binary[i]);
    }

    //lenght of hmac, actual comparison of the buffers
    if( (hmac_len== strlen(hmac)/2) && (CRYPTO_memcmp(hmac_binary,hmac_value,hmac_len)==0)){
        printf("Verification successful\n");
    }else
        printf("Verification failure\n");

    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();
}

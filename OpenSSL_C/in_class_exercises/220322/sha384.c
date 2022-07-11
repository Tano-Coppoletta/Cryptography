#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>

#define MAXBUF 1024

void handle_errors(){
    ERR_print_errors_fp(stderr);
    abort();
}

int main(int argc,char * argv){
    unsigned char *string = "Cryptography class 22-03-22";

    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();


    EVP_MD_CTX *md = EVP_MD_CTX_new();

    if(!EVP_DigestInit(md, EVP_sha384()))
        handle_errors();
    
    int len = strlen(string);

    if(!EVP_DigestUpdate(md, string, len)){
        handle_errors();
    }

    unsigned char md_value[EVP_MD_size(EVP_sha384())];
    int md_len;
    if(!EVP_DigestFinal(md, md_value, &md_len)){
        handle_errors();
    }

    printf("The digest is: \n");
    for(int i=0;i< md_len;i++){
        printf("%02x",md_value[i]);
    }

    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();

    return 0;

}
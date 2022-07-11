#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/err.h>
#include <string.h>

#define MAXBUF 1024

void handle_errors(){
    ERR_print_errors_fp(stderr);
    abort();
}

//the hash function is applied to:
// key || "keyed" || file || "keyed" || key

int main(){

    FILE *fin = fopen("chap9.pdf","r");
    if(fin==NULL){
        fprintf(stderr,"Error opening a file\n");
        exit(1);
    }    

    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    EVP_MD_CTX *md = EVP_MD_CTX_new();

    unsigned char key[32];
    for(int i=0;i<32;i++){
        key[i]= 0x88; //0xFF 0x11 0xF0
    }

    if(!EVP_DigestInit(md, EVP_sha1()))
        handle_errors();

    if(!EVP_DigestUpdate(md,key,32))
        handle_errors();

    //add keyed
    unsigned char buffer[MAXBUF];
    strcpy(buffer,"keyed");

    if(!EVP_DigestUpdate(md,buffer,5))
        handle_errors();

    int n_read;
    while((n_read = fread(buffer,1, MAXBUF,fin)) > 0){
        if(!EVP_DigestUpdate(md,buffer,n_read))
            handle_errors();
    }

    //insert keyed again
    strcpy(buffer,"keyed");

    if(!EVP_DigestUpdate(md,buffer,5))
        handle_errors();

    //insert again the key
    if(!EVP_DigestUpdate(md,key,32))
        handle_errors();

    


    unsigned char md_value[EVP_MD_size(EVP_sha1())];
    int md_len;

    if(!EVP_DigestFinal(md, md_value, &md_len))
        handle_errors();

    printf("The digest is \n");
    for(int i=0;i< md_len;i++){
        printf("%02x",md_value[i]);
    }

    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();
    

}
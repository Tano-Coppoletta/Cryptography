#include <stdio.h>
#include <string.h>
#include <openssl/err.h>

#include <openssl/evp.h>

#define MAXBUF 1024

void handle_errors(){
    ERR_print_errors_fp(stderr);
    abort();
}

//First parameter is the name of the file to hash
int main(int argc, char *argv[]){
   // char message[]="This is the message to hash!!!";

    if(argc!=2){
        fprintf(stderr,"Invalid parameters num. Usage: %s string_to_hash\n",argv[0]);
        exit(-1);
    }

    FILE *fp;
    if((fp=fopen(argv[1],"r"))==NULL){
        printf(stderr,"Could not open the file\n");
        exit(-1);

    }

    EVP_MD_CTX *md;

    //good practice
    //Load the human reada ble error strings for libcrypto
    ERR_load_crypto_strings();
    //Load all digest and chiper algorithms
    OpenSSL_add_all_algorithms();


    md=EVP_MD_CTX_new();

    if(EVP_DigestInit(md, EVP_sha1())==0)
        handle_errors();

    unsigned char buffer[MAXBUF];
    int n_read;
    while((n_read=fread(buffer,1,MAXBUF,fp))>0){
        if(EVP_DigestUpdate(md,buffer,n_read)==0)
            handle_errors();

    }


    unsigned char md_value[EVP_MD_size(EVP_sha1())];
    int md_len;

    EVP_DigestFinal(md,md_value,&md_len);

    EVP_MD_CTX_free(md);

    //Completely free al the cipher data
    CRYPTO_cleanup_all_ex_data();
    //Remove error string
    ERR_free_strings();

    printf("The digest is:\n");

    for(int i=0;i<md_len;i++){
        printf("%02x",md_value[i]);
    }
    printf("\n");

    return 0;
}
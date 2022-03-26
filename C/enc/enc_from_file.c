#include <stdio.h>
#include <openssl/evp.h>
#include <string.h>
#include <openssl/err.h>

#define ENCRYPT 1
#define DECRYPT 0

#define MAXSIZE 1024

void handle_errors(){
    ERR_print_errors_fp(stderr);
    abort();
}

//argv[1] -->  input file
//argv[2] --> key (hexstring)
//argv[3] --> IV (hexstring)
//argv[4] --> output file
//save in a buffer in memory the result of encryption

int main(int argc,char* argv[]){

    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();


    EVP_CIPHER_CTX *ctx= EVP_CIPHER_CTX_new();

    if(argc!=5){
        fprintf(stderr,"Invalid parameters. Usage: %s input_file key IV output_file\n",argv[0]);
        exit(1);
    }

    FILE *f_in;

    if((f_in=fopen(argv[1],"r"))==NULL){
        fprintf(stderr,"Errors opening the input file %s\n",argv[1]);
        exit(1);
    }

    FILE *f_out;

    if((f_out=fopen(argv[4],"wb"))==NULL){
        fprintf(stderr,"Errors opening the input file %s\n",argv[4]);
        exit(1);
    }

    //check key lenght
    if(strlen(argv[2])/2 !=32){
        fprintf(stderr,"Wrong key leght: %s\n",argv[2]);
        exit(1);
    }

    

    unsigned char key[strlen(argv[2])/2];
    for(int i=0;i< strlen(argv[2])/2; i++)
        sscanf(&argv[2][2*1],"%2hhx",&key[i]);


    //check IV lenght
    if(strlen(argv[3])/2 !=32){
        fprintf(stderr,"Wrong IV leght: %s\n",argv[3]);
        exit(1);
    }

    unsigned char iv[strlen(argv[3])/2];
    for(int i=0;i< strlen(argv[3])/2; i++)
        sscanf(&argv[3][2*1],"%2hhx",&iv[i]);


    if(! EVP_CipherInit(ctx, EVP_aes_128_cbc(), key, iv, ENCRYPT))
        handle_errors();

    int n_read;
    unsigned char buffer[MAXSIZE];
    unsigned char ciphertext[MAXSIZE + 16];
    int lenght, ciphertext_len=0;

    while((n_read=fread(buffer,1,MAXSIZE,f_in))>0){

        //if you risk to overflow the ciphertext variable you exit before with an error
        // n_read +1 block >left in ciphertext( MAX -ciphertext_len)
        /*if(ciphertext_len>100*MAXSIZE -n_read -EVP_CIPHER_CTX_block_size(ctx)){
            fprintf(stderr,"The file to cipher is larger than expected\n");
            exit(1);
        }*/

        if(! EVP_CipherUpdate(ctx,ciphertext,&lenght,buffer,n_read))
            handle_errors();
        ciphertext_len+=lenght;

        if(fwrite(ciphertext,1,lenght,f_out)<lenght){
            fprintf(stderr,"Error writing in output file\n");
            abort();
        }
        
    }

    if(! EVP_CipherFinal(ctx, ciphertext,&lenght))
        handle_errors();
    
    if(fwrite(ciphertext,1,lenght,f_out)<lenght){
            fprintf(stderr,"Error writing in output file\n");
            abort();
        }
    
    ciphertext_len+=lenght;

    EVP_CIPHER_CTX_free(ctx);

    printf("Ciphertext lenght: %d\n",ciphertext_len);

    

    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();

    fclose(f_in);
    fclose(f_out);

    return 0;
}
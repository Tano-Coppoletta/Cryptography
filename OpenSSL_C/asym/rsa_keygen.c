#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/bn.h>
#include <stdio.h>
#include <string.h>

void handle_errors(){
    ERR_print_errors_fp(stderr);
    abort();
}

int main(){
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    RSA *rsa_keypair;
    BIGNUM *bne=BN_new();
    if(!BN_set_word(bne, RSA_F4))
        handle_errors();

    rsa_keypair=RSA_new();
    
    if(!RSA_generate_key_ex(rsa_keypair, 2048, bne, NULL))
        handle_errors();

    FILE *rsa_file;
    //save the private key
    if((rsa_file=fopen("private.pem","w"))==NULL){
        fprintf(stderr,"Problems creating the file\n");
        abort();
    }

    if(!PEM_write_RSAPrivateKey(rsa_file,rsa_keypair, NULL, NULL, 0, NULL, NULL))
        handle_errors();
    
    fclose(rsa_file);

    //save the public key
    if((rsa_file=fopen("public.pem","w"))==NULL){
        fprintf(stderr,"Problems creating the file\n");
        abort();
    }

    if(!PEM_write_RSA_PUBKEY(rsa_file,rsa_keypair))
        handle_errors();

    fclose(rsa_file);

    ///////////////end of key pair generation//////////////////////

    //////////////encrypt////////////////////////////////////////
    unsigned char msg[]="This is the message to encrypt\n";
    //the output has a lenght equal to the length of the key
    unsigned char encrypted_msg[RSA_size(rsa_keypair)];
    int encrypted_len;
    //it uses the public key to encrypt
    if((encrypted_len = RSA_public_encrypt(strlen(msg)+1, msg, encrypted_msg, rsa_keypair, RSA_PKCS1_OAEP_PADDING))==-1){
        handle_errors();
    }

    FILE *out;

        if((out=fopen("encrypted.enc","w"))==NULL){
        fprintf(stderr,"Problems creating the file\n");
        abort();
    }

    if(fwrite(encrypted_msg,1 ,encrypted_len, out)< encrypted_len){
        fprintf(stderr,"Problems saving into file\n");
        abort();
    }

    fclose(out);
    printf("File saved!\n");


    /////////////////////decrypt/////////////////////////////////
    printf("I'm reading the encrypted file\n");

    //read the encrypted file
    FILE *in;
    if((in=fopen("encrypted.enc","r"))==NULL){
        fprintf(stderr,"Error reading the file\n");
        abort();
    }

    if((encrypted_len= fread(encrypted_msg, 1 , RSA_size(rsa_keypair),in))!= RSA_size(rsa_keypair)){
        handle_errors();
    }
    fclose(in);

    unsigned char decrypted_msg[RSA_size(rsa_keypair)];

    if(RSA_private_decrypt(encrypted_len, encrypted_msg, decrypted_msg, rsa_keypair, RSA_PKCS1_OAEP_PADDING)==-1)
        handle_errors();

    printf("Decrypted message: %s\n",decrypted_msg);
    //free everything

    RSA_free(rsa_keypair);

    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();
}
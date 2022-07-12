#include <stdio.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/bn.h>
#include <string.h>

int envelop_MAC(RSA *rsa_keypair, char *message, int message_len, char *key, int keylength, char *result);


void handle_errors(){
    ERR_print_errors_fp(stderr);
}

int main(){

    RSA *rsa_keypair = NULL;
    int bits = 4096;
    unsigned long e =RSA_F4;

    BIGNUM *bne = BN_new();

    if(!BN_set_word(bne,e))
        handle_errors();

    rsa_keypair = RSA_new();

    if(!RSA_generate_key_ex(rsa_keypair,bits,bne,NULL))
        handle_errors();
    
    unsigned char msg[] = "This is a message";
    unsigned char key[]="11111111111111111111111111111111";
    unsigned char result[RSA_size(rsa_keypair)];
    envelop_MAC(rsa_keypair,msg,strlen(msg),key, strlen(key),result);
}

int envelop_MAC(RSA *rsa_keypair, char *message, int message_len, char *key, int keylength, char *result){
    EVP_MD_CTX *md = EVP_MD_CTX_new();

    if(!EVP_DigestInit(md, EVP_sha256())){
        handle_errors();
        return -1;
    }
    
    if(!EVP_DigestUpdate(md,message,message_len)) {
        handle_errors();
        return -1;
    }

    if(!EVP_DigestUpdate(md,key,keylength)){
        handle_errors();
        return -1;
    }
    
    unsigned char md_value[EVP_MD_size(EVP_sha256())];
    int md_len;

    if(!EVP_DigestFinal(md,md_value,&md_len)){
        handle_errors();
        return -1;
    }

    
    EVP_MD_CTX_free(md);

    md = EVP_MD_CTX_new();

    if(!EVP_DigestInit(md,EVP_sha256())){
        handle_errors();
        return -1;
    }
    if(!EVP_DigestUpdate(md,md_value,EVP_MD_size(EVP_sha256()))){
        handle_errors();
        return -1;
    }
    
    unsigned char double_hashed[EVP_MD_size(EVP_sha256())];
    int double_hashed_len;
    if(!EVP_DigestFinal(md,double_hashed, &double_hashed_len)){
        handle_errors();
        return -1;
    }

    //encrypt with rsa the result of the double sha256

    int encrypted_len;
    unsigned char encrypted[RSA_size(rsa_keypair)];
    if((encrypted_len = RSA_public_encrypt(double_hashed_len,double_hashed,encrypted,rsa_keypair,RSA_PKCS1_OAEP_PADDING))==-1){
        handle_errors();
        return -1;
    }
    strcpy(result,encrypted);

    return 0;
    

}
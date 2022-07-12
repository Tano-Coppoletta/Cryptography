#include <stdio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>


void handle_errors(){
    ERR_print_errors_fp(stderr);
    abort();
}

#define RANDL 32

int main(){
    unsigned char r1[RANDL],r2[RANDL];

    if(RAND_load_file("/dev/random",32)!=32)
        handle_errors();
    
    if(!RAND_bytes(r1,RANDL))
        handle_errors();
    
    if(!RAND_bytes(r2,RANDL))
        handle_errors();
    
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    EVP_MD_CTX *md = EVP_MD_CTX_new();

    if(!EVP_DigestInit(md,EVP_sha256()))
        handle_errors();
    
    unsigned char key[EVP_MD_size(EVP_sha256())];
    

    if(!EVP_DigestUpdate(md,r1,strlen(r1)))
        handle_errors();
    if(!EVP_DigestUpdate(md,"key",3))
        handle_errors();

    if(!EVP_DigestUpdate(md,r2,strlen(r2)))
        handle_errors();

    int md_len;
    if(!EVP_DigestFinal(md, key, &md_len))
        handle_errors();

    printf("The digest is: ");
    for(int i = 0; i < md_len; i++)
	    printf("%02x", key[i]);
        printf("\n");

    RSA *rsa_keypair = NULL;
    int bits = 4096;
    unsigned long e =RSA_F4;

    BIGNUM *bne = BN_new();

    if(!BN_set_word(bne,e))
        handle_errors();

    rsa_keypair = RSA_new();

    if(!RSA_generate_key_ex(rsa_keypair,bits,bne,NULL))
        handle_errors();

    unsigned char encrypted_key[RSA_size(rsa_keypair)];
    int encrypted_len;
    if((encrypted_len = RSA_public_encrypt(md_len,key,encrypted_key,rsa_keypair, RSA_PKCS1_OAEP_PADDING))==-1)
        handle_errors();

    fwrite(encrypted_key,1, RSA_size(rsa_keypair),stdout);


}
#include <stdio.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <openssl/pem.h>

void handle_errors(){
    ERR_print_errors_fp(stderr);
    abort();
}

#define L 16

int main(){
    unsigned char rand1[L],rand2[L];

    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    if(RAND_load_file("/dev/random",16)!=16)
        handle_errors();
    
    if(!RAND_bytes(rand1,L))
        handle_errors();

    if(!RAND_bytes(rand2,L))
        handle_errors();
    
    BIGNUM *bn1 = BN_new();
    BIGNUM *bn2 = BN_new();

    BN_hex2bn(bn1, rand1);
    BN_hex2bn(bn2,rand2);

    BIGNUM *s1 = BN_new();
    BIGNUM *s2 = BN_new();
    BIGNUM *mod = BN_new();

    BN_set_word(mod,2^128);

    BN_CTX *bctx = BN_CTX_new();

    BN_add(s1,bn1,bn2); // s1 = rand1 + rand2
    BN_sub(s2,bn1,bn2); // s2 = rand1 - rand2


    BIGNUM *k1 = BN_new();
    BIGNUM *k2 = BN_new();

    BN_mod_mul(k1,s1,s2,mod,bctx);

    BN_mod_mul(s1,bn1,bn2,mod,bctx); // s1 = rand1*rand2

    BN_div(s1,k2,s1,s2,bctx); 

    BN_mod(k2,s1,mod,bctx);

    unsigned char iv[]  = "1111111111111111";

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    EVP_CipherInit(ctx,EVP_aes_128_cbc(),k1,iv,1);

    unsigned char ciphertext[48];
    int ciphertext_l;
    EVP_CipherUpdate(ctx,ciphertext, &ciphertext_l, k2, 16);
    int final_len;
    EVP_CipherFinal(ctx, ciphertext+ciphertext_l,&final_len);

    EVP_CIPHER_CTX_free(ctx);

    //generate an RSA keypair with a 2048 bit modululs


    RSA *rsa_keypair = NULL;
    BIGNUM *bne = BN_new();

    int bits=2048;
    unsigned long e= RSA_F4;

    if(!BN_set_word(bne,e))
        handle_errors();

    rsa_keypair = RSA_new();
    if(!RSA_generate_key_ex(rsa_keypair,bits,bne,NULL))
        handle_errors();
    
    unsigned char cip2[RSA_size(rsa_keypair)];
    int encrypted_len;

    if((encrypted_len = RSA_public_encrypt(16, ciphertext, cip2, rsa_keypair, RSA_PKCS1_OAEP_PADDING))==-1)
        handle_errors();

    

}
#include <stdio.h>
#include <openssl/err.h>
#include <openssl/bn.h>

void handle_errors(){
    ERR_print_errors_fp(stderr);
    abort();
}

int main(){
    BIGNUM *a=BN_new();
    BIGNUM *b=BN_new();
    BIGNUM *c = BN_new();
    BIGNUM *d =BN_new();
    BIGNUM *res=BN_new();
    unsigned char a_string[] = "11111111111111111111111111111111";
    unsigned char b_string[] = "22222222222222222222222222222222";
    unsigned char c_string[]= "3333";
    unsigned char d_strig[]= "2341234123412341234";

    BN_hex2bn(&a,a_string);
    BN_hex2bn(&b,b_string);
    BN_hex2bn(&c,c_string);
    BN_hex2bn(&d,d_strig);

    BN_print_fp(stdout,a);
    printf("\n");
    BN_print_fp(stdout,b);
    printf("\n");
    BN_print_fp(stdout,c);
    printf("\n");
    BN_print_fp(stdout,d);
    printf("\n");

    BN_CTX *ctx = BN_CTX_new();
    //compute (a+b)^c(mod d)
    
    BN_add(res,a,b);//a+b
    BIGNUM *res2 = BN_new();
    BN_print_fp(stdout,res);

    printf("\n");
    
    //(a+b)^c mod d
    if(!BN_mod_exp(res2,res,c,d,ctx))
        handle_errors();
    
    
    BN_print_fp(stdout,res2);
    printf("\n");

    BN_CTX_free(ctx);
}
#include <stdio.h>
#include <openssl/bn.h>

int main()
{
  // a newly instantiated BIGNUM is initialized to 0
  BIGNUM *b1=BN_new();
  BIGNUM *b2=BN_new();
  BIGNUM *b3=BN_new();
  BIGNUM *b4=BN_new();
  BIGNUM *res=BN_new();
  BIGNUM *res2=BN_new();

  BN_CTX *ctx=BN_CTX_new();

//   char hex_string1[]={"11111111111111111111111111111111"};
//   char hex_string2[]={"22222222222222222222222222222222"};
//   char hex_string3[]={"12341234123412341234"};
//   char hex_string4[]={"3333"};


  BN_hex2bn(&b1,"11111111111111111111111111111111");
  BN_print_fp(stdout,b1);
  printf("\n");

  BN_hex2bn(&b2,"22222222222222222222222222222222");
  BN_print_fp(stdout,b2);
  printf("\n");

  BN_hex2bn(&b3,"12341234123412341234");
  BN_print_fp(stdout,b3);
  printf("\n");

  BN_hex2bn(&b4,"3333");
  BN_print_fp(stdout,b4);
  printf("\n");

  //res= b1+b2
  BN_add(res,b1,b2);
  BN_print_fp(stdout,res);
  printf("\n");

  //res2=(b1+b2)^b3 (mod b4)
  BN_mod_exp(res2,res,b3,b4,ctx);
  
  // print in hex format
  BN_print_fp(stdout,res2);
  printf("\n");

  BN_free(b1);
  BN_free(b2);
  BN_free(b3);
  BN_free(b4);
  BN_free(res);
  BN_free(res2);

  BN_CTX_free(ctx);

  return 0;
}
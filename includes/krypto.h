#ifndef __KRYPTO_H__
#define __KRYPTO_H__

#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<assert.h>

#include"ripemd160.h"
#include"ed25519/ed25519.h"

#define fcase case
#define case break;case
#define elif else if

#define SI static inline
#define ERR(...) exit(((__VA_ARGS__),1))
#define putstr(s) fputs((s), stdout)

typedef unsigned char uchar;
typedef unsigned long ulong;
typedef unsigned  int uint;

SI int hexdigest(char*in, int in_len, char*out){
  for (int n = 0; n < in_len; n++)
    sprintf(2*n+out, "%02x", (unsigned char)in[n]);
  return 2*in_len;}

SI int hexdigestn(char*in, int in_len, char*out){
  int ret = hexdigest(in, in_len, out);
  strcpy(out+2*in_len, "\n");
  return ret+1;}

#endif//__KRYPTO_H__

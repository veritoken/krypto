#include <emscripten/bind.h>
#include<emscripten.h>
extern "C" {
#include "ed25519/ed25519.h"
}

using namespace emscripten;

float lerp(float a, float b, float t) {
    return (1 - t) * a + t * b;
}

float lerp2(float a, float b, float t) {
    return (1 - t) * a + t * b;
}

#include<stdio.h>

void forkOff(int s,int y) {
  printf("WOO HOO! [%d], %d\n", s, y);
  return;
}

extern "C"
const char* hello_world(){
  return "hello, world!";
}

unsigned char public_key[32], private_key[64], seed[32]="";
unsigned char signature[64];
//unsigned char xsignature[1024];

extern "C"
const char* hello_to(char*who){
  static char buf[1024];
  sprintf(buf, "hello, %s!", who);
  ed25519_create_keypair(public_key,
			 private_key,
			 seed);
  printf("hello to\n");
  return buf;
}

#define DUMP(s,m)				\
  for(int n=0;n<m;n++)				\
    printf("%02x", s[n]),			\
      printf((n+1)%32?" ":"\n");

#define XDUMP(s,m)				\
  for(int n=0;n<m;n++)				\
    b+=sprintf(b,"%02x", (s)[n]),		\
      b+=sprintf(b,(n+1)%32?"":"\n");

extern "C"
const char* xsignit(unsigned char*plaintext){
  static char buf[10240];
  ed25519_create_keypair(public_key, private_key, seed);
  ed25519_sign(signature, plaintext, strlen((char*)plaintext),
	       public_key, private_key);
  printf("XSIGNIT1\n");
  char*b = buf;
  b += sprintf(b, "seed: ");
  XDUMP(seed, 32);
  printf("XSIGNIT2[%s]\n", buf);
  b += sprintf(b, "prv1: ");
  XDUMP(private_key, 32);
  b += sprintf(b, "prv2: ");
  XDUMP(private_key+32, 32);
  printf("XSIGNIT3[%s]\n", buf);
  b += sprintf(b, "pubk: ");
  XDUMP(public_key, 32);
  printf("XSIGNIT4[%s]\n", buf);
  b += sprintf(b, "sig1: ");
  XDUMP(signature, 32);
  b += sprintf(b, "sig2: ");
  XDUMP(signature+32, 32);
  printf("XSIGNIT5[%s]\n", buf);
  return buf;
}

extern "C"
int xverify(char*securetext){
  static char buf[1024];
  sprintf(buf, "hello, %s!", securetext);
  //ed25519_create_keypair(public_key,
  //private_key,
  //seed);
  printf("hello to\n");
  return 1;
}

EMSCRIPTEN_BINDINGS(my_module) {
    function("lerp", &lerp);
    function("lerp2", &lerp2);
    function("forkOff", &forkOff);
}

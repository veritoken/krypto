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
  char*b = buf;
  /*
  b += sprintf(b, "seed: ");
  XDUMP(seed, 32);
  b += sprintf(b, "prv1: ");
  b += sprintf(b, "prv2: ");
  XDUMP(private_key+32, 32);
  */
  b += sprintf(b, "pubk: ");
  XDUMP(public_key, 32);
  b += sprintf(b, "sig1: ");
  XDUMP(signature, 32);
  b += sprintf(b, "sig2: ");
  XDUMP(signature+32, 32);
  //printf("XSIGNIT5[%s]\n", buf);
  return buf;
}

void extract_lines(unsigned char *buf, int bytes, int lines, int M[]){
  M[0] = 0; M[lines] = bytes;
  for(int m=0,n=0; n<bytes && m<lines-1; n++)
    if(buf[n] == '\n')  M[++m] = n+1;}

void scan_line(unsigned char*buf, int buf_len, unsigned char*out) {
  char hexbuf[3] = "00";
  int hexnum = 0;
  for(int n=0; n<buf_len; n+=2){
    memcpy(hexbuf, buf+n, 2);
    sscanf(hexbuf, "%x", &hexnum);
    out[n/2] = hexnum;}}

extern "C"
int xverify(unsigned char*securetext){
  //printf("--xverify[%s]\n", securetext);

  int M[15] = {0};

  unsigned char*buf = securetext;
  int bytes = strlen((char*)buf);
  
  extract_lines(buf, bytes, 4, M);

  //printf("x--xverify[%s]\n", securetext);

  scan_line(buf+M[0]+6, M[1]-M[0]-7, public_key);
  scan_line(buf+M[1]+6, M[2]-M[1]-7, signature+ 0);
  scan_line(buf+M[2]+6, M[3]-M[2]-7, signature+32);

  buf  += M[3];
  bytes = M[4] - M[3];

  if (!ed25519_verify(signature, buf, bytes, public_key)) {
    //printf("invalid signature\n");
    return 0;
  }
  
  //printf("totally valid signature\n");
  return 1;
}

EMSCRIPTEN_BINDINGS(my_module) {
    function("lerp", &lerp);
    function("lerp2", &lerp2);
    function("forkOff", &forkOff);
}

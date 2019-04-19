#include <emscripten/bind.h>
#include<emscripten.h>
extern "C" {
#include "ed25519/ed25519.h"
}
#include<stdio.h>

using namespace emscripten;

unsigned char public_key[32], private_key[64], seed[32]="";
unsigned char signature[64];

unsigned char public_hex_key[64], private_hex_key[128], hex_seed[64]="";
unsigned char hex_signature[128];

#define DUMP(s,m)				\
    for(int n=0;n<m;n++)			\
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
  char*b = buf;
  b += sprintf(b, "pubk: ");
  XDUMP(public_key, 32);
  b += sprintf(b, "sig1: ");
  XDUMP(signature, 32);
  b += sprintf(b, "sig2: ");
  XDUMP(signature+32, 32);
  return buf;
}

extern "C"
const char* xsignitlong(unsigned char*plaintext){
  static char buf[10240];
  ed25519_create_keypair(public_key, private_key, seed);
  ed25519_sign(signature, plaintext, strlen((char*)plaintext),
	       public_key, private_key);
  char*b = buf;
  b += sprintf(b, "seed: ");
  XDUMP(seed, 32);
  b += sprintf(b, "prv1: ");
  XDUMP(private_key, 32);
  b += sprintf(b, "prv2: ");
  XDUMP(private_key+32, 32);
  b += sprintf(b, "pubk: ");
  XDUMP(public_key, 32);
  b += sprintf(b, "sig1: ");
  XDUMP(signature, 32);
  b += sprintf(b, "sig2: ");
  XDUMP(signature+32, 32);
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
unsigned char*getseed(){
  char*b = (char*)hex_seed;
  b += sprintf(b, "seed: ");
  XDUMP(seed, 32);
  return hex_seed;
}

extern "C"
unsigned char*getprivate(){
  char*b = (char*)private_hex_key;
  b += sprintf(b, "prv1: ");
  XDUMP(private_key, 32);
  b += sprintf(b, "prv2: ");
  XDUMP(private_key+32, 32);
  return private_hex_key;
}

extern "C"
void createseed(){
  ed25519_create_seed(seed);
  printf(" * seed\n");
  DUMP(seed, 32);

  ed25519_create_keypair(public_key, private_key, seed);
  printf(" * private key\n");
  DUMP(private_key, 64);
  printf(" * public key\n");
  DUMP(public_key, 32);
}

extern "C"
unsigned char*getpublic(){
  char*b = (char*)public_hex_key;
  b += sprintf(b, "pubk: ");
  XDUMP(public_key, 32);
  return public_hex_key;
}

extern "C"
int xsetseed(unsigned char*newseed){
  //printf("xsetseed(%s)\n", newseed);
  scan_line(newseed, 64, seed);
  ed25519_create_keypair(public_key, private_key, seed);
  return 1;
}

extern "C"
int xsetprivate(unsigned char*privatekey){
  //printf("xsetprivate(%s)\n", privatekey);
  scan_line(privatekey, 128, private_key);
  return 1;
}

extern "C"
int xverify(unsigned char*securetext){
  int M[15] = {0};
  unsigned char*buf = securetext;
  int bytes = strlen((char*)buf);
  extract_lines(buf, bytes, 4, M);
  scan_line(buf+M[0]+6, M[1]-M[0]-7, public_key);
  scan_line(buf+M[1]+6, M[2]-M[1]-7, signature+ 0);
  scan_line(buf+M[2]+6, M[3]-M[2]-7, signature+32);
  buf  += M[3]; bytes = M[4] - M[3];
  return(ed25519_verify(signature, buf, bytes, public_key));
}

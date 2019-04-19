#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "ed25519/ed25519.h"

#define writes(fd,s) write(fd,(s),strlen(s))

#define  LOG(fmt,...) ((fprintf(stdout, "%s:%d:LOG: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__),fflush(stdout)))
#define WARN(fmt,...) ((fprintf(stderr, "%s:%d:ERR: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)))
#define  WRN(fmt,...) ((fprintf(stderr, "%s:%d:ERR: " fmt   "", __FILE__, __LINE__, ##__VA_ARGS__)))
#define  DIE(fmt,...) ((fprintf(stderr, "%s:%d:DIE: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__),exit(1)))
#define PERR(fmt,...) ((fprintf(stderr, "%s:%d:ERR: " fmt   "", __FILE__, __LINE__, ##__VA_ARGS__),perror(""),exit(1)))

#define DUMP0(s,m)				\
    for(int n=0;n<m;n++)			\
      printf("%02x", (s)[n]),			\
	printf((n+1)%32?" ":"\n");

#define DUMP(s,m)				\
    for(int n=0;n<m;n++)			\
      printf("%02x", (s)[n]),			\
	printf((n+1)%32?"":"\n");

#define default break;default
#define case    break;case

unsigned char* read_seed(char* filename, unsigned char*buf) {
  FILE*f = fopen(filename, "rb");
  if(!f) PERR("%s: ", filename);
  if(!fread(buf, 32, 1, f)) DIE("seed file too short");
  //if(fread(&buf,  1, 1, f)) DIE("seed file too long");
  return fclose(f), buf;}

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

int main(int argc, char*argv[]) {
  unsigned char public_key[32], private_key[64];
  unsigned char  signature[64], seed[32]="", action=0;
  int M[15] = {0};

  while(*++argv) /* process command line args */
    if     (!strcmp(*argv,"-u")) setbuf(stdout, 0);
    else if(!strcmp(*argv,"-s")) action = 's';
    else if(!strcmp(*argv,"-S")) action = 'S';
    else if(!strcmp(*argv,"-v")) action = 'v';
    else if(!strcmp(*argv,"-V")) action = 'V';
    else if(!strcmp(*argv,"--seed"))
      if(*++argv) read_seed(*argv, seed);
      else DIE("--seed needs filename");
    else DIE("bad opt: %s", *argv);
  if(isatty(0)) DIE("cannot read from tty");
  if(action==0) DIE("no action: try -s, -v, -S, or -V");
  if(action=='V' && seed[0]) DIE("cannot specify --seed when using -V");

  /* make a nice big buffer, 1M */
  int bytes = 1024*1024;
  unsigned char*buf=malloc(bytes);
  bytes = fread(buf, 1, bytes, stdin);
  if(bytes < 0)
    PERR("<stdin>: ");
  if(bytes < 1)
    DIE("file reading error.  (is it blank?)");

  switch(action){
  case 'S':
    ed25519_create_keypair(public_key, private_key, seed);

    printf("pubk: ");
    DUMP(public_key, 32);

    goto create_signature;
    
  case 's': 
    ed25519_create_keypair(public_key, private_key, seed);

    goto create_signature;

  case 'V':   
    extract_lines(buf, bytes, 4, M);

    scan_line(buf+M[0]+6, M[1]-M[0]-7, public_key);
    scan_line(buf+M[1]+6, M[2]-M[1]-7, signature+ 0);
    scan_line(buf+M[2]+6, M[3]-M[2]-7, signature+32);

    buf  += M[3];
    bytes = M[4] - M[3];

    goto verify_signature;

  case 'v':
    ed25519_create_keypair(public_key, private_key, seed);

    extract_lines(buf, bytes, 3, M);

    scan_line(buf+M[0]+6, M[1]-M[0]-7, signature+ 0);
    scan_line(buf+M[1]+6, M[2]-M[1]-7, signature+32);

    buf  += M[2];
    bytes = M[3] - M[2];

    goto verify_signature;

  create_signature:
    /* create signature on the message with the keypair */
    ed25519_sign(signature, buf, bytes, public_key, private_key);

    printf("sig1: ");
    DUMP(signature, 32);
    printf("sig2: ");
    DUMP(signature+32, 32);

    goto verify_signature;
    
  verify_signature:
    /* verify the signature */
    if (!ed25519_verify(signature, buf, bytes, public_key)) {
      printf("invalid signature\n");
      exit(1);
    }
    
  default:
    DIE("no action: try -s or -v");}               
}

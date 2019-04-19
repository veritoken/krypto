#include "rmd160.h"
#include <stdlib.h>
#include <stdio.h>

static const char _Zero[20] = "";
static const char FSTR[] = "nce: 0x%x";

int prefix_match(char*msg, char*s, int diff8, int mask,
                 unsigned n, char r160[]){
  ripemd160(msg, s - msg + sprintf(s, FSTR, n), r160);
  return memcmp(_Zero, r160, diff8) || r160[diff8] & mask;}

int main(int argc, char* argv[]){

  int blockno = 0, diff = 8;
  while(*++argv)
    if     (!strcmp(*argv, "-b")) blockno = atoi(*++argv);
    else if(!strcmp(*argv, "-d")) diff    = atoi(*++argv);
    else printf("BAD OPTION[%s]\n", *argv), exit(1);

  int bytes, s_len = 1024*1024;
  char* msg = malloc(s_len), *s = msg;
  while ((bytes = fread(s, 1, s_len, stdin))){
    s += bytes;
    s_len -= bytes;}

  const int mask = (0xff00 >> (diff%8)) & 0xff;
  char r160[20] = "";
  unsigned n = -1, diff8 = diff/8;
  while(prefix_match(msg, s, diff8, mask, ++n, r160)){}

  char h160[42] = "";
  hexdigest(r160, 20, h160);
  fprintf(stderr, "%0*x/%s/%x\n", -1+diff/4, blockno, h160+diff/4, n);
  printf(FSTR, n);}

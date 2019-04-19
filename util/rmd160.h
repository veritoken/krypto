//#include "ripemd160.h"
#include <string.h>
#include <stdio.h>
void ripemd160 (const void *in, unsigned long in_len, void *out);
static inline
int hexdigest(char*in, int in_len, char*out){
  for (int n = 0; n < in_len; n++)
    sprintf(2*n+out, "%02x", (unsigned char)in[n]);
  return 2*in_len;
}
static inline
int hexdigestn(char*in, int in_len, char*out){
  int ret = hexdigest(in, in_len, out);
  out[2*in_len] = '\n';
  out[2*in_len+1] = 0;
  return ret;
}
static inline
int ripemd160_hexdigest(char*in, char*out){
  return hexdigest(in, 20, out);
}
static inline
int ripemd160_hexdigestn(char*in, char*out){
  return hexdigestn(in, 20, out);
}
static inline
int fromDate(const char datestr[], int*YYYY, int*MM, int*DD,
	      int*hh, int*mm, int*ss, int*uu){
  double uud = 0;
  return sscanf(datestr, "%04d-%02d-%02dT%02d:%02d:%02d%lfZ",
		YYYY, MM, DD, hh, mm, ss, &uud) - 7
    ? : ( (*uu = uud*1000000), 0 );
}
static inline
int toDate(char datestr[], int YYYY, int MM, int DD,
	   int hh, int mm, int ss, int uu){
  return sprintf(datestr, "%04d-%02d-%02dT%02d:%02d:%02d.%06dZ",
		 YYYY, MM, DD, hh, mm, ss, uu);
}

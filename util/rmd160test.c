//
#include "rmd160.h"
#include <string.h>
#include <stdio.h>
char		buffer    [1024] = "Hello, world!\n";

char		r1        [128] = "";
char		r2        [128] = "";
char		r3        [128] = "";

int
main()
{
	printf("tests!\n");
	printf("%ld\n", strlen(buffer));
	printf("%ld\n", sizeof(buffer));
	ripemd160(buffer, 0, r1);
	ripemd160(buffer, strlen(buffer), r2);
	ripemd160(buffer, strlen(buffer) + 1, r3);
	for (int n = 0; n < 20; n++) {
		printf("%02x", (unsigned char)r1[n]);
	}
	printf("\n");
	for (int n = 0; n < 20; n++) {
		printf("%02x", (unsigned char)r2[n]);
	}
	printf("\n");
	for (int n = 0; n < 20; n++) {
		printf("%02x", (unsigned char)r3[n]);
	}
	printf("\n");

        char b1[1024];
        char b2[1024];
        char b3[1024];

        ripemd160_hexdigest(r1, b1);
        ripemd160_hexdigest(r2, b2);
        ripemd160_hexdigest(r3, b3);
	printf("%s\n", b1);
	printf("%s\n", b2);
	printf("%s\n", b3);
}

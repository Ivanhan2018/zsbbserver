#include "base64.h"

static const char e64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char d64[]="|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";


void base64_encode(char * p, int l, char * q)
{
	int i, o = 0;

	for (i = 0; i < l; i += 3) 
	{
		*(q + o) = e64[*(p + i) >> 2];
		*(q + o + 1) = e64[((*(p + i) & 0x03) << 4) | ((*(p + i + 1) & 0xf0) >> 4)];
		*(q + o + 2) = i + 1 < l ? e64[((*(p + i + 1) & 0x0f) << 2) | ((*(p + i + 2) & 0xc0) >> 6)] : '=';
		*(q + o + 3) = i + 2 < l ? e64[(*(p + i + 2) & 0x3f)] : '=';
		o += 4;
	}

	i = l + l / 3 + (int)(l % 3 != 0);
    if (i % 4) i += 4 - i % 4;
	*(q + i) = '\0';

}

void base64_decode(char * p, int l, char * q)
{
	int o, i;
	unsigned char c;

	for (i = 0; i < l; i++) 
	{
		c = *(p + i);
		c = (char)((c < 43 || c > 122) ? 0 : d64[ c - 43 ]);
		if (c) c = (unsigned char)((c == '$') ? 0 : c - 61);
		*(q + i) = (c ? c - 1 : 0);
	}

	o = 0;
	for (i = 0; i < l; i += 4) 
	{
		*(q + o) = (char)(*(q + i) << 2 | *(q + i + 1) >> 4);
		*(q + o + 1) = (char)(*(q + i + 1) << 4 | *(q + i + 2) >> 2);
		*(q + o + 2) = (char)(((*(q + i + 2) << 6) & 0xc0) | *(q + i + 3));
		o += 3;
	}

	for (i = o; i < l; i++) 
		*(q + i) = (char)0;
}


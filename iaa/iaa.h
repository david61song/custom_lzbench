#ifndef _IAA_H
#define _IAA_H

#include <qpl.h>

#ifdef IAA_PATH_HARDWARE
#define EXECUTION_PATH qpl_path_hardware;
#else
#define EXECUTION_PATH qpl_path_software;
#endif


char* IAA_deflate_init(size_t insize);
char* IAA_deflate_canned_init(size_t insize);
void IAA_deflate_deinit(char* workmem);
void IAA_deflate_canned_deinit(char* workmem);


int64_t IAA_deflate_fixed_compress(char *inbuf, size_t insize, char *outbuf, size_t outsize, char* workmem);
int64_t IAA_deflate_fixed_decompress(char *inbuf, size_t insize, char *outbuf, size_t outsize, char* workmem);

int64_t IAA_deflate_dynamic_compress(char *inbuf, size_t insize, char *outbuf, size_t outsize, char* workmem);
int64_t IAA_deflate_dynamic_decompress(char *inbuf, size_t insize, char *outbuf, size_t outsize, char* workmem);

int64_t IAA_deflate_canned_compress(char *inbuf, size_t insize, char *outbuf, size_t outsize, char* workmem);
int64_t IAA_deflate_canned_decompress(char *inbuf, size_t insize, char *outbuf, size_t outsize, char* workmem);

#endif 
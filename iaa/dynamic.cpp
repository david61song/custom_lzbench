#include "./iaa.h"
#include <iostream>

int64_t IAA_deflate_dynamic_compress(char *inbuf, size_t insize, char *outbuf, size_t outsize, char* workmem)
{
    qpl_job* job = (qpl_job*)workmem;
    qpl_status status;

    // Set up the job for compression
    job->op            = qpl_op_compress;
    job->level         = qpl_level_1;
    job->next_in_ptr   = (uint8_t*)inbuf;
    job->available_in  = (uint32_t)insize;
    job->next_out_ptr  = (uint8_t*)outbuf;
    job->available_out = (uint32_t)outsize;
    job->flags         = QPL_FLAG_FIRST | QPL_FLAG_LAST | QPL_FLAG_OMIT_VERIFY | QPL_FLAG_DYNAMIC_HUFFMAN;

    // Execute the compression job
    status = qpl_execute_job(job);
    if (status != QPL_STS_OK) {
        fprintf(stderr, "Error during QPL compression: %d\n", status);
        return 0;
    }

    // Return the size of the compressed data
    return (int64_t)job->total_out;
}

int64_t IAA_deflate_dynamic_decompress(char *inbuf, size_t insize, char *outbuf, size_t outsize, char* workmem)
{
    qpl_job* job = (qpl_job*)workmem;
    qpl_status status;

    // Set up the job for decompression
    job->op            = qpl_op_decompress;
    job->level         = qpl_default_level;
    job->next_in_ptr   = (uint8_t*)inbuf;
    job->available_in  = (uint32_t)insize;
    job->next_out_ptr  = (uint8_t*)outbuf;
    job->available_out = (uint32_t)outsize;
    job->flags         = QPL_FLAG_FIRST | QPL_FLAG_LAST | QPL_FLAG_DYNAMIC_HUFFMAN | QPL_FLAG_OMIT_VERIFY;

    // Execute the decompression job
    status = qpl_execute_job(job);
    if (status != QPL_STS_OK) {
        fprintf(stderr, "Error during QPL decompression: %d\n", status);
        exit(-1);
        return 0;
    }

    // Return the size of the decompressed data
    return (int64_t)job->total_out;
}
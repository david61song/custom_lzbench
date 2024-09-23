#include "./iaa.h"
#include <iostream>

int64_t IAA_deflate_canned_compress(char *inbuf, size_t insize, char *outbuf, size_t outsize, char* workmem)
{
    qpl_job* job = (qpl_job*)workmem;
    qpl_status status;
    qpl_histogram deflate_histogram {};
    qpl_path_t execution_path = EXECUTION_PATH;
    qpl_huffman_table_t huffman_table = nullptr;

    status = qpl_deflate_huffman_table_create(combined_table_type, execution_path, DEFAULT_ALLOCATOR_C, &huffman_table);
    if (status != QPL_STS_OK) {
        std::cout << "An error " << status << " acquired during Huffman table creation.\n";
        return 0;
    }

    // Filling deflate histogram first
    status = qpl_gather_deflate_statistics((uint8_t*)inbuf, (uint32_t) insize, &deflate_histogram, qpl_default_level,
                                           execution_path);
    if (status != QPL_STS_OK) {
        std::cout << "An error " << status << " acquired during gathering statistics for Huffman table.\n";
        qpl_huffman_table_destroy(job->huffman_table);
        return 0;
    }

    // Building the Huffman table
    status = qpl_huffman_table_init_with_histogram(huffman_table, &deflate_histogram);
    if (status != QPL_STS_OK) {
        std::cout << "An error " << status << " acquired during Huffman table initialization.\n";
        qpl_huffman_table_destroy(job->huffman_table);
        return 0;
    }

    job->op            = qpl_op_compress;
    job->level         = qpl_default_level;
    job->next_in_ptr   = (uint8_t*)inbuf;
    job->available_in  = (uint32_t)insize;
    job->next_out_ptr  = (uint8_t*)outbuf;
    job->available_out = (uint32_t)outsize;
    job->flags         = QPL_FLAG_FIRST | QPL_FLAG_LAST | QPL_FLAG_CANNED_MODE | QPL_FLAG_OMIT_VERIFY;
    job->huffman_table = huffman_table;


    // Compression
    status = qpl_execute_job(job);
    if (status != QPL_STS_OK) {
        std::cout << "An error " << status << " acquired during compression.\n";
        qpl_huffman_table_destroy(job->huffman_table);
        exit(-1);
        return 0;
    }

    // Return the size of the compressed data
    return (int64_t)job->total_out;
}

int64_t IAA_deflate_canned_decompress(char *inbuf, size_t insize, char *outbuf, size_t outsize, char* workmem)
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
    job->flags         = QPL_FLAG_FIRST | QPL_FLAG_LAST | QPL_FLAG_CANNED_MODE | QPL_FLAG_OMIT_VERIFY;

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
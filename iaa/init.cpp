#include "./iaa.h"
#include <iostream>

char* IAA_deflate_init(size_t insize)
{
    qpl_status status;
    qpl_path_t execution_path = EXECUTION_PATH; // Use hardware path for IAA
    uint32_t job_size = 0;
    qpl_job* job = NULL;

    // Get the size of the job structure
    status = qpl_get_job_size(execution_path, &job_size);
    if (status != QPL_STS_OK) {
        fprintf(stderr, "Error getting QPL job size: %d\n", status);
        return NULL;
    }

    // Allocate memory for the job structure
    job = (qpl_job*)malloc(job_size);
    if (!job) {
        fprintf(stderr, "Failed to allocate memory for QPL job\n");
        return NULL;
    }

    // Initialize the job structure
    status = qpl_init_job(execution_path, job);
    if (status != QPL_STS_OK) {
        fprintf(stderr, "Error initializing QPL job: %d\n", status);
        free(job);
        return NULL;
    }

    // Return the job structure as the work memory
    return (char*)job;
}

char* IAA_deflate_canned_init(size_t insize)
{
    qpl_status status;
    qpl_path_t execution_path = EXECUTION_PATH; // Use hardware path for IAA
    uint32_t job_size = 0;
    qpl_job* job = NULL;
    // Huffman table initialization

    // Get the size of the job structure
    status = qpl_get_job_size(execution_path, &job_size);
    if (status != QPL_STS_OK) {
        fprintf(stderr, "Error getting QPL job size: %d\n", status);
        return NULL;
    }

    // Allocate memory for the job structure
    job = (qpl_job*)malloc(job_size);
    if (!job) {
        fprintf(stderr, "Failed to allocate memory for QPL job\n");
        return NULL;
    }

    // Initialize the job structure
    status = qpl_init_job(execution_path, job);
    if (status != QPL_STS_OK) {
        fprintf(stderr, "Error initializing QPL job: %d\n", status);
        free(job);
        return NULL;
    }

    // Return the job structure as the work memory
    return (char*)job;
}

void IAA_deflate_canned_deinit(char* workmem) {
    qpl_job* job = (qpl_job*)workmem;
    qpl_status status; 

    // Freeing resources
    status = qpl_huffman_table_destroy(job->huffman_table);
    if (status != QPL_STS_OK) {
        std::cout << "An error " << status << " acquired during destroying Huffman table.\n";
        return ;
    }
    
    if (job) {
        qpl_fini_job(job);
        free(job);
    }
}


void IAA_deflate_deinit(char* workmem) {
    qpl_job* job = (qpl_job*)workmem;
    if (job) {
        qpl_fini_job(job);
        free(job);
    }
}


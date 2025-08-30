#define _GNU_SOURCE
#define SCALLOC_IMPLEMENTATION
#include "scalloc.h"
#include <sys/mman.h>
#include <stddef.h>
#include <stdio.h>

struct Block {
    int free_size;
    void* pointer;
};
struct BlockInfo {
    int size;
    void* pointer;
};

struct Block block_struct = {0, 0};
struct BlockInfo block_info = {0, 0};

void* scalloc(int size) {
    if(block_struct.free_size == 0) {
        int block = 0;
        while(block < size) {
            block = block + 4096;
        }
        void* allocate = mmap(
            NULL,
            block,
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS,
            -1,
            0
        );
        if (allocate == MAP_FAILED) {
            perror("MAP FAILED");
            return NULL;
        }
        void* return_ptr = allocate;
        block_struct.free_size = block - size;
        block_struct.pointer = (char*)allocate + size;
        block_info.size = block;
        block_info.pointer = allocate;
        return return_ptr;
    } else {
        if(block_struct.free_size > size) {
            void* ptr = block_struct.pointer;
            block_struct.pointer = (char*)block_struct.pointer + size;
            block_struct.free_size = block_struct.free_size - size;
            return ptr;
        } else if(block_struct.free_size <= size) {
            int block = 0;
            while(block < size - block_struct.free_size) {
                block = block + 4096;
            }
            int total_size = block + block_info.size;
            void* allocate = mremap(
                block_info.pointer,
                block_info.size,
                total_size,
                MREMAP_MAYMOVE
            );
            if (allocate == MAP_FAILED) {
                perror("MAP FAILED");
                return NULL;
            }
            block_struct.pointer = (char*)allocate + (block_info.size - block_struct.free_size);
            block_info.pointer = allocate;
            block_info.size = total_size;
            block_struct.free_size += block;
            void* return_pointer = block_struct.pointer;
            block_struct.pointer = (char*)block_struct.pointer + size;
            block_struct.free_size = block_struct.free_size - size;

            return return_pointer;
        }
    }

}
int scallocfree() {
    munmap(block_info.pointer, block_info.size);
    return 0;
}

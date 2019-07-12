
#ifndef MEXT2_H
#define MEXT2_H

#include <stdint.h>

#define MEXT2_SUPERBLOCK_OFFSET 1024

uint32_t mext2_get_inode_count( uint8_t dev_idx, uint8_t part_idx );
uint32_t mext2_get_major_version( uint8_t dev_idx, uint8_t part_idx );
uint16_t mext2_get_signature( uint8_t dev_idx, uint8_t part_idx );
uint32_t mext2_get_block_size( uint8_t dev_idx, uint8_t part_idx );

#endif /* MEXT2_H */


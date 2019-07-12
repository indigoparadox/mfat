
#ifndef MEXT2_H
#define MEXT2_H

#include "mfatdef.h"

#define MEXT2_SUPERBLOCK_OFFSET 1024
#define MEXT2_SUPERBLOCK_SZ     1024
#define MEXT2_BGDT_OFFSET (MEXT2_SUPERBLOCK_OFFSET + MEXT2_SUPERBLOCK_SZ)
#define MEXT2_BGD_SZ 32

#define mext2_get_bgd_offset( idx ) \
   (MEXT2_BGDT_OFFSET + (idx * MEXT2_BGD_SZ))

uint32_t mext2_get_inode_count( uint8_t dev_idx, uint8_t part_idx );
uint32_t mext2_get_inodes_per_group( uint8_t dev_idx, uint8_t part_idx );
uint32_t mext2_get_block_count( uint8_t dev_idx, uint8_t part_idx );
uint32_t mext2_get_blocks_per_group( uint8_t dev_idx, uint8_t part_idx );
uint32_t mext2_get_group_count( uint8_t dev_idx, uint8_t part_idx );
uint32_t mext2_get_group_count( uint8_t dev_idx, uint8_t part_idx );
uint32_t mext2_get_major_version( uint8_t dev_idx, uint8_t part_idx );
uint16_t mext2_get_signature( uint8_t dev_idx, uint8_t part_idx );
uint32_t mext2_get_block_size( uint8_t dev_idx, uint8_t part_idx );
uint16_t mext2_get_bgd_dir_count(
   FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx );

#endif /* MEXT2_H */


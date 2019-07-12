
#include "mext2.h"
#include "disk.h"
#include "mbytesin.h"

uint32_t mext2_get_inode_count( uint8_t dev_idx, uint8_t part_idx ) {
   uint32_t out = 0;
   mbytesin_read_big_endian_u32(
      dev_idx, part_idx, MEXT2_SUPERBLOCK_OFFSET, &out );
   return out;
}

uint32_t mext2_get_major_version( uint8_t dev_idx, uint8_t part_idx ) {
   uint32_t out = 0;
   mbytesin_read_big_endian_u32(
      dev_idx, part_idx, MEXT2_SUPERBLOCK_OFFSET + 76, &out );
   return out;
}

uint16_t mext2_get_signature( uint8_t dev_idx, uint8_t part_idx ) {
   uint16_t out = 0;
   mbytesin_read_big_endian_u16(
      dev_idx, part_idx, MEXT2_SUPERBLOCK_OFFSET + 56, &out );
   return out;
}

uint32_t mext2_get_block_size( uint8_t dev_idx, uint8_t part_idx ) {
   uint32_t out = 0;
   mbytesin_read_big_endian_u32(
      dev_idx, part_idx, MEXT2_SUPERBLOCK_OFFSET + 24, &out );
   return 1024 << out;
}


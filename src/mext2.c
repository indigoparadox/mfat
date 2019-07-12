
#include "mext2.h"
#include "disk.h"
#include "mbytesin.h"

#ifdef DEBUG
#include <assert.h>
#ifdef __ELOS__
#include "console.h"
#else /* !__ELOS__ */
#include <stdio.h>
#endif /* __ELOS__ */
#else /* !DEBUG */
#define assert( x )
#endif /* DEBUG */

#ifdef CHECK
#include <check.h>
#endif /* CHECK */

uint32_t mext2_get_inode_count( uint8_t dev_idx, uint8_t part_idx ) {
   uint32_t out = 0;
   mbytesin_read_big_endian_u32(
      dev_idx, part_idx, MEXT2_SUPERBLOCK_OFFSET, &out );
   return out;
}

uint32_t mext2_get_inodes_per_group( uint8_t dev_idx, uint8_t part_idx ) {
   uint32_t out = 0;
   mbytesin_read_big_endian_u32(
      dev_idx, part_idx, MEXT2_SUPERBLOCK_OFFSET + 40, &out );
   return out;
}

uint32_t mext2_get_block_count( uint8_t dev_idx, uint8_t part_idx ) {
   uint32_t out = 0;
   mbytesin_read_big_endian_u32(
      dev_idx, part_idx, MEXT2_SUPERBLOCK_OFFSET + 4, &out );
   return out;
}

uint32_t mext2_get_blocks_per_group( uint8_t dev_idx, uint8_t part_idx ) {
   uint32_t out = 0;
   mbytesin_read_big_endian_u32(
      dev_idx, part_idx, MEXT2_SUPERBLOCK_OFFSET + 32, &out );
   return out;
}

uint32_t mext2_get_group_count( uint8_t dev_idx, uint8_t part_idx ) {
   uint32_t out1 = 0;
#ifdef DEBUG
   uint32_t out2 = 0;
#endif /* DEBUG */
   out1 = mext2_get_block_count( dev_idx, part_idx ) /
      mext2_get_blocks_per_group( dev_idx, part_idx );

   /* Round up if needed. */
   if(
      0 < mext2_get_block_count( dev_idx, part_idx ) %
      mext2_get_blocks_per_group( dev_idx, part_idx )
   ) {
      out1++;
   }
   
#ifdef DEBUG
   out2 = mext2_get_inode_count( dev_idx, part_idx ) /
      mext2_get_inodes_per_group( dev_idx, part_idx );

   /* Round up if needed. */
   if(
      0 < mext2_get_inode_count( dev_idx, part_idx ) %
      mext2_get_inodes_per_group( dev_idx, part_idx )
   ) {
      out2++;
   }
   
#ifdef CHECK
   ck_assert_uint_eq( out1, out2 );
#else /* !CHECK */
   assert( out1 == out2 );
#endif /* CHECK */
#endif /* DEBUG */
   return out1;
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

uint16_t mext2_get_bgd_dir_count(
   FILEPTR_T bgd_offset, uint8_t dev_idx, uint8_t part_idx
) {
   uint16_t out = 0;
   mbytesin_read_big_endian_u16(
      dev_idx, part_idx, bgd_offset + 16, &out );
   return out;
}


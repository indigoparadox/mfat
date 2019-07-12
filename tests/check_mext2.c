
#include <check.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "../src/mext2.h"
#include "../src/disk.h"

static char g_img_name[] = "testimg.ext2.img";

START_TEST( test_mext2_superblock ) {
   uint32_t inodes = mext2_get_inode_count( 0, 0 );
   uint32_t version = mext2_get_major_version( 0, 0 );
   uint16_t signature = mext2_get_signature( 0, 0 );
   uint32_t block_size = mext2_get_block_size( 0, 0 );
   uint32_t block_count = mext2_get_block_count( 0, 0 );
   uint32_t bpg = mext2_get_blocks_per_group( 0, 0 );
   uint32_t group_count = mext2_get_group_count( 0, 0 );
   ck_assert_uint_eq( inodes, 48 );
   ck_assert_uint_eq( version, 0 );
   ck_assert_uint_eq( signature, 0xef53 );
   ck_assert_uint_eq( block_size, 1024 );
   ck_assert_uint_eq( block_count, 20480 );
   ck_assert_uint_eq( bpg, 6832 );
   ck_assert_uint_gt( group_count, 0 );
}
END_TEST

START_TEST( test_mext2_bgd ) {
   uint32_t bgd_offset = mext2_get_bgd_offset( _i );
   uint16_t dir_count = mext2_get_bgd_dir_count( bgd_offset, 0, 0 );
   ck_assert_uint_eq( dir_count, 1 );
}
END_TEST

static void setup_disk() {
   disk_open( 0, g_img_name );
}

static void teardown_disk() {
   disk_close( 0 );
}

Suite* mext2_suite( void ) {
   Suite* s;
   TCase* tc_metadata;

   s = suite_create( "mext2" );

   /* Core test case */
   tc_metadata = tcase_create( "Metadata" );

   tcase_add_checked_fixture( tc_metadata, setup_disk, teardown_disk );
   tcase_add_test( tc_metadata, test_mext2_superblock );
   tcase_add_loop_test( tc_metadata, test_mext2_bgd, 0, 2 );

   suite_add_tcase( s, tc_metadata );

   return s;
}


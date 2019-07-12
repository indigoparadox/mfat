
#include <check.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "../src/mfat.h"
#include "../src/disk.h"

#define IMG_NAME_LEN 12
static char g_img_name[] = "testimg.fat.img";

extern uint8_t* g_disk_map[MAX_FAKE_DISKS];

static char* g_data_filename = NULL;
static uint8_t g_data_filename_len = 0;
static void (*g_data_test)( uint32_t, uint32_t ) = NULL;
static int g_test_sz = 1;

void test_data_short( uint32_t c, uint32_t idx ) {
   const char* test_str = "The quick brown fox.";
   ck_assert_int_eq( c, test_str[idx] );
}

void test_data_long( uint32_t c, uint32_t idx ) {
   ck_assert_uint_eq( c, idx );
}

/* Convenient prototypes for normally static functions. */

uint16_t mfat_get_dir_entry_n_cluster_idx(
   uint32_t entry_offset, uint16_t iter_offset,
   uint8_t dev_idx, uint8_t part_idx );
uint16_t mfat_get_bytes_per_sector( uint8_t dev_idx, uint8_t part_idx );
uint8_t mfat_get_fat_count( uint8_t dev_idx, uint8_t part_idx );
uint8_t mfat_get_sectors_per_cluster(
   uint8_t dev_idx, uint8_t part_idx );
uint16_t mfat_get_sectors_per_fat( uint8_t dev_idx, uint8_t part_idx );
uint16_t mfat_get_root_dir_entries_count(
   uint8_t dev_idx, uint8_t part_idx );
uint32_t mfat_get_data_area_offset( uint8_t dev_idx, uint8_t part_idx );
uint16_t mfat_get_cluster_size( uint8_t dev_idx, uint8_t part_idx );
uint32_t mfat_get_cluster_data_offset(
   uint16_t fat_idx, uint8_t dev_idx, uint8_t part_idx );
uint32_t mfat_get_root_dir_offset( uint8_t dev_idx, uint8_t part_idx );

#define MFAT_OFFSET_FAT 512

START_TEST( test_mfat_bpb ) {
   uint32_t root_dir_start = 0;
   uint32_t root_dir_entries_count = 0;
   uint32_t data_offset = 0;
   //uint32_t cluster_count = 0;

   ck_assert_uint_eq( 512, mfat_get_bytes_per_sector( 0, 0 ) );
   ck_assert_uint_eq( 4  , mfat_get_sectors_per_cluster( 0, 0 ) );
   ck_assert_uint_eq( 2  , mfat_get_fat_count( 0, 0 ) );
   ck_assert_uint_eq( 20 , mfat_get_sectors_per_fat( 0, 0 ) );

   root_dir_entries_count = mfat_get_root_dir_entries_count( 0, 0 );
   ck_assert_uint_eq( 512, root_dir_entries_count );

   root_dir_start = mfat_get_root_dir_offset( 0, 0 );
   ck_assert_uint_eq( 512 * 2 * 20, root_dir_start );

   data_offset = root_dir_start + (root_dir_entries_count * 32);
   ck_assert_uint_eq( data_offset, mfat_get_data_area_offset( 0, 0 ) );

   //cluster_count = mfat_get_sectors_total( 0, 0 ) - data_offset;
   

   //ck_assert_uint_eq( cluster_count,
}
END_TEST

/*
START_TEST( test_mfat_entry ) {
   uint16_t entry = mfat_get_fat_entry( _i, 0, 0, 0 );

   ck_assert_uint_eq( entry, g_disk_map[0x200 + (2 * _i)] );
}
END_TEST
*/

#ifdef CHECK_FAT_CONSISTENCY

START_TEST( test_mfat_consistency ) {
   uint16_t last_entry;

   last_entry = mfat_get_fat_entry( _i - 1, 0, 0, 0 );

   ck_assert(
      0x0000 == last_entry ||
      0xffff == last_entry ||
      last_entry == _i );
}
END_TEST

#endif /* CHECK_FAT_CONSISTENCY */

START_TEST( test_mfat_fat_sz ) {
   //mfat_get_dir_entry_n_cluster_idx( 0, 0, 0, 0 );
}
END_TEST

START_TEST( test_mfat_cluster_data ) {
   uint16_t cluster_idx = 0;
   uint32_t file_on_disk_offset = 0;
   uint32_t file_size = 0;
   uint32_t cluster_end_on_disk = 0;
   uint32_t iter_offset = 0;
   uint32_t entry_offset = 0;
   uint32_t cluster_size = 0;
   uint32_t cluster_remainder = 0;
   uint32_t test_num = 0;
   uint8_t* bytes = 0;
   int i = 0;

   /* Grab the root directory and find the test file. */
   entry_offset = mfat_get_root_dir_offset( 0, 0 );
   entry_offset = mfat_get_dir_entry_first_offset( entry_offset, 0, 0 );
   entry_offset = mfat_get_dir_entry_offset(
      g_data_filename, g_data_filename_len, entry_offset, 0, 0 );
   file_size = mfat_get_dir_entry_size( entry_offset, 0, 0 );

   ck_assert_int_lt( (g_test_sz * _i), file_size );

   ck_assert_int_eq( 0, cluster_size % g_test_sz );

   cluster_size = mfat_get_cluster_size( 0, 0 );
   iter_offset = (g_test_sz * _i) % cluster_size;

   /* Grab the index of the cluster containing the requested chunk of the file
    * on the FAT. Then grab the offset in the data area.
    */
   cluster_idx = mfat_get_dir_entry_n_cluster_idx(
      entry_offset, iter_offset, 0, 0 );

   ck_assert_uint_ne( cluster_idx, 0 );

   /* Cluster index is valid, so translate it to a usable offset. */
   file_on_disk_offset = mfat_get_cluster_data_offset( cluster_idx, 0, 0 );
   cluster_remainder = file_on_disk_offset % cluster_size;

   ck_assert_uint_ne( file_on_disk_offset, 0 );
   ck_assert_uint_lt(
      mfat_get_cluster_size( 0, 0 ), cluster_size + file_on_disk_offset );
   ck_assert_uint_lt( 0, cluster_remainder );

   /* Set an end-of-file limit. */
   cluster_end_on_disk = file_on_disk_offset + cluster_size;

   ck_assert_uint_lt( file_on_disk_offset + iter_offset, cluster_end_on_disk );

   /* Grab the number as a multi-byte integer. */
   bytes = (uint8_t*)&test_num;
   for( i = 0 ; g_test_sz > i ; i++ ) {
      bytes[i] = disk_get_byte( 0, 0, file_on_disk_offset + iter_offset + i );
   }
   g_data_test( test_num, _i );
}
END_TEST

START_TEST( test_mfat_tables_identical ) {
   uint16_t entry_1 = 0;
   uint16_t entry_2 = 0;
   uint8_t j = 0;
   
   entry_1 = mfat_get_fat_entry( _i, 0, 0, 0 );
   for( j = 1 ; mfat_get_fat_count( 0, 0 ) > j ; j++ ) {
      entry_2 = mfat_get_fat_entry( _i, j, 0, 0 );
      ck_assert_uint_eq( entry_1, entry_2 );
   }
}
END_TEST

static void setup_disk() {
   disk_open( 0, g_img_name );
}

static void teardown_disk() {
   disk_close( 0 );
}

static void setup_data_short() {
   setup_disk();
   g_data_filename = "fox.txt";
   g_data_filename_len = 7;
   g_data_test = test_data_short;
}

static void teardown_data_short() {
   teardown_disk();
}

#ifdef CHECK_LONG_DATA

static void setup_data_long() {
   setup_disk();

   g_data_filename = "bcount.bin";
   g_data_filename_len = 10;
   g_data_test = test_data_long;
   g_test_sz = 4;
}

static void teardown_data_long() {
   teardown_disk();
}

#endif /* CHECK_LONG_DATA */

Suite* mfat_suite( void ) {
   Suite* s;
   TCase* tc_metadata;
   TCase* tc_data_short;
#ifdef CHECK_LONG_DATA
   TCase* tc_data_long;
#endif /* CHECK_LONG_DATA */

   s = suite_create( "mfat" );

   /* Core test case */
   tc_metadata = tcase_create( "Metadata" );

   //tcase_add_loop_test( tc_metadata, test_mfat_, 0, 4 );
   tcase_add_checked_fixture( tc_metadata, setup_disk, teardown_disk );
   tcase_add_test( tc_metadata, test_mfat_bpb );
   //tcase_add_loop_test( tc_metadata, test_mfat_entry, 2, 50 );
   tcase_add_test( tc_metadata, test_mfat_fat_sz );
#ifdef CHECK_FAT_CONSISTENCY
   tcase_add_loop_test( tc_metadata, test_mfat_consistency, 2, 50 );
#endif /* CHECK_FAT_CONSISTENCY */
   tcase_add_loop_test( tc_metadata, test_mfat_tables_identical, 2,
      2048 );

   /* File <= 1 cluster. */
   tc_data_short = tcase_create( "DataShort" );
   tcase_add_checked_fixture(
      tc_data_short, setup_data_short, teardown_data_short );
   tcase_add_loop_test( tc_data_short, test_mfat_cluster_data, 0, 20 );

#ifdef CHECK_LONG_DATA
   /* File > 1 cluster. */
   tc_data_long = tcase_create( "DataLong" );
   tcase_add_checked_fixture(
      tc_data_long, setup_data_long, teardown_data_long );
   tcase_add_loop_test( tc_data_long, test_mfat_cluster_data, 0, 1500 );
#endif /* CHECK_LONG_DATA */

   suite_add_tcase( s, tc_metadata );
   suite_add_tcase( s, tc_data_short );
#ifdef CHECK_LONG_DATA
   suite_add_tcase( s, tc_data_long );
#endif /* CHECK_LONG_DATA */

   return s;
}


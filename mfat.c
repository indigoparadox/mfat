
#include <stdio.h>
#include <assert.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>

struct mfat_bpb {
   uint8_t  boot_block[3];
   char     banner[8];
   uint16_t bytes_per_sector;
   uint8_t  sectors_per_cluster;
   uint16_t reserved_sectors;
   uint8_t  fat_count;
   uint16_t max_dir_ids;
   uint16_t sectors_per_fs;
   uint8_t  fat_id;
   uint16_t sectors_per_fat;
   uint16_t sectors_per_track;
   uint16_t heads;
   uint32_t hidden_sectors;
   uint32_t logical_sectors_ext;
} __attribute__( (packed) );

struct mfat_ebpb {
   struct mfat_bpb bpb;
   uint8_t  drive_num;
   uint8_t  unused37;
   uint8_t  e_boot_sig;
   uint32_t serial;
   char     label[11];
   char     system_id[8];
   uint8_t  unused62[448];
   uint16_t boot_sig;
} __attribute__( (packed) );

/*
struct mfat_ebpb {
   struct mfat_bpb bpb;
   uint32_t sectors_per_fat;
   uint16_t flags;
   uint16_t version;
   uint32_t root_cluster;
   uint16_t fsinfo_sector;
   uint16_t backup_boot_sector;
   uint32_t res_ints[3];
   uint8_t  drive_num;
   uint8_t  nt_flags;
   uint8_t  signature;
   uint32_t serial;
   char     label[11];
   char     system_id[8];
   uint8_t  boot_code[420];
   uint16_t boot_sig;
} __attribute__( (packed) );
*/

uint16_t read_int16_msb_first( uint16_t* num ) {
   uint16_t out = 0;
   uint8_t* bytes_ptr = (uint8_t*)num;
   int i = 0;
   
   for( i = 1 ; 0 <= i ; i-- ) {
      out |= bytes_ptr[i];
      out <<= 8;
   }

   return out;
}

int main() {
   int img_handle = 0;
   uint8_t* img_map = NULL;
   struct stat img_stat;
   struct mfat_ebpb* bpb = NULL;
   size_t img_size = 0;
   int i = 0;

   stat( "testimg.img", &img_stat );
   img_size = img_stat.st_size;

   img_handle = open( "testimg.img", O_RDONLY, 0 );
   assert( 0 < img_handle );

   img_map = mmap( NULL, img_size, PROT_READ,
      MAP_PRIVATE | MAP_POPULATE, img_handle, 0 );
   assert( MAP_FAILED != img_map );

   close( img_handle );

   bpb = (struct mfat_ebpb*)img_map;

   printf( "%s\nspc: %d\nres: %d\nspf: %d\n",
      bpb->bpb.banner, bpb->bpb.sectors_per_cluster,
      bpb->bpb.reserved_sectors, bpb->bpb.sectors_per_fat );

   assert( 0xeb == bpb->bpb.boot_block[0] );
   assert( 0x90 == bpb->bpb.boot_block[2] );
   assert( 512 == bpb->bpb.bytes_per_sector);
   assert( 4 == bpb->bpb.sectors_per_cluster );
   assert( 1 == bpb->bpb.reserved_sectors );
   assert( 2 == bpb->bpb.fat_count );
   assert( 40 == bpb->bpb.sectors_per_fat );

   printf( "%5ld   ", sizeof( struct mfat_ebpb ) );
   for(
      i = sizeof( struct mfat_ebpb );
      bpb->bpb.sectors_per_fat * bpb->bpb.bytes_per_sector > i;
      i += 2
   ) {
      printf( "%02x%02x ", img_map[i], img_map[i + 1] );
      if(
         0 == (i - sizeof( struct mfat_ebpb )) % 20 &&
         2 != (i - sizeof( struct mfat_ebpb ))
      ) {
         printf( "\n%5d   ", i + 2 );
      }
   } 

   printf( "\n" );

   munmap( img_map, img_size );
   
   return 0;
}


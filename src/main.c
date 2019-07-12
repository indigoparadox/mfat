
#include "disk.h"
#include "mfat.h"

#include <stdio.h>
#include <assert.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
   int img_handle = 0;
   uint8_t* img_map = NULL;
   struct stat img_stat;
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
   disk_set_ptr( 0, img_map );

   printf( "spc: %d\nspf: %d\n",
      mfat_get_sectors_per_cluster( 0, 0 ),
      mfat_get_sectors_per_fat( 0, 0 ) );

   assert( 512 == mfat_get_bytes_per_sector( 0, 0 ) );
   assert( 4 == mfat_get_sectors_per_cluster( 0, 0 ) );
   assert( 2 == mfat_get_fat_count( 0, 0 ) );
   assert( 40 == mfat_get_sectors_per_fat( 0, 0 ) );

   printf( "%5d   ", 0 );
   for(
      i = 0 ;
      i < mfat_get_entries_count( 0, 0 ) ;
      i++
   ) {
      printf( "%04x ", mfat_get_entry( i, 0, 0 ) );
      if( 0 == i % 10 && 0 != i ) {
         printf( "\n%5d   ", i );
      }
   } 

   printf( "\n" );

   munmap( img_map, img_size );
   
   return 0;
}


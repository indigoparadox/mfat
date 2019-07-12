
#include "disk.h"

#include <check.h>
#ifdef DISK_MMAP
#include <sys/stat.h>
#include <sys/mman.h>
#endif /* DISK_MMAP */
#include <unistd.h>
#include <fcntl.h>

#ifdef DISK_MMAP
uint8_t* g_disk_map[MAX_FAKE_DISKS] = { NULL };
size_t g_disk_sz[MAX_FAKE_DISKS] = { 0 };
#endif /* DISK_MMAP */
size_t g_disk_handles[MAX_FAKE_DISKS] = { 0 };

void disk_open( uint8_t dev_idx, char* img_name ) {
#ifdef DISK_MMAP
   struct stat img_stat;

   stat( img_name, &img_stat );
   g_disk_sz[dev_idx] = img_stat.st_size;
#endif /* DISK_MMAP */

   g_disk_handles[dev_idx] = open( img_name, O_RDONLY, 0 );
   ck_assert_int_lt( 0, g_disk_handles[dev_idx] );

#ifdef DISK_MMAP
   g_disk_map[dev_idx] = mmap( NULL, g_disk_sz[dev_idx], PROT_READ,
      MAP_PRIVATE | MAP_POPULATE, g_disk_handles[dev_idx], 0 );
   ck_assert_ptr_ne( MAP_FAILED, g_disk_map[dev_idx] );
#endif /* DISK_MMAP */
}

void disk_close( uint8_t dev_idx ) {
#ifdef DISK_MMAP
   munmap( g_disk_map[dev_idx], g_disk_sz[dev_idx] );
#endif /* DISK_MMAP */
   close( g_disk_handles[dev_idx] );
}

uint8_t disk_get_byte( uint8_t dev_idx, uint8_t part_idx, size_t offset ) {
#ifdef DISK_MMAP
   return g_disk_map[dev_idx][offset];
#else
   uint8_t byte = 0;
   lseek( g_disk_handles[dev_idx], offset, SEEK_SET );
   read( g_disk_handles[dev_idx], &byte, 1 );
   return byte;
#endif /* DISK_MMAP */
}

void disk_set_byte(
   uint8_t byte, uint8_t dev_idx, uint8_t part_idx, size_t offset
) {
#ifdef DISK_MMAP
   g_disk_map[dev_idx][offset] = byte;
   msync( g_disk_map[dev_idx], g_disk_sz[dev_idx], MS_SYNC );
#else
   lseek( g_disk_handles[dev_idx], offset, SEEK_SET );
   write( g_disk_handles[dev_idx], &byte, 1 );
#endif /* DISK_MMAP */
}


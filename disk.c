
#include "disk.h"

uint8_t* g_disk_map = NULL;

void disk_set_ptr( uint8_t dev_idx, uint8_t* disk_map ) {
   g_disk_map = disk_map;
}

uint8_t disk_get_byte( uint8_t dev_idx, uint8_t part_idx, size_t offset ) {
   return g_disk_map[offset];
}

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


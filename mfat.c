
#include "mfat.h"
#include "disk.h"

#include <assert.h>
#include <stdbool.h>

/*
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
*/

#define MFAT_OFFSET_FAT 512

uint8_t  mfat_get_fat_count(           uint8_t dev_idx, uint8_t part_idx ) {
   return disk_get_byte( dev_idx, part_idx, 16 );
}

uint16_t mfat_get_bytes_per_sector(    uint8_t dev_idx, uint8_t part_idx ) {
   uint16_t out = 0;
   out |= disk_get_byte( dev_idx, part_idx, 12 );
   out <<= 8;
   out |= disk_get_byte( dev_idx, part_idx, 11 );
   return out;
}

uint8_t  mfat_get_sectors_per_cluster( uint8_t dev_idx, uint8_t part_idx ) {
   return disk_get_byte( dev_idx, part_idx, 13 );
}

uint16_t mfat_get_sectors_per_fat(     uint8_t dev_idx, uint8_t part_idx ) {
   uint16_t out = 0;
   out |= disk_get_byte( dev_idx, part_idx, 23 );
   out <<= 8;
   out |= disk_get_byte( dev_idx, part_idx, 22 );
   return out;
}

uint16_t mfat_get_sectors_per_track(   uint8_t dev_idx, uint8_t part_idx ) {
   uint16_t out = 0;
   out |= disk_get_byte( dev_idx, part_idx, 25 );
   out <<= 8;
   out |= disk_get_byte( dev_idx, part_idx, 24 );
   return out;
}

uint32_t mfat_get_sectors_total(       uint8_t dev_idx, uint8_t part_idx ) {
   uint32_t out = 0;
   out |= disk_get_byte( dev_idx, part_idx, 35 );
   out <<= 8;
   out |= disk_get_byte( dev_idx, part_idx, 34 );
   out <<= 8;
   out |= disk_get_byte( dev_idx, part_idx, 33 );
   out <<= 8;
   out |= disk_get_byte( dev_idx, part_idx, 32 );
   return out;
}

uint16_t mfat_get_entries_count(       uint8_t dev_idx, uint8_t part_idx ) {
   return
      (mfat_get_sectors_per_fat( dev_idx, part_idx ) *
      mfat_get_bytes_per_sector( dev_idx, part_idx )) / 2;
}

uint16_t mfat_get_entry( uint16_t idx,  uint8_t dev_idx, uint8_t part_idx ) {
   uint16_t out = 0;
   uint16_t entry_offset = 0;

   /* Move past the BPB. */
   entry_offset += MFAT_OFFSET_FAT;
   entry_offset += (idx * 2);

   assert( idx <
      mfat_get_sectors_per_fat( dev_idx, part_idx ) *
      mfat_get_bytes_per_sector( dev_idx, part_idx ) );

   out |= disk_get_byte( dev_idx, part_idx, entry_offset + 1 );
   out <<= 8;
   out |= disk_get_byte( dev_idx, part_idx, entry_offset );
   return out;
}

uint16_t mfat_get_root_dir_offset( uint8_t dev_idx, uint8_t part_idx ) {
   uint16_t dir_offset = 0;

   dir_offset += MFAT_OFFSET_FAT;
   dir_offset += (mfat_get_bytes_per_sector( dev_idx, part_idx ) *
      mfat_get_sectors_per_fat( dev_idx, part_idx ) *
      mfat_get_fat_count( dev_idx, part_idx ));
   
   return dir_offset;
}

uint16_t mfat_get_dir_entry_offset(
   char name[11], uint16_t dir_offset, uint8_t dev_idx, uint8_t part_idx
) {
   uint16_t offset_out = dir_offset;
   int i = 0;
   bool found = false;
   
   while(
      !found &&
      0 != disk_get_byte( dev_idx, part_idx, offset_out )
   ) {
      for( i = 0 ; 8 > i ; i++ ) {
         /* TODO: cmp entry name with target. */
      }
   }

   return offset_out;
}

void mfat_get_dir_entry_name(
   char buffer[11], uint16_t offset, uint8_t dev_idx, uint8_t part_idx
) {
   int8_t i = 0;

   for( i = 0 ; 11 > i ; i++ ) {
      buffer[i] = disk_get_byte( dev_idx, part_idx, offset + i );
   }
}

uint8_t mfat_get_dir_entry_attrib(
   uint16_t offset, uint8_t dev_idx, uint8_t part_idx
) {
   return disk_get_byte( dev_idx, part_idx, offset + 11 );
}

uint8_t mfat_get_dir_entry_cluster(
   uint16_t cluster_idx, uint16_t entry_offset, uint8_t dev_idx, uint8_t part_idx
) {
   return 0;
}


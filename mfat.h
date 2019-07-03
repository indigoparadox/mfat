
#ifndef MFAT_H
#define MFAT_H

#include <stdint.h>

#define MFAT_ATTRIB_RO     0x01
#define MFAT_ATTRIB_HIDDEN 0x02
#define MFAT_ATTRIB_SYSTEM 0x04
#define MFAT_ATTRIB_VOL_ID 0x08
#define MFAT_ATTRIB_DIR    0x10
#define MFAT_ATTRIB_ARC    0x20
#define MFAT_ATTRIB_LFN    (MFAT_ATTRIB_RO | \
                           MFAT_ATTRIB_HIDDEN | \
                           MFAT_ATTRIB_SYSTEM | \
                           MFAT_ATTRIB_VOL_ID)

uint8_t  mfat_get_fat_count(           uint8_t dev_idx, uint8_t part_idx );
uint16_t mfat_get_bytes_per_sector(    uint8_t dev_idx, uint8_t part_idx );
uint8_t  mfat_get_sectors_per_cluster( uint8_t dev_idx, uint8_t part_idx );
uint16_t mfat_get_sectors_per_fat(     uint8_t dev_idx, uint8_t part_idx );
uint16_t mfat_get_sectors_per_track(   uint8_t dev_idx, uint8_t part_idx );
uint32_t mfat_get_sectors_total(       uint8_t dev_idx, uint8_t part_idx );
uint16_t mfat_get_entries_count(       uint8_t dev_idx, uint8_t part_idx );
uint16_t mfat_get_entry( uint16_t idx, uint8_t dev_idx, uint8_t part_idx );
uint16_t mfat_get_root_dir_offset(     uint8_t dev_idx, uint8_t part_idx );

uint16_t mfat_get_dir_entry_offset(
   char name[11], uint16_t parent_offset, uint8_t dev_idx, uint8_t part_idx
);
void mfat_get_dir_entry_name(
   char buffer[11], uint16_t offset, uint8_t dev_idx, uint8_t part_idx );
uint8_t mfat_get_dir_entry_attrib(
   uint16_t offset, uint8_t dev_idx, uint8_t part_idx );
uint8_t mfat_get_dir_entry_cluster(
   uint16_t cluster_idx, uint16_t entry_offset, uint8_t dev_idx, uint8_t part_idx );

#endif /* MFAT_H */


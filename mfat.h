
#ifndef MFAT_H
#define MFAT_H

#include <stdint.h>

typedef uint32_t FILEPTR_T;

#define MFAT_ATTRIB_RO     0x01
#define MFAT_ATTRIB_HIDDEN 0x02
#define MFAT_ATTRIB_SYSTEM 0x04
#define MFAT_ATTRIB_VOL_ID 0x08
#define MFAT_ATTRIB_DIR    0x10
#define MFAT_ATTRIB_ARC    0x20
#define MFAT_ATTRIB_LFN    0x0f

#define MFAT_FILENAME_LEN  13

uint16_t mfat_get_root_dir_entries_count( uint8_t dev_idx, uint8_t part_idx );
uint32_t mfat_get_sectors_total( uint8_t dev_idx, uint8_t part_idx );
uint16_t mfat_get_entries_count( uint8_t dev_idx, uint8_t part_idx );
uint16_t mfat_get_fat_entry(
   uint16_t cluster_idx, uint8_t fat_idx, uint8_t dev_idx, uint8_t part_idx );

FILEPTR_T mfat_get_dir_entry_first_offset(
   FILEPTR_T dir_offset, uint8_t dev_idx, uint8_t part_idx );
FILEPTR_T mfat_get_dir_entry_offset(
   const char search_name[MFAT_FILENAME_LEN], uint8_t search_name_len,
   FILEPTR_T parent_offset, uint8_t dev_idx, uint8_t part_idx );
FILEPTR_T mfat_get_dir_entry_next_offset(
   FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx );
FILEPTR_T mfat_get_root_dir_offset( uint8_t dev_idx, uint8_t part_idx );

uint8_t mfat_get_dir_entry_data(
   FILEPTR_T entry_offset, FILEPTR_T file_offset,
   uint8_t* buffer, uint16_t blen,
   uint8_t dev_idx, uint8_t part_idx );
void mfat_get_dir_entry_name(
   char buffer[MFAT_FILENAME_LEN],
   FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx );
uint8_t mfat_get_dir_entry_cyear(
   FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx );
uint32_t mfat_get_dir_entry_size(
   FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx );
uint8_t mfat_get_dir_entry_attrib(
   FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx );

#ifdef USE_DISK_RW

FILEPTR_T mfat_get_dir_entry_free_offset(
   FILEPTR_T dir_offset, uint32_t dir_sz, uint8_t dev_idx, uint8_t part_idx );
void mfat_set_dir_entry_name(
   const char name[MFAT_FILENAME_LEN],
   FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx );
void mfat_set_dir_entry_cyear(
   uint8_t cyear, FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx );
void mfat_set_dir_entry_size(
   uint32_t size, FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx );
void mfat_set_dir_entry_attrib(
   uint8_t attrib, FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx );

#endif /* USE_DISK_RW */

#endif /* MFAT_H */



#ifndef MFAT_H
#define MFAT_H

#include <stdint.h>

uint8_t  mfat_get_fat_count(           uint8_t dev_idx, uint8_t part_idx );
uint16_t mfat_get_bytes_per_sector(    uint8_t dev_idx, uint8_t part_idx );
uint8_t  mfat_get_sectors_per_cluster( uint8_t dev_idx, uint8_t part_idx );
uint16_t mfat_get_sectors_per_fat(     uint8_t dev_idx, uint8_t part_idx );
uint16_t mfat_get_sectors_per_track(   uint8_t dev_idx, uint8_t part_idx );
uint32_t mfat_get_sectors_total(       uint8_t dev_idx, uint8_t part_idx );
uint16_t mfat_get_entries_count(       uint8_t dev_idx, uint8_t part_idx );
uint16_t mfat_get_entry( uint16_t idx, uint8_t dev_idx, uint8_t part_idx );

#endif /* MFAT_H */


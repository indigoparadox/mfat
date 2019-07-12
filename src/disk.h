
#ifndef DISK_H
#define DISK_H

#include <stdint.h>
#include <stddef.h>

#define MAX_FAKE_DISKS 10

void disk_open( uint8_t dev_idx, char* img_name );
void disk_close( uint8_t dev_idx );
void disk_set_ptr( uint8_t dev_idx, uint8_t* disk_map );
uint8_t disk_get_byte( uint8_t dev_idx, uint8_t part_idx, size_t offset );
void disk_set_byte(
   uint8_t byte, uint8_t dev_idx, uint8_t part_idx, size_t offset );

#endif /* DISK_H */


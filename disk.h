
#ifndef DISK_H
#define DISK_H

#include <stdint.h>
#include <stddef.h>

void disk_set_ptr( uint8_t dev_idx, uint8_t* disk_map );
uint8_t disk_get_byte( uint8_t dev_idx, uint8_t part_idx, size_t offset );

#endif /* DISK_H */


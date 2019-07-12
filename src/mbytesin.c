
#include "mbytesin.h"
#include "disk.h"

#include <stdio.h>

void mbytesin_read_big_endian_u32(
   uint8_t dev_idx, uint8_t part_idx, size_t offset, uint32_t* out
) {
   *out |= disk_get_byte( dev_idx, part_idx, offset + 3 );
   *out <<= 8;
   *out |= disk_get_byte( dev_idx, part_idx, offset + 2 );
   *out <<= 8;
   *out |= disk_get_byte( dev_idx, part_idx, offset + 1 );
   *out <<= 8;
   *out |= disk_get_byte( dev_idx, part_idx, offset );
}

void mbytesin_read_big_endian_u16(
   uint8_t dev_idx, uint8_t part_idx, size_t offset, uint16_t* out
) {
   *out |= disk_get_byte( dev_idx, part_idx, offset + 1 );
   *out <<= 8;
   *out |= disk_get_byte( dev_idx, part_idx, offset );
}


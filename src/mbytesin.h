
#ifndef MBYTESIN_H
#define MBYTESIN_H

#include <stdint.h>
#include <stddef.h>

void mbytesin_read_big_endian_u32(
   uint8_t dev_idx, uint8_t part_idx, size_t offset, uint32_t* out );
void mbytesin_read_big_endian_u16(
   uint8_t dev_idx, uint8_t part_idx, size_t offset, uint16_t* out );

#endif /* MBYTESIN_H */


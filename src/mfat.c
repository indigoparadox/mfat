
#include "mfat.h"
#include "disk.h"
#include "mbytesin.h"

#ifdef ALPHA_PRESENT
#include "alpha.h"
#endif /* ALPHA_PRESENT */

#ifdef DEBUG
#include <assert.h>
#ifdef __ELOS__
#include "console.h"
#else
#include <stdio.h>
#endif /* __ELOS__ */
#else
#define assert( x )
#endif /* DEBUG */

#include <stdbool.h>

#define MFAT_DIR_ENTRY_SZ 32

#ifndef CHECK
static
#endif /* CHECK */
uint8_t mfat_get_fat_count( uint8_t dev_idx, uint8_t part_idx ) {
#ifdef DEBUG_TEST_IMG
   assert( 2 == disk_get_byte( dev_idx, part_idx, 16 ) );
#endif /* DEBUG_TEST_IMG */
   return disk_get_byte( dev_idx, part_idx, 16 );
}

#ifndef CHECK
static
#endif /* CHECK */
uint16_t mfat_get_reserved_sectors( uint8_t dev_idx, uint8_t part_idx ) {
   uint16_t out = 0;
   mbytesin_read_big_endian_u16( dev_idx, part_idx, 14, &out );
   return out;
}

uint16_t mfat_get_bytes_per_sector( uint8_t dev_idx, uint8_t part_idx ) {
   uint16_t out = 0;
   mbytesin_read_big_endian_u16( dev_idx, part_idx, 11, &out );
#ifdef DEBUG_TEST_IMG
   assert( 512 == out );
#endif /* DEBUG_TEST_IMG */
   return out;
}

#ifndef CHECK
static
#endif /* CHECK */
uint8_t mfat_get_sectors_per_cluster(
   uint8_t dev_idx, uint8_t part_idx
) {
#ifdef DEBUG_TEST_IMG
   assert( 4 == disk_get_byte( dev_idx, part_idx, 13 ) );
#endif /* DEBUG_TEST_IMG */
   return disk_get_byte( dev_idx, part_idx, 13 );
}

#ifndef CHECK
static
#endif /* CHECK */
uint16_t mfat_get_sectors_per_fat( uint8_t dev_idx, uint8_t part_idx ) {
   uint16_t out = 0;
   mbytesin_read_big_endian_u16( dev_idx, part_idx, 22, &out );
#ifdef DEBUG_TEST_IMG
   assert( 52 == out );
#endif /* DEBUG_TEST_IMG */
   return out;
}

#ifndef CHECK
static
#endif /* CHECK */
uint32_t mfat_get_bytes_per_fat( uint8_t dev_idx, uint8_t part_idx ) {
   uint32_t out = 0;
   out = mfat_get_sectors_per_fat( dev_idx, part_idx );
   out *= mfat_get_bytes_per_sector( dev_idx, part_idx );
   return out;
}

uint16_t mfat_get_root_dir_entries_count(
   uint8_t dev_idx, uint8_t part_idx
) {
   uint16_t out = 0;
   mbytesin_read_big_endian_u16( dev_idx, part_idx, 17, &out );
#ifdef DEBUG_TEST_IMG
   assert( 512 == out );
#endif /* DEBUG_TEST_IMG */
   return out;
}

uint32_t mfat_get_sectors_total( uint8_t dev_idx, uint8_t part_idx ) {
   uint32_t out = 0;
   mbytesin_read_big_endian_u32( dev_idx, part_idx, 32, &out );
   return out;
}

FILEPTR_T mfat_get_root_dir_offset( uint8_t dev_idx, uint8_t part_idx ) {
   FILEPTR_T dir_offset = 0;

   dir_offset = mfat_get_reserved_sectors( dev_idx, part_idx );
   dir_offset *= mfat_get_bytes_per_sector( dev_idx, part_idx );

   dir_offset += mfat_get_fat_count( dev_idx, part_idx ) *
      mfat_get_bytes_per_fat( dev_idx, part_idx );

#ifdef DEBUG_TEST_IMG
   printf( "rsb:%d\nbpf:%d\nspf:%d\nbps:%d\n0x%4x\n",
      mfat_get_reserved_sectors( dev_idx, part_idx ) *
         mfat_get_bytes_per_sector( dev_idx, part_idx ),
      mfat_get_bytes_per_fat( dev_idx, part_idx ),
      mfat_get_sectors_per_fat( dev_idx, part_idx ),
      mfat_get_bytes_per_sector( dev_idx, part_idx ),
      dir_offset );

   assert(
      0 == (dir_offset % mfat_get_sectors_per_cluster( dev_idx, part_idx ) ) );
   assert( 0xd800 == dir_offset );
#endif /* DEBUG_TEST_IMG */

   return dir_offset;
}

#ifndef CHECK
static
#endif /* !CHECK */
FILEPTR_T mfat_get_data_area_offset( uint8_t dev_idx, uint8_t part_idx ) {
   FILEPTR_T dir_offset = 0;

   dir_offset = mfat_get_root_dir_offset( dev_idx, part_idx );
   dir_offset += (mfat_get_root_dir_entries_count( dev_idx, part_idx ) *
      MFAT_DIR_ENTRY_SZ);

   assert( 0 == dir_offset % mfat_get_bytes_per_sector( dev_idx, part_idx ) );

   return dir_offset;
}

#ifndef CHECK
static
#endif /* !CHECK */
uint16_t mfat_get_cluster_size( uint8_t dev_idx, uint8_t part_idx ) {
   return mfat_get_sectors_per_cluster( dev_idx, part_idx ) *
      mfat_get_bytes_per_sector( dev_idx, part_idx );
}

#ifndef CHECK
static
#endif /* !CHECK */
FILEPTR_T mfat_get_cluster_data_offset(
   uint16_t cluster_idx, uint8_t dev_idx, uint8_t part_idx
) {
   uint16_t cluster_size = 0;
   FILEPTR_T data_offset = 0;

   /* FAT quirk; cluster IDs start at 2 while clusters don't. */
   cluster_idx -= 2;

   cluster_size = mfat_get_cluster_size( dev_idx, part_idx );

   data_offset = mfat_get_data_area_offset( dev_idx, part_idx );
#ifdef MFAT_DATA_DEBUG
   printf( "\n%d * %d\n", fat_idx, cluster_size );
#endif /* MFAT_DATA_DEBUG */
   data_offset += cluster_idx * cluster_size;

   return data_offset;
}

uint16_t mfat_get_entries_count( uint8_t dev_idx, uint8_t part_idx ) {
   uint16_t out;
   /* Each FAT entry is 2 bytes. */
   out = mfat_get_bytes_per_fat( dev_idx, part_idx ) / 2;
   return out;
}

uint16_t mfat_get_fat_entry(
   uint16_t cluster_idx, uint8_t fat_idx, uint8_t dev_idx, uint8_t part_idx
) {
   uint16_t out = 0;
   uint32_t entry_offset = 0;

   /* Move past the BPB. */
   entry_offset = mfat_get_reserved_sectors( dev_idx, part_idx );
   entry_offset *= mfat_get_bytes_per_sector( dev_idx, part_idx );

   entry_offset +=
      (mfat_get_bytes_per_fat( dev_idx, part_idx ) * fat_idx);
   entry_offset += (cluster_idx * 2); /* get_byte below grabs half an entry. */

   assert( cluster_idx < mfat_get_entries_count( dev_idx, part_idx ) );

   mbytesin_read_big_endian_u16( dev_idx, part_idx, entry_offset, &out );

   return out;
}

FILEPTR_T mfat_get_dir_entry_first_offset(
   FILEPTR_T dir_offset, uint8_t dev_idx, uint8_t part_idx
) {
   uint8_t entry_id = 0;
   uint8_t entry_attrib = 0;

   /* Hunt for the first actual entry (i.e. skip LFNs, etc). */
   entry_attrib = mfat_get_dir_entry_attrib( dir_offset, dev_idx, part_idx );
   entry_id = disk_get_byte( dev_idx, part_idx, dir_offset );
   while(
      (0xe5 == entry_id || MFAT_ATTRIB_LFN == (MFAT_ATTRIB_LFN & entry_attrib))
      && 0x00 != entry_id
   ) {
      dir_offset += MFAT_DIR_ENTRY_SZ;
      entry_attrib = mfat_get_dir_entry_attrib( dir_offset, dev_idx, part_idx );
      entry_id = disk_get_byte( dev_idx, part_idx, dir_offset );
   }
   
   return dir_offset;
}

static uint8_t mfat_filename_cmp(
   const char filename1[MFAT_FILENAME_LEN],
   const char filename2[MFAT_FILENAME_LEN]
) {
   int i = 0;
   char c1, c2;

   for( i = 0 ; MFAT_FILENAME_LEN > i ; i++ ) {
#ifdef ALPHA_PRESENT
      c1 = alpha_tolower( filename1[i] );
      c2 = alpha_tolower( filename2[i] );
#else
      c1 = filename1[i];
      c2 = filename2[i];
#endif /* ALPHA_PRESENT */
      /* Compare entry name with target. */
      if(
         c1 != c2 &&
         /* Skip spaces and .s. */
         ' ' != filename1[i] &&
         '.' != filename1[i] &&
         ' ' != filename2[i] &&
         '.' != filename2[i]
      ) {
         return 1;
      } else if( '\0' == filename1[i] || '\0' == filename2[i] ) {
         return 0;
      }
   }

   return 0;
}

FILEPTR_T mfat_get_dir_entry_offset(
   const char search_name[MFAT_FILENAME_LEN], uint8_t search_name_len,
   FILEPTR_T dir_offset, uint8_t dev_idx, uint8_t part_idx
) {
   FILEPTR_T offset_out = dir_offset;
   char entry_name[MFAT_FILENAME_LEN];
   
   while( 0 != disk_get_byte( dev_idx, part_idx, offset_out ) ) {
      mfat_get_dir_entry_name( entry_name, offset_out, dev_idx, part_idx );
      if( 0 == mfat_filename_cmp( entry_name, search_name ) ) {
         /* Found it, so skip to the end. */
         goto hit;
      }
      offset_out =
         mfat_get_dir_entry_next_offset( offset_out, dev_idx, part_idx );
      if( 0 == offset_out ) {
         break;
      }
   }
   
   /* Not found. */
   offset_out = 0;

hit:
   return offset_out;
}

FILEPTR_T mfat_get_dir_entry_next_offset(
   FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx
) {
   FILEPTR_T offset_out = offset;
   uint8_t entry_id = 0;
   uint8_t entry_attrib = 0;

   /* Loop through unused directory entries until we find a used one or just
    * reach the end of the directory. */
   do {
      offset_out += MFAT_DIR_ENTRY_SZ;
      entry_attrib = mfat_get_dir_entry_attrib( offset_out, dev_idx, part_idx );
      entry_id = disk_get_byte( dev_idx, part_idx, offset_out );
   } while(
      0x00 != entry_id &&
      (0xe5 == entry_id ||
      /* LFN entries are considered unused. */
      MFAT_ATTRIB_LFN == (MFAT_ATTRIB_LFN & entry_attrib))
   );

   if( 0x00 == entry_id ) {
      /* End of the directory. */
      return 0;
   } else {
      /* Found a used entry. */
      return offset_out;
   }
}

#ifndef CHECK
static
#endif /* CHECK */
uint16_t mfat_get_dir_entry_first_cluster_idx(
   FILEPTR_T entry_offset, uint8_t dev_idx, uint8_t part_idx
) {
   uint16_t out = 0;
   mbytesin_read_big_endian_u16( dev_idx, part_idx, entry_offset + 26, &out );
   return out;
}

#ifndef CHECK
static
#endif /* CHECK */
uint16_t mfat_get_dir_entry_n_cluster_idx(
   FILEPTR_T entry_offset, uint32_t* iter_offset_p,
   const uint16_t* cluster_size_p, uint32_t* file_size_p,
   uint8_t dev_idx, uint8_t part_idx
) {
   uint16_t cluster_idx_out = 0;

   /* Starting at the first cluster, find the cluster that contains the data
    * at the requested offset. Shrink down the provided offset until we're
    * within the bounds of the iterated cluster. That must be the cluster we
    * want!
    */
   cluster_idx_out = mfat_get_dir_entry_first_cluster_idx(
      entry_offset, dev_idx, part_idx );
   while( *iter_offset_p >= *cluster_size_p ) {
#ifdef MFAT_DATA_DEBUG
      printf( "\nseeking; %d >= %d\n", *iter_offset_p, cluster_size );
#endif /* MFAT_DATA_DEBUG */
      cluster_idx_out =
         mfat_get_fat_entry( cluster_idx_out, 0, dev_idx, part_idx );
      *iter_offset_p -= *cluster_size_p;
      *file_size_p -= *cluster_size_p;
   }

   return cluster_idx_out;
}

uint8_t mfat_get_dir_entry_data(
   FILEPTR_T entry_offset, uint32_t iter_file_offset, uint8_t* buffer,
   uint16_t blen, uint8_t dev_idx, uint8_t part_idx
) {
   uint16_t cluster_idx = 0;
   FILEPTR_T disk_cluster_offset = 0;
   uint8_t read = 0;
   uint32_t file_size = 0;
   uint16_t cluster_size = 0;

   file_size = mfat_get_dir_entry_size( entry_offset, dev_idx, part_idx );
   if( file_size <= iter_file_offset ) {
      /* Seek past end of file. */
      buffer[0] = '\0';
      return 0;
   }

   cluster_size = mfat_get_cluster_size( dev_idx, part_idx );

   /* We get inconsistencies/cluster overlaps if the buffer len isn't divisible
    * by 4.
    */
   assert( 0 == blen % 4 || iter_file_offset < cluster_size );

new_cluster:

   /* Grab the index of the cluster containing the requested chunk of the file
    * on the FAT. Then grab the offset in the data area.
    */
   cluster_idx = mfat_get_dir_entry_n_cluster_idx(
      entry_offset, &iter_file_offset, &cluster_size, &file_size,
      dev_idx, part_idx );
   disk_cluster_offset = mfat_get_cluster_data_offset(
      cluster_idx, dev_idx, part_idx );

   assert( mfat_get_cluster_size( dev_idx, part_idx ) <
      cluster_size + disk_cluster_offset );

#ifdef MFAT_DATA_DEBUG
   printf( "\ncluster start: %d\ncluster end: %d\n",
      disk_cluster_offset, cluster_end );
#endif /* MFAT_DATA_DEBUG */

   assert( iter_file_offset < cluster_size );
   /* Read from the disk into the buffer until we run out of file or the rest
    * of the requested data is in another cluster.
    */
   while( blen > read && file_size > iter_file_offset ) {
      assert( iter_file_offset < cluster_size );
      buffer[read] = disk_get_byte( dev_idx, part_idx,
         disk_cluster_offset + iter_file_offset );
      read++;
      iter_file_offset++;
      if( iter_file_offset >= cluster_size ) {
#ifdef MFAT_DATA_DEBUG
         printf( "\nbreaking: read '%c', %d + %d (%d) >= %d\n",
            buffer[read - 1],
            disk_cluster_offset, iter_file_offset,
            (disk_cluster_offset + iter_file_offset),
            cluster_size );
#endif /* MFAT_DATA_DEBUG */
         goto new_cluster;
      }
   }

   return read;
}

void mfat_get_dir_entry_name(
   char buffer[MFAT_FILENAME_LEN],
   FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx
) {
   int8_t src_i = 0, dest_i = 0;
   char c;

   /* Copy the entry name into the provided buffer. */
   for( src_i = 0 ; MFAT_FILENAME_LEN - 2 > src_i ; src_i++ ) {
      c = disk_get_byte( dev_idx, part_idx, offset + src_i );

      /* Just skip blanks. */
      if( ' ' != c ) {
         /* Only add the . if there's something after it. */
         if( 8 == src_i ) {
            buffer[dest_i++] = '.';
         }

         buffer[dest_i++] = c;
      }
   }

   /* Zero out the rest of the buffer. */
   while( dest_i < MFAT_FILENAME_LEN ) {
      buffer[dest_i++] = '\0';
   }
}

uint8_t mfat_get_dir_entry_cyear(
   FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx
) {
   uint16_t year_out = 0;

   year_out = disk_get_byte( dev_idx, part_idx, offset + 17 );
   year_out &= 0xfe;
   year_out >>= 1;
   
   return (uint8_t)year_out;
}

uint32_t mfat_get_dir_entry_size(
   FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx
) {
   uint32_t out = 0;
   mbytesin_read_big_endian_u32( dev_idx, part_idx, offset + 28, &out );
   return out;
}

uint8_t mfat_get_dir_entry_attrib(
   FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx
) {
   return disk_get_byte( dev_idx, part_idx, offset + 11 );
}

#ifndef USE_DISK_RO

FILEPTR_T mfat_get_dir_entry_free_offset(
   FILEPTR_T dir_offset, uint32_t dir_sz, uint8_t dev_idx, uint8_t part_idx
) {
   uint8_t entry_id = 0;
   uint32_t dir_end = dir_offset + dir_sz;

   /* Hunt for the first free entry. */
   entry_id = disk_get_byte( dev_idx, part_idx, dir_offset );
   while( dir_offset < dir_end ) {
#ifdef MFAT_DATA_DEBUG
      printf( "offset: %4x eid: %2x\n", dir_offset, entry_id );
#endif /* MFAT_DATA_DEBUG */
      if( 0xe5 == entry_id || 0x00 == entry_id ) {
         return dir_offset;
      }
      dir_offset += MFAT_DIR_ENTRY_SZ;
      entry_id = disk_get_byte( dev_idx, part_idx, dir_offset );
   }
   
   return 0; /* Could not find a free entry. */
}

void mfat_set_dir_entry_name(
   const char name[MFAT_FILENAME_LEN],
   FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx
) {
   uint8_t src_i = 0;
   uint8_t dest_i = 0;

   /* Copy the entry name from the provided buffer. */
   while( MFAT_FILENAME_LEN - 2 > dest_i && ' ' != name[src_i] ) {
      if( '.' == name[src_i] ) {
         while( 8 > dest_i ) {
            /* Pad out the filename with spaces. */
            disk_set_byte( ' ', dev_idx, part_idx, offset + dest_i );
            dest_i++;
         }
         src_i++;
         continue;
      }

      disk_set_byte( name[src_i], dev_idx, part_idx, offset + dest_i );
      src_i++;
      dest_i++;
   }
}

void mfat_set_dir_entry_cyear(
   uint8_t cyear, FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx
) {
   uint16_t year_buffer = 0;

   /* Get the current cdate. */
   year_buffer = disk_get_byte( dev_idx, part_idx, offset + 17 );
   cyear <<= 1;
   year_buffer &= cyear;

   /* Bitwise and the year into the date and write it back. */
   disk_set_byte(
      (uint8_t)(year_buffer & 0xff), dev_idx, part_idx, offset + 16 );
   year_buffer >>= 8;
   disk_set_byte(
      (uint8_t)(year_buffer & 0xff), dev_idx, part_idx, offset + 17 );
}

void mfat_set_dir_entry_size(
   uint32_t size, FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx
) {
   disk_set_byte(
      (uint8_t)(size & 0xff), dev_idx, part_idx, offset + 28 );
   size >>= 8;
   disk_set_byte(
      (uint8_t)(size & 0xff), dev_idx, part_idx, offset + 29 );
   size >>= 8;
   disk_set_byte(
      (uint8_t)(size & 0xff), dev_idx, part_idx, offset + 30 );
   size >>= 8;
   disk_set_byte(
      (uint8_t)(size & 0xff), dev_idx, part_idx, offset + 31 );
}

void mfat_set_dir_entry_attrib(
   uint8_t attrib, FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx
) {
   disk_set_byte( attrib, dev_idx, part_idx, offset + 11 );
}

#endif /* !USE_DISK_RO */



#ifndef MFAT_H
#define MFAT_H

/*! \file mfat.h */

#include <stdint.h>

/*! \brief An offset (in bytes) to a directory entry on disk. This will
 *         generally be used as a file "handle" for the rest of the library.
 *         Offsets for directories should point to the first directory entry.
 */
typedef uint32_t FILEPTR_T;

/*! \addtogroup mfat_attrib FAT Attributes
 *  \brief Possible values composing the FAT attribute byte.
 *  @{
 */

#define MFAT_ATTRIB_RO     0x01
#define MFAT_ATTRIB_HIDDEN 0x02
#define MFAT_ATTRIB_SYSTEM 0x04
#define MFAT_ATTRIB_VOL_ID 0x08
#define MFAT_ATTRIB_DIR    0x10
#define MFAT_ATTRIB_ARC    0x20
#define MFAT_ATTRIB_LFN    0x0f

/*! @} */

/*! \brief Maximum filename length. 8 chars + . + 3 char extension. */
#define MFAT_FILENAME_LEN  13

/*! \addtogroup mfat_fat FAT and BPB Information
 *  \brief Access drive/FS metadata from the BPB and block entries from the FAT.
 *  @{
 */

/*!  \brief Get the number of entries in the root directory as described in the
 *          BPB.
 */
uint16_t mfat_get_root_dir_entries_count( uint8_t dev_idx, uint8_t part_idx );

/*!  \brief Get the total number of sectors on the drive as described in the
 *          BPB.
 */
uint32_t mfat_get_sectors_total( uint8_t dev_idx, uint8_t part_idx );

/*!  \brief Get the number of bytes in each sectory on the drive as described
 *          in the BPB.
 */
uint16_t mfat_get_bytes_per_sector( uint8_t dev_idx, uint8_t part_idx );

/*!  \brief Get the total number of clusters mapped in the FAT.
 */
uint16_t mfat_get_entries_count( uint8_t dev_idx, uint8_t part_idx );

/*!  \brief Get a specific cluster from the FAT.
 *   @param cluster_idx The index of the cluster to lookup from the FAT.
 *   @param fat_idx The FAT from which to get the cluster (most filesystems
 *                  have >= 2 FATs.
 */
uint16_t mfat_get_fat_entry(
   uint16_t cluster_idx, uint8_t fat_idx, uint8_t dev_idx, uint8_t part_idx );

/*!  \brief Get the offset of the root directory as described in the BPB.
 */
FILEPTR_T mfat_get_root_dir_offset( uint8_t dev_idx, uint8_t part_idx );

/*! @} */

/*! \addtogroup mfat_dir Directory Entry Information
 *  \brief Access information for a specified directory entry.
 *  @{
 */

/*!  \brief Get the offset of the first valid entry in the specified directory.
 *          Skips LFN entries, blank entries, etc.
 *   @param dir_offset The offset (in bytes) of the first entry of the directory
 *                     to examine.
 */
FILEPTR_T mfat_get_dir_entry_first_offset(
   FILEPTR_T dir_offset, uint8_t dev_idx, uint8_t part_idx );

/*!  \brief Get the offset of the directory entry containing the specified
 *          filename.
 *   @param search_name A pointer to the buffer containing the filename to
 *                      search for. Spaces and the . will be ignored.
 *   @param dir_offset The offset (in bytes) of the first entry of the directory
 *                     to examine.
 */
FILEPTR_T mfat_get_dir_entry_offset(
   const char search_name[MFAT_FILENAME_LEN], uint8_t search_name_len,
   FILEPTR_T dir_offset, uint8_t dev_idx, uint8_t part_idx );

/*!  \brief Given a directory entry, return the next valid directory entry or
 *          0 if there are no more in this directory.
 *          Skips LFN entries/blank entries/etc.
 *   @param offset The offset (in bytes) of the directory entry to iterate from.
 */
FILEPTR_T mfat_get_dir_entry_next_offset(
   FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx );

/*!  \brief Get the filename stored in the specified directory entry.
 *   @param buffer A pointer to the buffer in which the requested filename will
 *                 be stored. While the buffer MUST be able to hold
 *                 #MFAT_FILENAME_LEN bytes, the returned filename will not
 *                 necessarily be that long.
 *   @param offset The offset (in bytes) of the directory entry to examine.
 */
void mfat_get_dir_entry_name(
   char buffer[MFAT_FILENAME_LEN],
   FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx );
uint8_t mfat_get_dir_entry_cyear(
   FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx );
uint32_t mfat_get_dir_entry_size(
   FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx );
uint8_t mfat_get_dir_entry_attrib(
   FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx );

/*! @} */

uint8_t mfat_get_dir_entry_data(
   FILEPTR_T entry_offset, FILEPTR_T file_offset,
   uint8_t* buffer, uint16_t blen,
   uint8_t dev_idx, uint8_t part_idx );

#ifndef USE_DISK_RO

/*!  \brief Mark the directory entry at the specified offset as available.
 */
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

#endif /* !USE_DISK_RO */

#endif /* MFAT_H */


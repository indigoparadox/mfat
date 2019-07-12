
#ifndef MEXT2_H
#define MEXT2_H

/*! \file mext2.h */

#include "mfatdef.h"

#define MEXT2_SUPERBLOCK_OFFSET 1024
#define MEXT2_SUPERBLOCK_SZ     1024
#define MEXT2_BGDT_OFFSET (MEXT2_SUPERBLOCK_OFFSET + MEXT2_SUPERBLOCK_SZ)
#define MEXT2_BGD_SZ 32

/*! \addtogroup inode_types inode File Types
 *  @{
 */
#define MEXT2_T_IFSOCK  0xC000   /*!< \brief Socket */
#define MEXT2_T_IFLNK   0xA000   /*!< \brief Symbolic Link */
#define MEXT2_T_IFREG   0x8000   /*!< \brief Regular File */
#define MEXT2_T_IFBLK   0x6000   /*!< \brief Block Device */
#define MEXT2_T_IFDIR   0x4000   /*!< \brief Directory */
#define MEXT2_T_IFCHR   0x2000   /*!< \brief Character Device */
#define MEXT2_T_IFIFO   0x1000   /*!< \brief FIFO */
/*! @} */

/*! \addtogroup mext2_metadata ext2 Filesystem Metadata
 *  @{
 */

/*! \brief Get the number of inodes total in this filesystem.
 */
uint32_t mext2_get_inode_count( uint8_t dev_idx, uint8_t part_idx );

/*! \brief Get the number of inodes in a block group on this filesystem.
 */
uint32_t mext2_get_inodes_per_group( uint8_t dev_idx, uint8_t part_idx );

/*! \brief Get the number of blocks total in this filesystem.
 */
uint32_t mext2_get_block_count( uint8_t dev_idx, uint8_t part_idx );

/*! \brief Get the number of blocks per block group on this filesystem.
 */
uint32_t mext2_get_blocks_per_group( uint8_t dev_idx, uint8_t part_idx );

/*! \brief Get the number of block groups total on this filesystem.
 */
uint32_t mext2_get_group_count( uint8_t dev_idx, uint8_t part_idx );

/*! \brief Get the ext2 implementation version number (major) of this
 *         filesystem.
 */
uint32_t mext2_get_major_version( uint8_t dev_idx, uint8_t part_idx );

/*! \brief Get the ext2 signature of this filesystem (should always be 0xef53).
 */
uint16_t mext2_get_signature( uint8_t dev_idx, uint8_t part_idx );

/*! \brief Get the size of a block (in bytes) on this filesystem.
 */
uint32_t mext2_get_block_size( uint8_t dev_idx, uint8_t part_idx );

/*! @} */

/*! \addtogroup mext2_bgd ext2 Block Group Descriptors
 *  @{
 */

/*! \brief Get the offset (in bytes) of a specified block group's descriptor.
 *         Information about the block group can then be extracted and used.
 *  @param idx The index of the block group to look up.
 */
#define mext2_get_bgd_offset( idx ) \
   (MEXT2_BGDT_OFFSET + (idx * MEXT2_BGD_SZ))

/*! \brief Get the number of directories in a block group from its
 *         descriptor.
 *  @param offset The offset (in bytes) of the block group descriptor
 *                (See #mext2_get_bgd_offset).
 */
uint16_t mext2_get_bgd_dir_count(
   FILEPTR_T offset, uint8_t dev_idx, uint8_t part_idx );

/*! @} */

#endif /* MEXT2_H */


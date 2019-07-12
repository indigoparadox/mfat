
#ifndef MFATDEF_H
#define MFATDEF_H

#include <stdint.h>

/*! \file mfatdef.h */

/*! \brief An offset (in bytes) to a directory entry on disk. This will
 *         generally be used as a file "handle" for the rest of the library.
 *         Offsets for directories should point to the first directory entry.
 */
typedef uint32_t FILEPTR_T;

typedef uint32_t INODE_T;

#endif /* MFATDEF_H */


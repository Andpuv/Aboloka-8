# ifndef __CORE_ABOLOKA_8_RAM_H__
#   define __CORE_ABOLOKA_8_RAM_H__

#   include <stdint.h>
#   include <stddef.h>
#   include <stdbool.h>

#   include "aboloka-8-api.h"

#   define ABOLOKA_8_RAM_SIZE_LOG2 16
#   define ABOLOKA_8_RAM_SIZE                                                  \
  (                                                                            \
    (size_t)1 << ABOLOKA_8_RAM_SIZE_LOG2                                       \
  )

struct aboloka_8_ram_t {
  uint8_t data [ ABOLOKA_8_RAM_SIZE ];
};

__ABOLOKA_8_CORE_API__
struct aboloka_8_ram_t * aboloka_8_ram_construct (
  void
);

__ABOLOKA_8_CORE_API__
void aboloka_8_ram_deconstruct (
  struct aboloka_8_ram_t * self
);

__ABOLOKA_8_CORE_API__
bool aboloka_8_ram_initialize (
  struct aboloka_8_ram_t * self
);

__ABOLOKA_8_CORE_API__
void aboloka_8_ram_deinitialize (
  struct aboloka_8_ram_t * self
);

__ABOLOKA_8_CORE_API__
bool aboloka_8_ram_read (
  struct aboloka_8_ram_t * self,
  uint16_t                 addr,
  uint8_t *                data
);

__ABOLOKA_8_CORE_API__
bool aboloka_8_ram_write (
  struct aboloka_8_ram_t * self,
  uint16_t                 addr,
  uint8_t                  data
);

__ABOLOKA_8_CORE_API__
bool aboloka_8_ram_refresh (
  struct aboloka_8_ram_t * self
);

# endif /* __CORE_ABOLOKA_8_RAM_H__ */
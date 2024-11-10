# include "../../inc/core/aboloka-8-ram.h"
# include <assert.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>

__ABOLOKA_8_CORE_API__
struct aboloka_8_ram_t * aboloka_8_ram_construct (
  void
)
{
  /* Acquire resources used by this instance. */
  struct aboloka_8_ram_t * self =
    (struct aboloka_8_ram_t *)malloc(
      sizeof(*self)
    );

  if ( /* /unlikely/ */ !self ) {
    /* /discard/ */ fprintf(stderr,
      "[ FATAL ] Cannot construct the RAM device:\n"
      "          %s.",
      strerror(errno)
    );

    /* Abort the execution safely. */
    exit(EXIT_FAILURE);
  }

  if ( /* /likely/ */ aboloka_8_ram_initialize(self) )
    return self;

  /* Release resources used by this instance. */
  free(self);

  return (struct aboloka_8_ram_t *)NULL;
}

__ABOLOKA_8_CORE_API__
void aboloka_8_ram_deconstruct (
  struct aboloka_8_ram_t * self
)
{
  aboloka_8_ram_deinitialize(self);

  /* Release resources used by this instance. */
  free(self);
}

__ABOLOKA_8_CORE_API__
bool aboloka_8_ram_initialize (
  struct aboloka_8_ram_t * self
)
{
  assert(self);

  /* Clear the RAM data. */
  for ( size_t index = (size_t)0; index < sizeof(self->data); ++index ) {
    self->data[ index ] = UINT8_C(0x00);
  }

  /* NOTE(Andpuv) [4]:                                                         *
   * In future versions, for bigger RAMs, malloc()'d data can be used. If the  *
   * allocation fails, this routine returns false.                             *
   */
  return true;
}

__ABOLOKA_8_CORE_API__
void aboloka_8_ram_deinitialize (
  struct aboloka_8_ram_t * self
)
{
  assert(self);

  /* /nothing-to-do/ */
}

__ABOLOKA_8_CORE_API__
bool aboloka_8_ram_read (
  struct aboloka_8_ram_t * self,
  uint16_t                 addr,
  uint8_t *                data
)
{
  assert(self);

  *data = self->data[ addr ];

  /* NOTE(Andpuv) [1]:                                                         *
   * In future versions, latency simulation to RAM accesses can be added as an *
   * option to make Aboloka-8 more realistic.                                  *
   * In that configuration, the return value of those routines help the CPU to *
   * understand if the RAM have already completed the operation or not.        *
   */
  return true;
}

__ABOLOKA_8_CORE_API__
bool aboloka_8_ram_write (
  struct aboloka_8_ram_t * self,
  uint16_t                 addr,
  uint8_t                  data
)
{
  assert(self);

  self->data[ addr ] = data;

  /* See NOTE(Andpuv) [1]. */
  return true;
}

__ABOLOKA_8_CORE_API__
bool aboloka_8_ram_refresh (
  struct aboloka_8_ram_t * self
)
{
  assert(self);

  /* NOTE(Andpuv) [2]:                                                         *
   * In future versions, latency simulation to RAM refresh cycles can be added *
   * as an option to make Aboloka-8 more realistic.                            *
   * In that configuration, the return value of those routines help the CPU to *
   * understand if the RAM have already completed the refresh or not.          *
   *                                                                           *
   * NOTE(Andpuv) [3]:                                                         *
   * The option in Note(Andpuv) [2] can be applied only if Aboloka-8 uses DRAM *
   * (Dynamic RAM) as the main memory. If it uses SRAM (Static RAM) the        *
   * refresh has no latency.                                                   *
   */
  return true;
}
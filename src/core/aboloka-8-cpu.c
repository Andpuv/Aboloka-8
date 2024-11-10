# include "../../inc/core/aboloka-8-cpu.h"
# include "../../inc/core/aboloka-8-ram.h"
# include <assert.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>

__ABOLOKA_8_CORE_API__
struct aboloka_8_cpu_t * aboloka_8_cpu_construct (
  struct aboloka_8_ram_t * ram
)
{
  /* Acquire resources used by this instance. */
  struct aboloka_8_cpu_t * self =
    (struct aboloka_8_cpu_t *)malloc(
      sizeof(*self)
    );

  if ( /* /unlikely/ */ !self ) {
    /* /discard/ */ fprintf(stderr,
      "[ FATAL ] Cannot construct the CPU device:\n"
      "          %s.",
      strerror(errno)
    );

    /* Abort the execution safely. */
    exit(EXIT_FAILURE);
  }

  if ( /* /likely/ */ aboloka_8_cpu_initialize(self, ram) )
    return self;

  /* Release resources used by this instance. */
  free(self);

  return (struct aboloka_8_cpu_t *)NULL;
}

__ABOLOKA_8_CORE_API__
void aboloka_8_cpu_deconstruct (
  struct aboloka_8_cpu_t * self
)
{
  aboloka_8_cpu_deinitialize(self);

  /* Release resources used by this instance. */
  free(self);
}

__ABOLOKA_8_CORE_API__
bool aboloka_8_cpu_initialize (
  struct aboloka_8_cpu_t * self,
  struct aboloka_8_ram_t * ram
)
{
  assert(self);

  if ( !ram ) {
    ram = aboloka_8_ram_construct();

    if ( /* /unlikely/ */ !ram )
      return false;

    self->__owns_ram__ = true;
  } else {
    self->__owns_ram__ = false;
  }

  self->ram = ram;

  self->cycles       = UINT16_C(0);
  self->stage        = 0x0;
  self->target_stage = 0x0;
  self->quit         = false;
  self->mcr          = UINT8_C(0x00);
  self->mrr          = UINT8_C(0x00);

  /* TODO(Andpuv) [1]: Other initialization. */

  return true;
}

__ABOLOKA_8_CORE_API__
void aboloka_8_cpu_deinitialize (
  struct aboloka_8_cpu_t * self
)
{
  assert(self);

  if ( self->__owns_ram__ ) {
    aboloka_8_ram_deconstruct(self->ram);
    self->ram = (struct aboloka_8_ram_t *)NULL;
  }
}

# include "aboloka-8-cpu-impl.h"

__ABOLOKA_8_CORE_API__
uint8_t * aboloka_8_cpu_reg (
  struct aboloka_8_cpu_t * self,
  int                      reg_id
)
{
  assert(self);

  if ( self->csr & ABOLOKA_8_CPU_UF ) {
    reg_id |= 0x4;
  }

  return self->regs + reg_id;
}

__ABOLOKA_8_CORE_API__
uint8_t * aboloka_8_cpu_seg (
  struct aboloka_8_cpu_t * self,
  int                      seg_id
)
{
  assert(self);

  if ( self->csr & ABOLOKA_8_CPU_UF ) {
    seg_id |= 0x4;
  }

  return self->segs + seg_id;
}

__ABOLOKA_8_CORE_API__
bool aboloka_8_cpu_intr (
  struct aboloka_8_cpu_t * self,
  int                      int_id
)
{
  assert(self && int_id < ABOLOKA_8_CPU_N_IRQS);

  int_id = UINT8_C(0x1) << int_id;

  if ( self->irr & int_id )
    /* The IRQ line is already used. */
    return false;

  self->irr |= int_id;
  return true;
}

__ABOLOKA_8_CORE_API__
bool aboloka_8_cpu_inta (
  struct aboloka_8_cpu_t * self,
  int                      int_id
)
{
  assert(self && int_id < ABOLOKA_8_CPU_N_IRQS);

  int_id = UINT8_C(0x1) << int_id;
  return !( self->irr & int_id );
}

__ABOLOKA_8_CORE_API__
bool aboloka_8_cpu_halt (
  struct aboloka_8_cpu_t * self,
  bool                     is_halt
)
{
  assert(self);

  if ( is_halt ) {
    if ( self->quit || self->stage < 0x0 )
      /* The CPU is already halted. */
      return false;

    self->target_stage = self->stage;
    self->stage = -1;
  } else {
    if ( self->quit )
      /* The CPU is halted (powered off). */
      return false;

    if ( 0x0 <= self->stage )
      /* The CPU is already running. */
      return false;

    self->stage = self->target_stage;
  }

  return true;
}

__ABOLOKA_8_CORE_API__
bool aboloka_8_cpu_cycle (
  struct aboloka_8_cpu_t * self
)
{
  assert(self);

  if ( self->quit )
    return false;

  if ( self->mrr <= self->mcr ) {
    self->target_stage = self->stage;
    self->stage        = 0x5; /* Go to refresh stage. */
  } else if ( UINT8_C(0x00) != self->mrr ) {
    ++self->mcr;
  }

  if ( self->stage < 0x0 ) {
    int int_id = UINT8_C(0x1) << ABOLOKA_8_CPU_WAKE_UP;

    if ( !( self->irr & int_id ) )
      return true;

    self->irr  &= ~int_id;
    self->stage = self->target_stage;
  }

  switch ( self->stage ) {
  case 0x0: {
    if ( !cpu_fetch(self) )
      break;

    self->stage = 0x1;
  } /* /fall-through/ */
  case 0x1: {
    if ( !cpu_decode(self) )
      break;

    self->stage = 0x2;
  } /* /fall-through/ */
  case 0x2: {
    if ( !cpu_execute(self) )
      break;

    self->stage = 0x3;
  } /* /fall-through/ */
  case 0x3: {
    if ( !cpu_access(self) )
      break;

    self->stage = 0x4;
  } /* /fall-through/ */
  case 0x4: {
    if ( self->ins.stage < self->ins.stages ) {
      ++self->ins.stage;
      self->stage = 0x2; /* Go back to execution stage. */
    } else {
      self->stage = 0x0; /* Go back to fetch stage. */
    }
  } break;

  case 0x5: {
    if ( !aboloka_8_ram_refresh(self->ram) )
      break;

    self->mcr   = UINT8_C(0x00);
    self->stage = self->target_stage;
  } break;
  }

  ++self->cycles;

  return true;
}
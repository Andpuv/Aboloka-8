# include "../../inc/core/aboloka-8-cpu.h"
# include "../../inc/core/aboloka-8-ram.h"
# include <assert.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include "aboloka-8-cpu-legacy.h"
# include "aboloka-8-cpu-impl.h"

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

  /* Reset the CPU. */
  while ( !aboloka_8_cpu_reset(self) );

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

  if ( self->csr & CPU_UF ) {
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
  assert(self && int_id < CPU_N_IRQS);

  if ( self->irr[ int_id ] & UINT8_C(0x1) )
    return false;

  self->irr[ int_id ] |= UINT8_C(0x1);
  return true;
}

__ABOLOKA_8_CORE_API__
bool aboloka_8_cpu_inta (
  struct aboloka_8_cpu_t * self,
  int                      int_id
)
{
  assert(self && int_id < CPU_N_IRQS);

  return !( self->irr[ int_id ] & UINT8_C(0x1) );
}

__ABOLOKA_8_CORE_API__
bool aboloka_8_cpu_halt (
  struct aboloka_8_cpu_t * self,
  bool                     is_halt
)
{
  assert(self);

  if ( is_halt ) {
    if ( self->quit || CPU_STAGE_HALT == self->stage )
      return false;

    self->target_stage = self->stage;
    self->stage        = CPU_STAGE_HALT;
  } else {
    if ( self->quit || CPU_STAGE_HALT != self->stage )
      return false;

    self->stage        = self->target_stage;
    self->target_stage = CPU_STAGE_FETCH;
  }

  return true;
}

__ABOLOKA_8_CORE_API__
bool aboloka_8_cpu_reset (
  struct aboloka_8_cpu_t * self
)
{
  assert(self);

  self->quit         = false;
  self->stage        = CPU_STAGE_IDLE;
  self->target_stage = CPU_STAGE_FETCH;
  self->cycles       = UINT16_C(0);
  self->ip           = UINT8_C(0x00);
  self->csr          = CPU_LF;
  self->mcr          = UINT8_C(0x00);
  self->mrr          = UINT8_C(0x00);
  self->idt          = UINT16_C(0x0000);
  self->mdt          = UINT16_C(0x0000);

  for ( int index = 0; index < CPU_N_IRQS; ++index ) {
    self->imr[ index ] = UINT8_C(0x00);
  }

  for ( int index = 0; index < CPU_N_IRQS; ++index ) {
    self->irr[ index ] = UINT8_C(0x00);
  }

  for ( int index = 0; index < CPU_N_REGS; ++index ) {
    self->regs[ index ] = UINT8_C(0x00);
  }

  for ( int index = 0; index < CPU_N_SEGS; ++index ) {
    self->segs[ index ] = UINT8_C(0x00);
  }

  self->ins.ip       = UINT16_C(0x00);
  self->ins.ip_queue = 0;
  self->ins.opcode   = 0;
  self->ins.stages   = 0;
  self->ins.stage    = 0;
  self->ins.access   = CPU_NO_ACCESS;
  self->ins.index    = 0;
  self->ins.dst_id   = 0;
  self->ins.src_id   = 0;
  self->ins.dst      = UINT8_C(0x00);
  self->ins.src      = UINT8_C(0x00);
  self->ins.seg      = UINT8_C(0x00);
  self->ins.ofs      = UINT8_C(0x00);
  self->ir.front     = -1;
  self->ir.rear      = -1;

  for ( int index = 0; index < (int)sizeof(self->ir.queue); ++index ) {
    self->ir.queue[ index ] = UINT8_C(0x00);
  }

  self->isa.idle    = cpu_legacy_idle;
  self->isa.fetch   = cpu_legacy_fetch;
  self->isa.decode  = cpu_legacy_decode;
  self->isa.execute = cpu_legacy_execute;
  self->isa.access  = cpu_legacy_access;

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
    self->stage        = CPU_STAGE_REFRESH;
  } else if ( UINT8_C(0x00) != self->mrr ) {
    ++self->mcr;
  }

  switch ( self->stage ) {
  case CPU_STAGE_HALT:
    break;

  case CPU_STAGE_IDLE: {
    if ( self->isa.idle(self) )
      break;

    self->stage = self->target_stage;
  } break;

  case CPU_STAGE_FETCH: {
    if ( !self->isa.fetch(self) )
      break;

    self->stage = CPU_STAGE_DECODE;
  } /* /fall-through/ */
  case CPU_STAGE_DECODE: {
    if ( !self->isa.decode(self) )
      break;

    self->stage = CPU_STAGE_ACCESS_0;
  } /* /fall-through/ */
  case CPU_STAGE_ACCESS_0: {
    if ( !self->isa.access(self) )
      break;

    self->stage = CPU_STAGE_EXECUTE;
  } /* /fall-through/ */
  case CPU_STAGE_EXECUTE: {
    if ( !self->isa.execute(self) )
      break;

    self->stage = CPU_STAGE_ACCESS_1;
  } /* /fall-through/ */
  case CPU_STAGE_ACCESS_1: {
    if ( !self->isa.access(self) )
      break;

    if ( self->ins.stage < self->ins.stages ) {
      ++self->ins.stage;
      self->stage = CPU_STAGE_ACCESS_0;
    } else {
      self->stage = CPU_STAGE_FETCH;
    }
  } break;

  case CPU_STAGE_REFRESH: {
    if ( !aboloka_8_ram_refresh(self->ram) )
      break;

    self->mcr   = UINT8_C(0x00);
    self->stage = self->target_stage;
  } break;
  }

  if ( CPU_TF == ( self->csr & ( CPU_TF | CPU_LF ) ) ) {
    cpu_exception(self, CPU_EXCEPTION_CAUSE_SS);
  }

  ++self->cycles;
  return true;
}

__ABOLOKA_8_CORE_API__
bool aboloka_8_cpu_is_halted (
  struct aboloka_8_cpu_t * self
)
{
  return self->quit || ( CPU_STAGE_HALT == self->stage );
}

# include "aboloka-8-cpu-impl.h"
# include "aboloka-8-cpu-legacy.h"

void cpu_update_flags (
  struct aboloka_8_cpu_t * self,
  bool                     cf,
  bool                     zf,
  bool                     sf,
  bool                     of
)
{
  self->csr &= ~( CPU_CF | CPU_ZF | CPU_SF | CPU_OF );
  self->csr |=
    ( cf * CPU_CF ) |
    ( zf * CPU_ZF ) |
    ( sf * CPU_SF ) |
    ( of * CPU_OF );
}

uint8_t cpu_add_and_update_flags (
  struct aboloka_8_cpu_t * self,
  uint8_t                  src_0,
  uint8_t                  src_1
)
{
  uint16_t res = (uint16_t)src_0 + (uint16_t)src_1;

  bool src_0_sf = UINT8_C(0x00) != ( src_0 & UINT8_C(0x80) );
  bool src_1_sf = UINT8_C(0x00) != ( src_1 & UINT8_C(0x80) );
  bool cf       = UINT16_C(0x00) != ( res >> 8 );
  bool zf       = UINT16_C(0x00) == ( res & UINT16_C(0xFF) );
  bool sf       = UINT16_C(0x00) != ( res & UINT16_C(0x80) );
  bool of       = src_0_sf == src_1_sf && src_0_sf != sf;

  cpu_update_flags(self, cf, zf, sf, of);

  return (uint8_t)res;
}

uint8_t cpu_sub_and_update_flags (
  struct aboloka_8_cpu_t * self,
  uint8_t                  src_0,
  uint8_t                  src_1
)
{
  return cpu_add_and_update_flags(self, src_0, ~src_1 + UINT8_C(1));
}

void cpu_update_flags_after_and_or_xor (
  struct aboloka_8_cpu_t * self,
  uint8_t                  res
)
{
  bool cf = false;
  bool zf = UINT8_C(0x00) == res;
  bool sf = UINT8_C(0x00) != ( res & UINT8_C(0x80) );
  bool of = false;

  cpu_update_flags(self, cf, zf, sf, of);
}

uint8_t cpu_sll_and_update_flags (
  struct aboloka_8_cpu_t * self,
  uint8_t                  src_0,
  uint8_t                  src_1
)
{
  uint16_t res = (uint16_t)src_0 << (int)( (unsigned int)src_1 );

  bool cf = UINT16_C(0x00) != ( res >> 8 );
  bool zf = UINT16_C(0x00) == ( res & UINT16_C(0xFF) );
  bool sf = UINT16_C(0x00) != ( res & UINT16_C(0x80) );
  bool of = false;

  cpu_update_flags(self, cf, zf, sf, of);

  return (uint8_t)res;
}

uint8_t cpu_srl_and_update_flags (
  struct aboloka_8_cpu_t * self,
  uint8_t                  src_0,
  uint8_t                  src_1
)
{
  uint16_t res = (uint16_t)src_0 >> (int)( (unsigned int)src_1 );

  bool cf = UINT16_C(0x00) != ( res >> 8 );
  bool zf = UINT16_C(0x00) == ( res & UINT16_C(0xFF) );
  bool sf = UINT16_C(0x00) != ( res & UINT16_C(0x80) );
  bool of = false;

  cpu_update_flags(self, cf, zf, sf, of);

  return (uint8_t)res;
}

uint8_t cpu_sra_and_update_flags (
  struct aboloka_8_cpu_t * self,
  uint8_t                  src_0,
  uint8_t                  src_1
)
{
  uint16_t res = (uint16_t)(
    (int16_t)( (int8_t)src_0 ) >> (int)( (unsigned int)src_1 )
  );

  bool cf = UINT16_C(0x00) != ( res >> 8 );
  bool zf = UINT16_C(0x00) == ( res & UINT16_C(0xFF) );
  bool sf = UINT16_C(0x00) != ( res & UINT16_C(0x80) );
  bool of = false;

  cpu_update_flags(self, cf, zf, sf, of);

  return (uint8_t)res;
}

void cpu_interruption (
  struct aboloka_8_cpu_t * self,
  uint8_t                  int_id,
  uint8_t                  cause
)
{
  self->irr[ int_id ] |= UINT8_C(1) << cause;
}

void cpu_exception (
  struct aboloka_8_cpu_t * self,
  uint8_t                  cause
)
{
  self->irr[ CPU_IRQ_0 ] |= UINT8_C(1) << cause;
}

bool cpu_read_mem (
  struct aboloka_8_cpu_t * self,
  uint8_t                  seg,
  uint8_t                  ofs,
  uint8_t *                data
)
{
  uint16_t addr = ( (uint16_t)seg << 8 ) | (uint16_t)ofs;
  return aboloka_8_ram_read(self->ram, addr, data);
}

bool cpu_write_mem (
  struct aboloka_8_cpu_t * self,
  uint8_t                  seg,
  uint8_t                  ofs,
  uint8_t                  data
)
{
  uint16_t addr = ( (uint16_t)seg << 8 ) | (uint16_t)ofs;
  return aboloka_8_ram_write(self->ram, addr, data);
}

bool cpu_idle ( struct aboloka_8_cpu_t * self )
{
  int int_id = UINT8_C(0x1) << CPU_EXCEPTION_CAUSE_WU;

  if ( !( self->irr[ CPU_IRQ_0 ] & int_id ) )
    return true;

  self->irr[ CPU_IRQ_0 ] &= ~int_id;
  self->stage = self->target_stage;

  return false;
}

bool cpu_fetch ( struct aboloka_8_cpu_t * self )
{
  return true;
}

bool cpu_decode ( struct aboloka_8_cpu_t * self )
{
  self->ins.stages = 0;
  self->ins.stage  = 0;
  self->ins.access = CPU_NO_ACCESS;

  return true;
}

bool cpu_execute ( struct aboloka_8_cpu_t * self )
{
  switch ( self->ins.opcode ) {
  case 0x00: {
    self->stage = CPU_STAGE_IDLE;
  } break;

  case 0x01: {
    self->quit  = true;
    self->stage = CPU_STAGE_IDLE;
  } break;

  case 0x6E: {
    self->stage       = CPU_STAGE_FETCH;
    self->isa.idle    = cpu_legacy_idle;
    self->isa.fetch   = cpu_legacy_fetch;
    self->isa.decode  = cpu_legacy_decode;
    self->isa.execute = cpu_legacy_execute;
    self->isa.access  = cpu_legacy_access;
  } break;

  case 0x6F: {
    self->ins.access = CPU_ACCESS_REG_WRITE;
    self->ins.dst_id = CPU_AX;
    self->ins.dst    = self->unique_id;
  } break;

  default: {
    cpu_exception(self, CPU_EXCEPTION_CAUSE_UD);
  } break;
  }

  return true;
}

bool cpu_access ( struct aboloka_8_cpu_t * self )
{
  switch ( self->ins.access ) {
  case CPU_ACCESS_REG_WRITE: {
    *aboloka_8_cpu_reg(self, self->ins.dst_id) = self->ins.dst;
  } break;

  case CPU_ACCESS_REG_READ: {
    self->ins.src = *aboloka_8_cpu_reg(self, self->ins.src_id);
  } break;

  case CPU_ACCESS_SEG_WRITE: {
    *aboloka_8_cpu_seg(self, self->ins.dst_id) = self->ins.dst;
  } break;

  case CPU_ACCESS_SEG_READ: {
    self->ins.src = *aboloka_8_cpu_seg(self, self->ins.src_id);
  } break;

  case CPU_ACCESS_MEM_WRITE:
    return cpu_write_mem(self,
      self->ins.seg,
      self->ins.ofs,
      self->ins.dst
    );

  case CPU_ACCESS_MEM_READ:
    return cpu_read_mem(self,
      self->ins.seg,
      self->ins.ofs,
      &self->ins.src
    );
  }

  self->ins.access = CPU_NO_ACCESS;

  return true;
}

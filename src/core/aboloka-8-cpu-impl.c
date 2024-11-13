# include "aboloka-8-cpu-impl.h"

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

  bool dst_sf = UINT8_C(0x00) != ( self->ins.dst & UINT8_C(0x80) );
  bool src_sf = UINT8_C(0x00) != ( self->ins.src & UINT8_C(0x80) );

  bool cf = UINT16_C(0x00) != ( res >> 8 );
  bool zf = UINT16_C(0x00) == ( res & UINT16_C(0xFF) );
  bool sf = UINT16_C(0x00) != ( res & UINT16_C(0x80) );
  bool of = dst_sf == src_sf && dst_sf != sf;

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

bool cpu_read (
  struct aboloka_8_cpu_t * self,
  uint8_t                  seg,
  uint8_t                  ofs,
  uint8_t *                val
)
{
  return true;
}

bool cpu_write (
  struct aboloka_8_cpu_t * self,
  uint8_t                  seg,
  uint8_t                  ofs,
  uint8_t                  val
)
{
  return true;
}

bool cpu_fetch ( struct aboloka_8_cpu_t * self )
{
  return true;
}

bool cpu_decode ( struct aboloka_8_cpu_t * self )
{
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
    self->csr  &= ~CPU_1F;
  } break;

  case 0x02:
  case 0x04: case 0x05: case 0x06: case 0x07: {
    switch ( self->ins.stage ) {
    case 0: {
      self->ins.stages = 2;
      self->ins.stage  = 0;
      self->pc         = self->ins.dst;
      self->ins.access = CPU_ACCESS_SEG_READ;
      self->ins.src_id = CPU_ES;
    } break;

    case 1: {
      self->ins.access = CPU_ACCESS_SEG_WRITE;
      self->ins.dst_id = CPU_CS;
      self->ins.dst    = self->ins.src;
    } break;
    }

    ++self->ins.stage;
  } break;

  case 0x03:
  case 0x08: case 0x09: case 0x0A: case 0x0B: {
    switch ( self->ins.stage ) {
    case 0: {
      int16_t pc =
        (int16_t)self->pc +
        (int16_t)( (int8_t)self->ins.dst );

      if ( pc < INT16_C(0) || CPU_SEG_SIZE <= pc ) {
        self->ins.stages = 2;
        self->ins.stage  = 0;
        self->ins.pc     = pc % CPU_SEG_SIZE;
        self->ins.dst    = pc / CPU_SEG_SIZE;
        self->ins.access = CPU_ACCESS_SEG_READ;
        self->ins.src_id = CPU_ES;
      } else {
        self->ins.stages = 1;
        self->ins.stage  = 0;
        self->ins.pc     = pc % CPU_SEG_SIZE;
      }
    } break;

    case 1: {
      self->ins.access = CPU_ACCESS_SEG_WRITE;
      self->ins.dst_id = CPU_CS;
      self->ins.dst   += self->ins.src;
    } break;
    }

    ++self->ins.stage;
  } break;

  case 0x0C: {

  } break;

  case 0x0D: {

  } break;

  case 0x0E: {

  } break;

  case 0x10: case 0x11: case 0x12: case 0x13: {
    switch ( self->ins.stage ) {
    case 0: {
      self->ins.stages = 1;
      self->ins.stage  = 0;
      self->ins.access = CPU_ACCESS_REG_WRITE;
      self->ins.dst    = self->ins.src;
    } break;
    }

    ++self->ins.stage;
  } break;

  case 0x14: {

  } break;

  case 0x15: case 0x16: case 0x17: {
    switch ( self->ins.stage ) {
    case 0: {
      self->ins.stages = 1;
      self->ins.stage  = 0;
      self->ins.access = CPU_ACCESS_REG_WRITE;
      self->ins.dst_id = CPU_A;
      self->ins.dst    = self->ins.src;
    } break;
    }

    ++self->ins.stage;
  } break;

  case 0x18: case 0x19: case 0x1A: case 0x1B: {
    switch ( self->ins.stage ) {
    case 0: {
      self->ins.stages = 1;
      self->ins.stage  = 0;
      self->ins.access = CPU_ACCESS_REG_WRITE;
      self->ins.dst    = self->ins.src;
    } break;
    }

    ++self->ins.stage;
  } break;

  case 0x1C: {

  } break;

  case 0x1D: case 0x1E: case 0x1F: {
    switch ( self->ins.stage ) {
    case 0: {
      self->ins.stages = 2;
      self->ins.stage  = 0;
      self->ins.access = CPU_ACCESS_REG_WRITE;
      self->ins.dst    = self->ins.dst ^ self->ins.src;
      self->ins.src    = self->ins.dst ^ self->ins.src;
      self->ins.dst    = self->ins.dst ^ self->ins.src;
    } break;

    case 1: {
      self->ins.access = CPU_ACCESS_REG_WRITE;
      self->ins.dst_id = self->ins.src_id;
      self->ins.dst    = self->ins.src;
    } break;
    }

    ++self->ins.stage;
  } break;

  case 0x30: {
    self->ins.stages = 0;
    self->ins.stage  = 0;

    if ( self->csr & CPU_UF ) {
      cpu_exception(self, CPU_EXCEPTION_CAUSE_KP);
    } else {
      self->csr |= CPU_IF;
    }
  } break;

  case 0x31: {
    self->ins.stages = 0;
    self->ins.stage  = 0;

    if ( self->csr & CPU_UF ) {
      cpu_exception(self, CPU_EXCEPTION_CAUSE_KP);
    } else {
      self->csr &= ~CPU_IF;
    }
  } break;

  case 0x50: case 0x51: case 0x52: case 0x53: {
    switch ( self->ins.stage ) {
    case 0: {
      self->ins.stages = 1;
      self->ins.stage  = 0;
      self->ins.access = CPU_ACCESS_REG_WRITE;
      ++self->ins.dst;
    } break;
    }

    ++self->ins.stage;
  } break;

  case 0x54: case 0x55: case 0x56: case 0x57: {
    switch ( self->ins.stage ) {
    case 0: {
      self->ins.stages = 1;
      self->ins.stage  = 0;
      self->ins.access = CPU_ACCESS_REG_WRITE;
      --self->ins.dst;
    } break;
    }

    ++self->ins.stage;
  } break;

  case 0x60: case 0x61: case 0x62: case 0x63:
  case 0x64: case 0x65: case 0x66: case 0x67:
  case 0x68: case 0x69: case 0x6A: case 0x6B:
  case 0x6C: case 0x6D: case 0x6E: case 0x6F: {
    switch ( self->ins.stage ) {
    case 0: {
      self->ins.stages = 2;
      self->ins.stage  = 0;
      self->ins.access = CPU_ACCESS_REG_READ;
      self->ins.src_id = CPU_A;
    } break;

    case 1: {
      self->ins.access = CPU_ACCESS_REG_WRITE;
      self->ins.dst_id = CPU_A;

      if ( self->ins.opcode ) {
        self->ins.src ^= self->ins.dst;
      }

      self->ins.dst = self->ins.src;

      cpu_update_flags(self,
        false,
        is_zero(self->ins.dst),
        is_sign(self->ins.dst),
        false
      );
    } break;
    }

    ++self->ins.stage;
  } break;

  case 0x0F:
  case 0x8C: case 0x8D: case 0x8E: case 0x8F:
  case 0xAC:
  case 0xBA: case 0xBB:
    /* /fall-through/ */
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
    return cpu_write(self,
      self->ins.seg,
      self->ins.ofs,
      self->ins.dst
    );

  case CPU_ACCESS_MEM_READ:
    return cpu_read(self,
      self->ins.seg,
      self->ins.ofs,
      &self->ins.src
    );

  case CPU_ACCESS_INT_TABLE: {
    if ( (int)sizeof(self->__idt__) <= self->ins.index )
      break;

    bool is_done = cpu_read(self,
      self->ins.seg,
      self->ins.ofs,
      &self->ins.src
    );

    if ( /* /unlikely/ */ !is_done )
      return is_done;

    self->__idt__[ self->ins.index++ ] = self->ins.src;

    if ( UINT8_C(0xFF) == self->ins.ofs ) {
      ++self->ins.seg;
    } else {
      ++self->ins.ofs;
    }
  } return false; /* Continue. */

  case CPU_ACCESS_MAP_TABLE: {
    if ( (int)sizeof(self->__mdt__) <= self->ins.index )
      break;

    bool is_done = cpu_read(self,
      self->ins.seg,
      self->ins.ofs,
      &self->ins.src
    );

    if ( /* /unlikely/ */ !is_done )
      return is_done;

    self->__mdt__[ self->ins.index++ ] = self->ins.src;

    if ( UINT8_C(0xFF) == self->ins.ofs ) {
      ++self->ins.seg;
    } else {
      ++self->ins.ofs;
    }
  } return false; /* Continue. */
  }

  self->ins.access = CPU_NO_ACCESS;

  return true;
}

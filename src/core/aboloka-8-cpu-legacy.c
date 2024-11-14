# include "aboloka-8-cpu-legacy.h"
# include <stdio.h>
# include <limits.h>

# define a  regs[ CPU_LEGACY_A ]
# define x  regs[ CPU_LEGACY_X ]
# define y  regs[ CPU_LEGACY_Y ]
# define z  regs[ CPU_LEGACY_Z ]
# define cs segs[ CPU_LEGACY_CS ]

bool cpu_legacy_read_mem (
  struct aboloka_8_cpu_t * self,
  uint16_t                 addr,
  uint8_t *                data
)
{
  return aboloka_8_ram_read(self->ram, addr, data);
}

bool cpu_legacy_write_mem (
  struct aboloka_8_cpu_t * self,
  uint16_t                 addr,
  uint8_t                  data
)
{
  return aboloka_8_ram_write(self->ram, addr, data);
}

bool cpu_legacy_fetch   ( struct aboloka_8_cpu_t * self )
{
  self->ir.front = -1;
  self->ir.rear  = -1;

  uint16_t ip = ( (uint16_t)self->cs << 8 ) | self->ip;

  if ( !cpu_legacy_read_mem(self, ip, self->ir.queue + ++self->ir.rear) )
    return false;

  if ( !cpu_legacy_read_mem(self, ++ip, self->ir.queue + ++self->ir.rear) )
    return false;

  if ( !cpu_legacy_read_mem(self, ++ip, self->ir.queue + ++self->ir.rear) )
    return false;

  if ( !cpu_legacy_read_mem(self, ++ip, self->ir.queue + ++self->ir.rear) )
    return false;

  self->cs = (uint8_t)( ip >> 8 );
  self->ip = (uint8_t)( ip >> 0 );

  return true;
}

bool cpu_legacy_decode  ( struct aboloka_8_cpu_t * self )
{
  self->ins.opcode = self->ir.queue[ ++self->ir.front ];
  self->ins.index  = 0;
  self->ins.stages = INT_MAX;
  self->ins.stage  = 0;

  return true;
}

bool cpu_legacy_execute ( struct aboloka_8_cpu_t * self )
{
  int       opcode = self->ins.opcode;
  uint8_t * queue  = self->ir.queue + self->ir.front;

  switch ( opcode ) {
  case 0x10: {
    switch ( self->ins.stage ) {
    case 0: {
      self->ins.stages = 3;
      self->ins.addr =
        /* Aboloka-8 Avuxo's specifications seems to be little-endian. */
        ( (uint16_t)queue[ 1 ] << 8 ) |
        ( (uint16_t)queue[ 2 ] << 0 );
      self->ins.data = queue[ 3 ];

      ++self->stage;
    } /* /fall-through/ */
    case 1: {
      if ( !cpu_legacy_write_mem(self, self->ins.addr, self->ins.data) )
        return false;

      ++self->stage;
    } /* /fall-through/ */
    case 2: {
      self->regs[ opcode - 0x15 ] = self->ins.data;

      ++self->stage;
    } break;
    }
  } break;

  case 0x11: case 0x12: case 0x13: case 0x14: {
    self->regs[ opcode - 0x11 ] = queue[ 1 ];
  } break;

  case 0x15: case 0x16: case 0x17: case 0x18: {
    switch ( self->ins.stage ) {
    case 0: {
      self->ins.stages = 3;
      self->ins.addr =
        /* Aboloka-8 Avuxo's specifications seems to be little-endian. */
        ( (uint16_t)queue[ 1 ] << 8 ) |
        ( (uint16_t)queue[ 2 ] << 0 );

      ++self->stage;
    } /* /fall-through/ */
    case 1: {
      if ( !cpu_legacy_read_mem(self, self->ins.addr, &self->ins.data) )
        return false;

      ++self->stage;
    } /* /fall-through/ */
    case 2: {
      self->regs[ opcode - 0x15 ] = self->ins.data;

      ++self->stage;
    } break;
    }
  } break;

  case 0x20: {
jump_to_addr:

    self->ins.addr =
      /* Aboloka-8 Avuxo's specifications seems to be little-endian. */
      ( (uint16_t)queue[ 1 ] << 8 ) |
      ( (uint16_t)queue[ 2 ] << 0 );

    self->cs = (uint8_t)( self->ins.addr >> 8 );
    self->ip = (uint8_t)( self->ins.addr >> 0 );
  } break;

  case 0x21: {
jump_to_reg:

    /* How to handle 16-bit jumps using registers? */
    self->ins.addr = (uint16_t)self->regs[ queue[ 1 ] % CPU_LEGACY_N_REGS ];

    self->cs = (uint8_t)( self->ins.addr >> 8 );
    self->ip = (uint8_t)( self->ins.addr >> 0 );
  } break;

  case 0x22: {
    if ( UINT8_C(0x00) != ( self->csr & CPU_LEGACY_EF ) )
      goto jump_to_addr;
  } break;

  case 0x23: {
    if ( UINT8_C(0x00) != ( self->csr & CPU_LEGACY_EF ) )
      goto jump_to_reg;
  } break;

  case 0x25: {
    /* TODO(Andpuv) [2]: Implement subroutines. */
  } break;

  case 0x26: {
    /* TODO(Andpuv) [2]: Implement subroutines. */
  } break;

  case 0x30: {
    /* TODO(Andpuv) [1]: Update flags after computation. */
    self->a += queue[ 1 ];
  } break;

  case 0x32: {
    /* TODO(Andpuv) [1]: Update flags after computation. */
    self->a -= queue[ 1 ];
  } break;

  case 0x35: {
    /* TODO(Andpuv) [1]: Update flags after computation. */
    ++self->a;
  } break;

  case 0x37: {
    /* TODO(Andpuv) [1]: Update flags after computation. */
    --self->a;
  } break;

  case 0x31: case 0x33: case 0x34: case 0x36: {
    switch ( self->ins.stage ) {
    case 0: {
      self->ins.stages = 3;
      self->ins.addr =
        /* Aboloka-8 Avuxo's specifications seems to be little-endian. */
        ( (uint16_t)queue[ 1 ] << 8 ) |
        ( (uint16_t)queue[ 2 ] << 0 );

      ++self->ins.stage;
    } /* /fall-through/ */
    case 1: {
      if ( !cpu_legacy_read_mem(self, self->ins.addr, &self->ins.data) )
        return false;

      /* TODO(Andpuv) [1]: Update flags after computation. */
      switch ( opcode ) {
      case 0x31: {
        self->ins.data += queue[ 3 ];
      } break;

      case 0x33: {
        self->ins.data -= queue[ 3 ];
      } break;

      case 0x34: {
        ++self->ins.data;
      } break;

      case 0x36: {
        --self->ins.data;
      } break;
      }

      ++self->ins.stage;
    } /* /fall-through/ */
    case 2: {
      if ( !cpu_legacy_write_mem(self, self->ins.addr, self->ins.data) )
        return false;

      ++self->ins.stage;
    } break;
    }
  } break;

  case 0x40: {
    self->csr &= ~( CPU_LEGACY_EF | CPU_LEGACY_CF | CPU_LEGACY_OF );
  } break;

  case 0x41: {
    /* Switch to non-legacy mode. */
    self->csr &= ~CPU_LEGACY_LF;
  } break;

  case 0x55: {
    /* /discard/ */ fputc(
      (int)( (int8_t)queue[ 1 ] ),
      stdout
    );
  } break;

  case 0x46: {
    /* /discard/ */ fputc(
      (int)( (int8_t)self->regs[ queue[ 1 ] % CPU_LEGACY_N_REGS ] ),
      stdout
    );
  } break;

  case 0x57: {
    switch ( self->ins.stage ) {
    case 0: {
      self->ins.stages = 3;
      self->ins.addr =
        /* Aboloka-8 Avuxo's specifications seems to be little-endian. */
        ( (uint16_t)queue[ 1 ] << 8 ) |
        ( (uint16_t)queue[ 2 ] << 0 );

      ++self->ins.stage;
    } /* /fall-through/ */;
    case 1: {
      self->ins.addr += self->ins.index++;

      if ( !cpu_legacy_read_mem(self, self->ins.addr, &self->ins.data) )
        return false;

      ++self->ins.stage;
    } /* /fall-through/ */
    case 2: {
      if ( UINT8_C(0x00) != self->ins.data ) {
        /* /discard/ */ fputc(
          (int)( (int8_t)self->ins.data ),
          stdout
        );

        if ( UINT8_C(0x0A) == self->ins.data ) {
          /* /discard/ */ fflush(stdout);
        }

        return false;
      }
    } break;
    }
  } break;

  case 0xE9: {
    /* /do-nothing/ */
  } break;

  case 0xEA: {
    self->quit = true;
  } break;
  }

  return true;
}

bool cpu_legacy_access  ( struct aboloka_8_cpu_t * self )
{
  return true;
}

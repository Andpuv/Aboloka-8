# ifndef __ABOLOKA_8_CPU_LEGACY_H__
#   define __ABOLOKA_8_CPU_LEGACY_H__

#   include "../../inc/core/aboloka-8-cpu.h"
#   include "../../inc/core/aboloka-8-ram.h"

#   define CPU_LEGACY_X  ABOLOKA_8_CPU_AX
#   define CPU_LEGACY_Y  ABOLOKA_8_CPU_BX
#   define CPU_LEGACY_Z  ABOLOKA_8_CPU_CX
#   define CPU_LEGACY_A  ABOLOKA_8_CPU_DX
#   define CPU_LEGACY_N_REGS 0x4

#   define CPU_LEGACY_CS ABOLOKA_8_CPU_CS

#   define CPU_LEGACY_EF ABOLOKA_8_CPU_CF
#   define CPU_LEGACY_CF ABOLOKA_8_CPU_ZF
#   define CPU_LEGACY_OF ABOLOKA_8_CPU_SF
#   define CPU_LEGACY_3F ABOLOKA_8_CPU_OF
#   define CPU_LEGACY_4F ABOLOKA_8_CPU_4F
#   define CPU_LEGACY_5F ABOLOKA_8_CPU_5F
#   define CPU_LEGACY_LF ABOLOKA_8_CPU_LF
#   define CPU_LEGACY_7F ABOLOKA_8_CPU_7F

bool cpu_legacy_read_mem (
  struct aboloka_8_cpu_t * self,
  uint16_t                 addr,
  uint8_t *                data
);

bool cpu_legacy_write_mem (
  struct aboloka_8_cpu_t * self,
  uint16_t                 addr,
  uint8_t                  data
);

bool cpu_legacy_fetch   ( struct aboloka_8_cpu_t * self );
bool cpu_legacy_decode  ( struct aboloka_8_cpu_t * self );
bool cpu_legacy_execute ( struct aboloka_8_cpu_t * self );
bool cpu_legacy_access  ( struct aboloka_8_cpu_t * self );

# endif /* __ABOLOKA_8_CPU_LEGACY_H__ */

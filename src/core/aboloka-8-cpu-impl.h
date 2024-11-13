# ifndef __CORE_ABOLOKA_8_CPU_IMPL_H__
#   define __CORE_ABOLOKA_8_CPU_IMPL_H__

#   include "../../inc/core/aboloka-8-cpu.h"
#   include "../../inc/core/aboloka-8-ram.h"

#   define CPU_SEG_SIZE ABOLOKA_8_CPU_SEG_SIZE

#   define CPU_IRQ_0  ABOLOKA_8_CPU_IRQ_0
#   define CPU_IRQ_1  ABOLOKA_8_CPU_IRQ_1
#   define CPU_IRQ_2  ABOLOKA_8_CPU_IRQ_2
#   define CPU_IRQ_3  ABOLOKA_8_CPU_IRQ_3
#   define CPU_IRQ_4  ABOLOKA_8_CPU_IRQ_4
#   define CPU_IRQ_5  ABOLOKA_8_CPU_IRQ_5
#   define CPU_IRQ_6  ABOLOKA_8_CPU_IRQ_6
#   define CPU_IRQ_7  ABOLOKA_8_CPU_IRQ_7
#   define CPU_N_IRQS ABOLOKA_8_CPU_N_IRQS

#   define CPU_REG_0  ABOLOKA_8_CPU_REG_0
#   define CPU_REG_1  ABOLOKA_8_CPU_REG_1
#   define CPU_REG_2  ABOLOKA_8_CPU_REG_2
#   define CPU_REG_3  ABOLOKA_8_CPU_REG_3
#   define CPU_REG_4  ABOLOKA_8_CPU_REG_4
#   define CPU_REG_5  ABOLOKA_8_CPU_REG_5
#   define CPU_REG_6  ABOLOKA_8_CPU_REG_6
#   define CPU_REG_7  ABOLOKA_8_CPU_REG_7
#   define CPU_N_REGS ABOLOKA_8_CPU_N_REGS

#   define CPU_A      ABOLOKA_8_CPU_A
#   define CPU_X      ABOLOKA_8_CPU_X
#   define CPU_Y      ABOLOKA_8_CPU_Y
#   define CPU_Z      ABOLOKA_8_CPU_Z

#   define CPU_SEG_0  ABOLOKA_8_CPU_SEG_0
#   define CPU_SEG_1  ABOLOKA_8_CPU_SEG_1
#   define CPU_SEG_2  ABOLOKA_8_CPU_SEG_2
#   define CPU_SEG_3  ABOLOKA_8_CPU_SEG_3
#   define CPU_SEG_4  ABOLOKA_8_CPU_SEG_4
#   define CPU_SEG_5  ABOLOKA_8_CPU_SEG_5
#   define CPU_SEG_6  ABOLOKA_8_CPU_SEG_6
#   define CPU_SEG_7  ABOLOKA_8_CPU_SEG_7
#   define CPU_N_SEGS ABOLOKA_8_CPU_N_SEGS

#   define CPU_CS     ABOLOKA_8_CPU_CS
#   define CPU_DS     ABOLOKA_8_CPU_DS
#   define CPU_SS     ABOLOKA_8_CPU_SS
#   define CPU_ES     ABOLOKA_8_CPU_ES

#   define CPU_N_INTS ABOLOKA_8_CPU_N_INTS
#   define CPU_N_MAPS ABOLOKA_8_CPU_N_MAPS

#   define CPU_CF     ABOLOKA_8_CPU_CF
#   define CPU_1F     ABOLOKA_8_CPU_1F
#   define CPU_ZF     ABOLOKA_8_CPU_ZF
#   define CPU_SF     ABOLOKA_8_CPU_SF
#   define CPU_OF     ABOLOKA_8_CPU_OF
#   define CPU_IF     ABOLOKA_8_CPU_IF
#   define CPU_UF     ABOLOKA_8_CPU_UF
#   define CPU_TF     ABOLOKA_8_CPU_TF

void cpu_update_flags (
  struct aboloka_8_cpu_t * self,
  bool                     cf,
  bool                     zf,
  bool                     sf,
  bool                     of
);

void cpu_interruption (
  struct aboloka_8_cpu_t * self,
  uint8_t                  int_id,
  uint8_t                  cause
);

#   define CPU_EXCEPTION_CAUSE_WU 0 /* Wake up               */
#   define CPU_EXCEPTION_CAUSE_MC 1 /* Machine Check         */
#   define CPU_EXCEPTION_CAUSE_BP 2 /* BreakPoint            */
#   define CPU_EXCEPTION_CAUSE_SS 3 /* Single Step execution */
#   define CPU_EXCEPTION_CAUSE_UD 4 /* UnDefined instruction */
#   define CPU_EXCEPTION_CAUSE_SF 5 /* Segmentation Fault    */
#   define CPU_EXCEPTION_CAUSE_BE 6 /* Bus Error             */
#   define CPU_EXCEPTION_CAUSE_KP 7 /* Kernel Protection     */

void cpu_exception (
  struct aboloka_8_cpu_t * self,
  uint8_t                  cause
);

bool cpu_read (
  struct aboloka_8_cpu_t * self,
  uint8_t                  seg,
  uint8_t                  ofs,
  uint8_t *                val
);

bool cpu_write (
  struct aboloka_8_cpu_t * self,
  uint8_t                  seg,
  uint8_t                  ofs,
  uint8_t                  val
);

#   define CPU_STAGE_IDLE       -1
#   define CPU_STAGE_FETCH       0
#   define CPU_STAGE_DECODE      1
#   define CPU_STAGE_ACCESS_0    2
#   define CPU_STAGE_EXECUTE     3
#   define CPU_STAGE_ACCESS_1    4
#   define CPU_STAGE_REFRESH     5

#   define CPU_NO_ACCESS        -1
#   define CPU_ACCESS_REG_WRITE  0
#   define CPU_ACCESS_REG_READ   1
#   define CPU_ACCESS_SEG_WRITE  2
#   define CPU_ACCESS_SEG_READ   3
#   define CPU_ACCESS_MEM_WRITE  4
#   define CPU_ACCESS_MEM_READ   5
#   define CPU_ACCESS_INT_TABLE  6
#   define CPU_ACCESS_MAP_TABLE  7

bool cpu_fetch   ( struct aboloka_8_cpu_t * self );
bool cpu_decode  ( struct aboloka_8_cpu_t * self );
bool cpu_execute ( struct aboloka_8_cpu_t * self );
bool cpu_access  ( struct aboloka_8_cpu_t * self );

# endif /* __CORE_ABOLOKA_8_CPU_IMPL_H__ */

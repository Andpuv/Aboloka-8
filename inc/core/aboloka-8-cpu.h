# ifndef __CORE_ABOLOKA_8_CPU_H__
#   define __CORE_ABOLOKA_8_CPU_H__

#   include <stdint.h>
#   include <stddef.h>
#   include <stdbool.h>

#   include "aboloka-8-api.h"

struct aboloka_8_ram_t;

#   define ABOLOKA_8_CPU_SEG_SIZE_LOG2 8
#   define ABOLOKA_8_CPU_SEG_SIZE                                              \
  (                                                                            \
    (size_t)1 << ABOLOKA_8_CPU_SEG_SIZE_LOG2                                   \
  )

#   define ABOLOKA_8_CPU_IRQ_0  0x0
#   define ABOLOKA_8_CPU_IRQ_1  0x1
#   define ABOLOKA_8_CPU_IRQ_2  0x2
#   define ABOLOKA_8_CPU_IRQ_3  0x3
#   define ABOLOKA_8_CPU_IRQ_4  0x4
#   define ABOLOKA_8_CPU_IRQ_5  0x5
#   define ABOLOKA_8_CPU_IRQ_6  0x6
#   define ABOLOKA_8_CPU_IRQ_7  0x7
#   define ABOLOKA_8_CPU_N_IRQS 0x8

/* This interrupt is used to wake up the CPU. */
#   define ABOLOKA_8_CPU_WAKE_UP ABOLOKA_8_CPU_IRQ_0

#   define ABOLOKA_8_CPU_REG_0  0x0
#   define ABOLOKA_8_CPU_REG_1  0x1
#   define ABOLOKA_8_CPU_REG_2  0x2
#   define ABOLOKA_8_CPU_REG_3  0x3
#   define ABOLOKA_8_CPU_REG_4  0x4
#   define ABOLOKA_8_CPU_REG_5  0x5
#   define ABOLOKA_8_CPU_REG_6  0x6
#   define ABOLOKA_8_CPU_REG_7  0x7
#   define ABOLOKA_8_CPU_N_REGS 0x8

#   define ABOLOKA_8_CPU_A      0x0
#   define ABOLOKA_8_CPU_X      0x1
#   define ABOLOKA_8_CPU_Y      0x2
#   define ABOLOKA_8_CPU_Z      0x3

#   define ABOLOKA_8_CPU_SEG_0  0x0
#   define ABOLOKA_8_CPU_SEG_1  0x1
#   define ABOLOKA_8_CPU_SEG_2  0x2
#   define ABOLOKA_8_CPU_SEG_3  0x3
#   define ABOLOKA_8_CPU_SEG_4  0x4
#   define ABOLOKA_8_CPU_SEG_5  0x5
#   define ABOLOKA_8_CPU_SEG_6  0x6
#   define ABOLOKA_8_CPU_SEG_7  0x7
#   define ABOLOKA_8_CPU_N_SEGS 0x8

#   define ABOLOKA_8_CPU_CS     0x0 /* Code Segment            */
#   define ABOLOKA_8_CPU_DS     0x1 /* Data Segment            */
#   define ABOLOKA_8_CPU_SS     0x2 /* Stack Segment           */
#   define ABOLOKA_8_CPU_ES     0x3 /* Extra data/code Segment */

#   define ABOLOKA_8_CPU_N_INTS 0x80
#   define ABOLOKA_8_CPU_N_MAPS 0x80

#   define ABOLOKA_8_CPU_CF UINT8_C(0x01)
#   define ABOLOKA_8_CPU_1F UINT8_C(0x02)
#   define ABOLOKA_8_CPU_ZF UINT8_C(0x04)
#   define ABOLOKA_8_CPU_SF UINT8_C(0x08)
#   define ABOLOKA_8_CPU_OF UINT8_C(0x10)
#   define ABOLOKA_8_CPU_IF UINT8_C(0x20)
#   define ABOLOKA_8_CPU_UF UINT8_C(0x40)
#   define ABOLOKA_8_CPU_7F UINT8_C(0x80)

struct aboloka_8_ir_t {
  uint8_t queue [ 0x4 ];
  int     front;
  int     rear;
};

struct aboloka_8_ins_t {
  uint16_t pc;
  int      ip;
  int      opcode;
  int      dst_id;
  int      src_id;
  uint8_t  dst;
  uint8_t  src;
  uint8_t  seg;
  uint8_t  ofs;
  int      stage;
  int      stages;
};

struct aboloka_8_cpu_t {
  struct aboloka_8_ram_t * ram;
  bool                     __owns_ram__;
  bool                     quit;
  int                      stage;
  int                      target_stage;
  uint16_t                 cycles;
  struct aboloka_8_ins_t   ins;
  struct aboloka_8_ir_t    ir;  /* Intruction Register          */
  uint8_t                  pc;  /* Program Counter              */
  uint8_t                  csr; /* Control and Status Register  */
  uint8_t                  mrr; /* Memory Refresh Register      */
  uint8_t                  mcr; /* Memory Counter Register      */
  uint8_t                  imr; /* Interrupt Masks Register     */
  uint8_t                  irr; /* Interrupt Requestes Register */
  uint16_t                 idt; /* Interrupt Descriptors Table  */
  uint16_t                 mdt; /* Memory Descriptors Table     */
  uint8_t                  sp;  /* Stack top Pointer            */
  uint8_t                  regs [ ABOLOKA_8_CPU_N_REGS ];
  uint8_t                  segs [ ABOLOKA_8_CPU_N_SEGS ];
  uint16_t                 __idt__ [ ABOLOKA_8_CPU_N_INTS ];
  uint16_t                 __mdt__ [ ABOLOKA_8_CPU_N_MAPS ];
};

__ABOLOKA_8_CORE_API__
struct aboloka_8_cpu_t * aboloka_8_cpu_construct (
  struct aboloka_8_ram_t * ram
);

__ABOLOKA_8_CORE_API__
void aboloka_8_cpu_deconstruct (
  struct aboloka_8_cpu_t * self
);

__ABOLOKA_8_CORE_API__
bool aboloka_8_cpu_initialize (
  struct aboloka_8_cpu_t * self,
  struct aboloka_8_ram_t * ram
);

__ABOLOKA_8_CORE_API__
void aboloka_8_cpu_deinitialize (
  struct aboloka_8_cpu_t * self
);

__ABOLOKA_8_CORE_API__
uint8_t * aboloka_8_cpu_reg (
  struct aboloka_8_cpu_t * self,
  int                      reg_id
);

__ABOLOKA_8_CORE_API__
uint8_t * aboloka_8_cpu_seg (
  struct aboloka_8_cpu_t * self,
  int                      seg_id
);

__ABOLOKA_8_CORE_API__
bool aboloka_8_cpu_intr (
  struct aboloka_8_cpu_t * self,
  int                      int_id
);

__ABOLOKA_8_CORE_API__
bool aboloka_8_cpu_inta (
  struct aboloka_8_cpu_t * self,
  int                      int_id
);

__ABOLOKA_8_CORE_API__
bool aboloka_8_cpu_halt (
  struct aboloka_8_cpu_t * self,
  bool                     is_halt
);

__ABOLOKA_8_CORE_API__
bool aboloka_8_cpu_cycle (
  struct aboloka_8_cpu_t * self
);

# endif /* __CORE_ABOLOKA_8_CPU_H__ */
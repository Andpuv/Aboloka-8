# ifndef __CORE_ABOLOKA_8_CPU_IMPL_H__
#   define __CORE_ABOLOKA_8_CPU_IMPL_H__

#   include "../../inc/core/aboloka-8-cpu.h"
#   include "../../inc/core/aboloka-8-ram.h"

bool cpu_fetch   ( struct aboloka_8_cpu_t * self );
bool cpu_decode  ( struct aboloka_8_cpu_t * self );
bool cpu_execute ( struct aboloka_8_cpu_t * self );
bool cpu_access  ( struct aboloka_8_cpu_t * self );

# endif /* __CORE_ABOLOKA_8_CPU_IMPL_H__ */
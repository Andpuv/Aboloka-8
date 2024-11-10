# include "main.h"

int main ( int argc, char ** argv )
{
  fprintf(stdout, "Hello World!\n");

  struct aboloka_8_cpu_t * cpu = aboloka_8_cpu_construct(
    (struct aboloka_8_ram_t *)NULL
  );

  if ( /* /unlikely/ */ !cpu )
    return EXIT_FAILURE;

  unsigned int elapsed_cycles;
  unsigned int cycles = 0U;
  bool is_halted = false;
  bool is_done;

  while ( !cpu->quit ) {
    fprintf(stderr, "Cycle %u/%u:\n", (unsigned int)cpu->cycles, cycles);

    if ( 0U == ( cycles % 20U ) ) {
      fprintf(stderr, "... Trying to HALT the CPU...");

      is_done = aboloka_8_cpu_halt(cpu, true);

      fprintf(stderr, "\r... Trying to HALT the CPU... %s\n",
        is_done ? "Halted" : "Running"
      );

      if ( is_done ) {
        elapsed_cycles = 0;
        is_halted      = true;
      }
    } else if ( is_halted && 0U == ( elapsed_cycles % 10U ) ) {
      fprintf(stderr, "... Trying to RUN the CPU...");

      is_done = aboloka_8_cpu_halt(cpu, false);

      fprintf(stderr, "\r... Trying to RUN the CPU... %s\n",
        is_done ? "Running" : "Halted"
      );

      if ( is_done ) {
        elapsed_cycles = 0;
        is_halted      = false;
      }
    }

    aboloka_8_cpu_cycle(cpu);
    ++cycles;
    ++elapsed_cycles;

    getchar();
  }

  aboloka_8_cpu_deconstruct(cpu);

  return EXIT_SUCCESS;
}
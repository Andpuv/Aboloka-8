# include "main.h"
# include <stdio.h>
# include <time.h>
# include <stdlib.h>
# include <string.h>

int main ( int argc, char ** argv )
{
  srand(time(NULL));

  struct aboloka_8_cpu_t * cpu = aboloka_8_cpu_construct(
    (struct aboloka_8_ram_t *)NULL
  );

  if ( /* /unlikely/ */ !cpu )
    return EXIT_FAILURE;

/*/
  unsigned int elapsed_cycles;
  unsigned int cycles = 0U;
  bool is_halted;
  bool is_done;
*/
  cpu->mrr = 22;

  uint8_t test_0 [] = {
    0x55,  'A', 0x00, 0x00, /* 0x00:00 | emit 'A'       */
    0x55,  'B', 0x00, 0x00, /* 0x00:04 | emit 'B'       */
    0x55,  'O', 0x00, 0x00, /* 0x00:08 | emit 'O'       */
    0x55,  'L', 0x00, 0x00, /* 0x00:0C | emit 'L'       */
    0x55,  'O', 0x00, 0x00, /* 0x00:10 | emit 'O'       */
    0x55,  'K', 0x00, 0x00, /* 0x00:14 | emit 'K'       */
    0x55,  'A', 0x00, 0x00, /* 0x00:18 | emit 'A'       */
    0x55,  '-', 0x00, 0x00, /* 0x00:1C | emit '-'       */
    0x55,  '8', 0x00, 0x00, /* 0x00:20 | emit '8'       */
    0x55, '\n', 0x00, 0x00, /* 0x00:24 | emit '\n'      */
    0x14, 0x00, 0x00, 0x00, /* 0x00:28 | lda #$0        */
    0x30, 0x01, 0x00, 0x00, /* 0x00:2C | here: add #$1  */
    0x20, 0x00, 0x2C, 0x00, /* 0x00:30 | jmp here       */
    0xEA, 0x00, 0x00, 0x00, /* 0x00:34 | exit           */
  };
  uint8_t test_1 [] = {
    0x55,  'A', 0x00, 0x00, /* 0x00:00 | emit 'A'       */
    0x55,  'B', 0x00, 0x00, /* 0x00:04 | emit 'B'       */
    0x55,  'O', 0x00, 0x00, /* 0x00:08 | emit 'O'       */
    0x55,  'L', 0x00, 0x00, /* 0x00:0C | emit 'L'       */
    0x55,  'O', 0x00, 0x00, /* 0x00:10 | emit 'O'       */
    0x55,  'K', 0x00, 0x00, /* 0x00:14 | emit 'K'       */
    0x55,  'A', 0x00, 0x00, /* 0x00:18 | emit 'A'       */
    0x55,  '-', 0x00, 0x00, /* 0x00:1C | emit '-'       */
    0x55,  '8', 0x00, 0x00, /* 0x00:20 | emit '8'       */
    0x55, '\n', 0x00, 0x00, /* 0x00:24 | emit '\n'      */
    0x14, 0x00, 0x00, 0x00, /* 0x00:28 | lda #$0        */
    0x30, 0x01, 0x00, 0x00, /* 0x00:2C | here: add #$1  */
    0x10, 0xFF, 0xFF,  '.', /* 0x00:30 | ld $FFFF, '.'  */
    0x15, 0xFF, 0xFF, 0x00, /* 0x00:34 | ldx $FFFF      */
    0x46, 0x00, 0x00, 0x00, /* 0x00:38 | emit x         */
    0x20, 0x00, 0x2C, 0x00, /* 0x00:3C | jmp here       */
    0xEA, 0x00, 0x00, 0x00, /* 0x00:40 | exit           */
  };

  uint8_t * test_data = test_0;
  size_t    test_size = sizeof(test_0);

  for ( int argi = 0; argi < argc; ++argi ) {
    char * args = argv[ argi ];

    if ( 0 == strcmp(args, "--test-0") ) {
      test_data = test_0;
      test_size = sizeof(test_0);
    } else if ( 0 == strcmp(args, "--test-1") ) {
      test_data = test_1;
      test_size = sizeof(test_1);
    }
  }

  for ( int idx = 0; idx < (int)test_size; ++idx ) {
    uint16_t addr = (uint16_t)( (unsigned int)idx );
    uint8_t  data = test_data[ idx ];

    if ( aboloka_8_ram_write(cpu->ram, addr, data) )
      continue;

    --idx; /* Try again. */
  }

  clock_t start, end;

  start = clock();
  while ( !cpu->quit ) {
/*
    is_halted = aboloka_8_cpu_is_halted(cpu);

    fprintf(stderr, ">>> Cycle %u/%u:\n", (unsigned int)cpu->cycles, cycles);
    fprintf(stderr, "--- CS:IP = %02X:%02X (STAGE = %d).\n",
      (unsigned int)cpu->segs[ 0 ],
      (unsigned int)cpu->ip,
      cpu->stage
    );

    if ( 0U == ( cycles % 20U ) ) {
      fprintf(stderr, "--- Trying to HALT the CPU...");

      is_done = aboloka_8_cpu_halt(cpu, true);

      fprintf(stderr, "\r--- Trying to HALT the CPU... %s\n",
        is_done ? "Halted" : "Running"
      );

      if ( is_done ) {
        elapsed_cycles = 0;
        is_halted      = true;
      }
    } else if ( is_halted && 0U == ( elapsed_cycles % 10U ) ) {
      fprintf(stderr, "--- Trying to RUN the CPU...");

      is_done = aboloka_8_cpu_halt(cpu, false);

      fprintf(stderr, "\r--- Trying to RUN the CPU... %s\n",
        is_done ? "Running" : "Halted"
      );

      if ( is_done ) {
        elapsed_cycles = 0;
        is_halted      = false;
      }
    }
*/

    aboloka_8_cpu_cycle(cpu);
    cpu->quit = 100000000 <= cpu->cycles || cpu->regs[ 0x3 ] == 0xFF;
/*
    ++cycles;
    ++elapsed_cycles;

    int ch = getchar();

    if ( ch == 'q' || ch == 'Q' ) {
      fprintf(stderr, "--- Quitted.\n");

      cpu->quit = true;
    } else if ( ch == 'w' || ch == 'W' ) {
      if ( is_halted ) {
        elapsed_cycles = 0;
        is_halted      = false;

        fprintf(stderr, "--- Trying to WAKE UP the CPU...");

        is_done = aboloka_8_cpu_intr(cpu, ABOLOKA_8_CPU_IRQ_0);

        fprintf(stderr, "\r--- Trying to WAKE UP the CPU... %s\n",
          is_done ? "Waking up" : "Sleeping"
        );
      } else {
        fprintf(stderr, "--- CPU is already running.\n");
      }
    }
*/
  }
  end = clock();
  double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
  double estimated_frequency = (double)cpu->cycles / cpu_time_used;

  fflush(stdout);
  fprintf(stderr, "ACC = 0x%08X\n", cpu->regs[ 0x3 ]);
  fprintf(stderr, "Total Cycles: %u\n", cpu->cycles);
  fprintf(stderr, "Execution Time: %f seconds\n", cpu_time_used);
  fprintf(stderr, "Estimated CPU Frequency: %f Hz\n", estimated_frequency);

  aboloka_8_cpu_deconstruct(cpu);

  return EXIT_SUCCESS;
}

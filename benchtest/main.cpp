#include "benchmark/benchmark.h"
#include "Table.h"


static void BM_TableWrite(benchmark::State& state) {
  const int table_size = 16 * 16;
    Table table{1, 1};
    for (auto _ : state) {
      for (int i = 0; i < table_size; i++)
      {
          table.writeInt(i, 0, i);
          table.writeDouble(i, 1, i + 0.1);
      }
    }
}
// Register the function as a benchmark
BENCHMARK(BM_TableWrite);

static void BM_TableRead(benchmark::State& state) {
  const int table_size = 16 * 16;
    Table table{1, 1};
    for (int i = 0; i < table_size; i++)
    {
        table.writeInt(i, 0, i);
        table.writeDouble(i, 1, i + 0.1);
    }
    for (auto _ : state) {
      for (int i = 0; i < table_size; i++) {
        table.readInt(i, 0);
        table.readDouble(i, 1);
      }
    }
}

BENCHMARK(BM_TableRead);
BENCHMARK_MAIN();
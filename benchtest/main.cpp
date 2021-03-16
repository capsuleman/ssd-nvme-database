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

static void BM_TableSearch(benchmark::State& state) {
  const int table_size = 16 * 16;
    Table table{3, 2};
    for (int i = 0; i < table_size; i++)
    {
        table.writeInt(i, 0, 1);
        table.writeInt(i, 1, i);
        table.writeInt(i, 2, 2 * i);
        table.writeDouble(i, 3, i + 0.1);
        table.writeDouble(i, 4, i * 1.0);
    }
    for (auto _ : state) {
      int attribute_predicates[3] = {1, 2, 4};
      double value_predicates[2] = {2.1, 2.0};
      auto result = table.findRows(attribute_predicates, value_predicates);
    };
}

BENCHMARK(BM_TableWrite);
BENCHMARK(BM_TableRead);
BENCHMARK(BM_TableSearch);
BENCHMARK_MAIN();

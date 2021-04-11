#include <chrono>
#include <iostream>

#include "benchmark/benchmark.h"
#include "Table.hpp"

static void BM_TableWrite(benchmark::State &state)
{
    const int table_size = 16 * 16;
    Table table{1, 1};
    for (auto _ : state)
    {
        for (int i = 0; i < table_size; i++)
        {
            table.writeInt(i, 0, i);
            table.writeDouble(i, 1, i + 0.1);
        }
    }
}

static void BM_TableWriteMultiple(benchmark::State &state)
{
    const int table_size = 16 * 16;
    Table table{1, 1};
    unsigned int attributes[table_size];
    double values[table_size];

    for (int i = 0; i < table_size; i++)
    {
        attributes[i] = i;
        values[i] = 0.1 * i;
    }
    for (auto _ : state)
    {
        table.writeRows(0, table_size, attributes, values);
    }
}

static void BM_TableRead(benchmark::State &state)
{
    const int table_size = 16 * 16;
    Table table{1, 0};
    unsigned int attributes[table_size];
    for (int i = 0; i < table_size; i++)
    {
        attributes[i] = i;
    }
    table.writeRows(0, table_size, attributes, nullptr);

    unsigned int i = 0;
    for (auto _ : state)
    {
        table.readInt(i, 0);
        i = (i + 1) % table_size;
    }
}

static void BM_TableSearch(benchmark::State &state)
{
    const int table_size = 16 * 16;
    Table table{1, 0};
    unsigned int attributes[table_size];
    for (int i = 0; i < table_size; i++)
    {
        attributes[i] = i;
    }
    table.writeRows(0, table_size, attributes, nullptr);

    for (auto _ : state)
    {
        int attribute_predicates[1] = {2};
        auto result = table.findRows(attribute_predicates, nullptr);
    };
}

// Test load / comparison / unload
static void BM_ChunkLoad(benchmark::State &state)
{
    const int column_size = CHUNK_SIZE;
    ChunkAllocator chunk_allocator;
    Chunk chunk{chunk_allocator.getChunk(false)};

    unsigned int attributes[column_size];
    for (int i = 0; i < column_size; i++)
    {
        attributes[i] = i;
    }
    chunk.writeInts(0, column_size, attributes);

    for (auto _ : state)
    {
        auto start = std::chrono::high_resolution_clock::now();
        // Just counting the loading
        chunk.load();
        auto end = std::chrono::high_resolution_clock::now();
        chunk.unload();

        auto elapsed_seconds =
            std::chrono::duration_cast<std::chrono::duration<double>>(
                end - start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void BM_ChunkComparison(benchmark::State &state)
{
    const int column_size = CHUNK_SIZE;
    ChunkAllocator chunk_allocator;
    Chunk chunk{chunk_allocator.getChunk(false)};

    unsigned int attributes[column_size];
    for (int i = 0; i < column_size; i++)
    {
        attributes[i] = i;
    }
    chunk.writeInts(0, column_size, attributes);

    for (auto _ : state)
    {
        chunk.load();
        auto start = std::chrono::high_resolution_clock::now();
        // Just counting the comparison
        chunk.findInt(1);
        auto end = std::chrono::high_resolution_clock::now();
        chunk.unload();

        auto elapsed_seconds =
            std::chrono::duration_cast<std::chrono::duration<double>>(
                end - start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}

static void BM_BitsetTest(benchmark::State &state)
{
    const int column_size = CHUNK_SIZE;
    unsigned int attributes[column_size];
    std::bitset<column_size> results;
    for (int i = 0; i < column_size; i++)
    {
        attributes[i] = i;
    }

    for (auto _ : state)
    {
        for (int i = 0; i < column_size; i++)
        {
            results[i] = (attributes[i] == 1);
        }
    }
    if (results.count() != 1)
    {
        std::cout << "should not be printed" << std::endl;
    }
}

BENCHMARK(BM_TableWrite);
BENCHMARK(BM_TableWriteMultiple);
BENCHMARK(BM_TableRead);
BENCHMARK(BM_TableSearch);
BENCHMARK(BM_ChunkLoad)->UseManualTime();
BENCHMARK(BM_ChunkComparison)->UseManualTime();
BENCHMARK(BM_BitsetTest);
BENCHMARK_MAIN();

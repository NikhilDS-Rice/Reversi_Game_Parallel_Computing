/*
 * reducer-opor-demo.cpp
 *
 * This example demonstrates using an opor reducer to compute a bitmask in
 * parallel.  It initializes the result to 0, and then sets every "Kth" bit,
 * both in a serial loop and in parallel.  The results are displayed and
 * compared.
 */

#include <cstdio>
#include <cstdlib>
#include <cilk/cilk.h>
#include <cilk/reducer_opor.h>

// This test starts with a 64-bit mask of all 1's, and
// sets bits 0, K, 2K, etc. to 0. 
void reducer_opor_test(unsigned K)
{
    // Set the bits that are "K" bits apart to 1.
    uint64_t serial_ans = 0;
    for (int i = 0; i < 64; i+= K)
    {
        uint64_t i_mask = (1ULL << i);
        serial_ans |= i_mask;
    }
    printf("Serial result:   0x%llX\n", serial_ans);

    // Do the same operation in parallel, in a cilk_for loop.
    cilk::reducer_opor<uint64_t> parallel_ans(0);
    cilk_for(int i = 0; i < 64; i+= K)
    {
        uint64_t i_mask = (1ULL << i);
        parallel_ans |= i_mask;
    }
    printf("Parallel result: 0x%llX\n", parallel_ans.get_value());

    assert(serial_ans == parallel_ans.get_value());
}

int main(int argc, char* argv[])
{
    // Default argument for spacing.
    int spacing = 4;

    // Read in spacing from command line, if one is provided.
    if (argc > 1)
        spacing = atoi(argv[1]);

    printf("Testing with spacing= %d\n", spacing);
    reducer_opor_test(spacing);
    return 0;
}

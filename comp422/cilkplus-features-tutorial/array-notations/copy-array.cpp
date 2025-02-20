/*
 * copy-array.cpp
 *
 * Demonstrate copyng all elements of an array from another array using both
 * for loop and Array Notation
 */

#include <stdio.h>
#include <string.h>

const int array_size=10;

int main(int argc, char **argv)
{
    int a[array_size], b[array_size], c[array_size];

    // Initialize array a using for loop
    for (int i = 0; i < array_size; i++)
        a[i] = i;

    // Copy data using a for loop.  We could also use memcpy
    for (int i = 0; i < array_size; i++)
        b[i] = a[i];

    // Copy the array using Array Notation.  Since the array is statically
    // allocated, we can use default values for the start index (0) and
    // number of elements (all of them).
    c[:] = a[:];

    // Verify the results - The arrays should be identical
    if (0 == memcmp(b, c, sizeof(b)))
        printf("Success\n");
    else
        printf("Failed\n");

    return 0;
}

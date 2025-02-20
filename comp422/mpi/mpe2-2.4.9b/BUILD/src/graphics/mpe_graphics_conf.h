/* mpe_graphics_conf.h.  Generated from mpe_graphics_conf.h.in by configure.  */
/* mpe_graphics_conf.h.in.  Generated from configure.ac by autoheader.  */

/* Define MPE_CONST to const */
#define HAVE_CONST_IN_MPI_DECL 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define if MPI_Comm_f2c and c2f available */
#define HAVE_MPI_COMM_F2C 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `system' function. */
#define HAVE_SYSTEM 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define MPI_Fint as int */
/* #undef MPI_Fint */

/* Define to 1 if your C compiler doesn't accept -c and -o together. */
/* #undef NO_MINUS_C_MINUS_O */

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "wgropp@illinois.edu"

/* Define to the full name of this package. */
#define PACKAGE_NAME "mpe2"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "mpe2 2.4.9b"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "mpe2"

/* Define to the home page for this package. */
#define PACKAGE_URL "http://www.mcs.anl.gov/research/projects/perfvis/software/MPE/"

/* Define to the version of this package. */
#define PACKAGE_VERSION "2.4.9b"

/* Define if pointers are 64 bits */
#define POINTER_64_BITS 1

/* The size of `void *', as computed by sizeof. */
#define SIZEOF_VOID_P 8

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */


/* Define MPE_CONST=const if mpi.h uses const in MPI function declaration. */
#if defined(HAVE_CONST_IN_MPI_DECL) || MPI_VERSION >= 3
#define MPE_CONST const
#else
#define MPE_CONST
#endif


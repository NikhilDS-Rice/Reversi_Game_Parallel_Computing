/* mpe_wrappers_conf.h.  Generated from mpe_wrappers_conf.h.in by configure.  */
/* mpe_wrappers_conf.h.in.  Generated from configure.ac by autoheader.  */

/* Define to one of `_getb67', `GETB67', `getb67' for Cray-2 and Cray-YMP
   systems. This function is required for `alloca.c' support on those systems.
   */
/* #undef CRAY_STACKSEG_END */

/* Define to 1 if using `alloca.c'. */
/* #undef C_ALLOCA */

/* Define to 1 if you have `alloca', as a function or macro. */
#define HAVE_ALLOCA 1

/* Define to 1 if you have <alloca.h> and it should be used (not on Ultrix).
   */
#define HAVE_ALLOCA_H 1

/* Define is removed MPI-1 routines have const in the declarations */
/* #undef HAVE_CONST_IN_MPI1_DECL */

/* Define MPE_CONST to const */
#define HAVE_CONST_IN_MPI_DECL 1

/* Define if CRAY's FCD logical is found */
/* #undef HAVE_CRAY_FCD_LOGICAL */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define if mpir_iargc() is available. */
/* #undef HAVE_MPIR_GETARG */

/* Define if mpir_iargc() is available. */
/* #undef HAVE_MPIR_IARGC */

/* Define to 1 if you have the `MPI_Address' function. */
#define HAVE_MPI_ADDRESS 1

/* Define if MPI_Comm_f2c and c2f available */
#define HAVE_MPI_COMM_F2C 1

/* Define if MPI_Errhandler_f2c and c2f available */
#define HAVE_MPI_ERRHANDLER_F2C 1

/* Define if MPI_F_STATUSES_IGNORE available */
#define HAVE_MPI_F_STATUSES_IGNORE 1

/* Define if MPI_F_STATUS_IGNORE available */
#define HAVE_MPI_F_STATUS_IGNORE 1

/* Define if MPI_Group_f2c and c2f available */
#define HAVE_MPI_GROUP_F2C 1

/* Define if MPI_Init_thread available */
#define HAVE_MPI_INIT_THREAD 1

/* Define if MPI-IO available */
#define HAVE_MPI_IO 1

/* Define if MPI Naming Service is available */
#define HAVE_MPI_NAMING 1

/* Define if MPI_Op_f2c and c2f available */
#define HAVE_MPI_OP_F2C 1

/* Define if MPI_Request_f2c and c2f available */
#define HAVE_MPI_REQUEST_F2C 1

/* Define if MPI-RMA is available */
#define HAVE_MPI_RMA 1

/* Define if MPI_Win_lock/unlock are available */
#define HAVE_MPI_RMA_LOCK 1

/* Define if MPI_Win_test is available */
#define HAVE_MPI_RMA_TEST 1

/* Define if MPI-SPAWN is available */
#define HAVE_MPI_SPAWN 1

/* Define if MPI_STATUSES_IGNORE available */
#define HAVE_MPI_STATUSES_IGNORE 1

/* Define if MPI_PROCNULL not handled in MPI_Status */
/* #undef HAVE_MPI_STATUS_BROKEN_ON_PROC_NULL */

/* Define if MPI_Status_f2c and c2f available */
#define HAVE_MPI_STATUS_F2C 1

/* Define if MPI_STATUS_IGNORE available */
#define HAVE_MPI_STATUS_IGNORE 1

/* Define if MPI_Type_F2c and c2f available */
#define HAVE_MPI_TYPE_F2C 1

/* Define to 1 if you have the `MPI_Type_hindexed' function. */
#define HAVE_MPI_TYPE_HINDEXED 1

/* Define to 1 if you have the `MPI_Type_struct' function. */
#define HAVE_MPI_TYPE_STRUCT 1

/* Define if no MPIO_Request */
/* #undef HAVE_NO_MPIO_REQUEST */

/* Define if the compiler supports simple prototyptes */
#define HAVE_PROTOTYPES 1

/* Define if stdarg.h is available */
#define HAVE_STDARG_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdio.h> header file. */
#define HAVE_STDIO_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define if safe PMPI calls are made */
#define MAKE_SAFE_PMPI_CALL 1

/* Define as the value for Fortran logical false */
/* #undef MPE_F77_FALSE_VALUE */

/* Define as the value for Fortran logical true */
/* #undef MPE_F77_TRUE_VALUE */

/* Define MPI_Fint as int */
/* #undef MPI_Fint */

/* Define as the size of MPI_STATUS_SIZE */
/* #undef MPI_STATUS_SIZE */

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

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at runtime.
	STACK_DIRECTION > 0 => grows toward higher addresses
	STACK_DIRECTION < 0 => grows toward lower addresses
	STACK_DIRECTION = 0 => direction of growth unknown */
/* #undef STACK_DIRECTION */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define is stdarg can be used */
#define USE_STDARG 1

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */


/* Define MPE_CONST=const if mpi.h uses const in MPI function declaration. */
#if defined(HAVE_CONST_IN_MPI_DECL) || MPI_VERSION >= 3
#define MPE_CONST const
#if defined(HAVE_CONST_IN_MPI1_DECL)
/* Define MPE_CONST1 if the removed MPI-1 functions incorrectly have const */
#define MPE_CONST1 const
#else
#define MPE_CONST1
#endif /* HAVE_CONST_IN_MPI1_DECL */
#else
#define MPE_CONST
#define MPE_CONST1
#endif /* HAVE_CONST_IN_MPI_DECL */


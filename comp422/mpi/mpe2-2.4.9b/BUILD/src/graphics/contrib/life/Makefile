
##### User configurable options #####

MPI_CC      = mpicc 
MPI_CLINKER = $(MPI_CC)
#
MPI_CFLAGS  = 
MPI_LIBS    = 
CFLAGS      = 
MPE_INCDIR  = /projects/comp422/mpi/mpe2-2.4.9b/BUILD/include
MPE_LIBDIR  = /projects/comp422/mpi/mpe2-2.4.9b/BUILD/lib
PROF_LIBS   = -L$(MPE_LIBDIR) -lmpe -lX11 $(MPI_LIBS) -lm
#
INCLUDE_DIR = -I$(MPE_INCDIR)
LDFLAGS     = -lpthread

### End User configurable options ###

RM          = rm

VPATH=.:$(srcdir)

srcdir      = ../../../../../src/graphics/contrib/life

# MPE include path, x_INCS, goes in front of MPI include path, MPI_xFLAGS,
# in case MPI include path is in MPI_xFLAGS containing old MPE include path.
MPICC_FLAGS   = $(INCLUDE_DIR) $(MPI_CFLAGS) $(MPE_COPTS)
MPICC_LDFLAGS = $(LDFLAGS) $(MPI_CFLAGS) $(MPE_LDOPTS)
EXECS         = life_g

.SUFFIXES: .c .o 

.c.o:
	$(MPI_CC) $(MPICC_FLAGS) -c $<

default: $(EXECS)

life_g:	life_g.o
	$(MPI_CLINKER) $(MPICC_LDFLAGS) -o $@ $? $(PROF_LIBS)

clean:
	@-$(RM) -f *.o *~ PI* $(EXECS)

distclean: clean
	@-$(RM) -f Makefile

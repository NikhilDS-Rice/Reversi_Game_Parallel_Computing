ALL: default
##### User configurable options #####

MPI_CC      = mpicc 
MPI_CLINKER = $(MPI_CC)
#
MPI_CFLAGS  = 
MPI_LIBS    = 
CFLAGS      = 
MPE_GRAPH   = @MPE_GRAPHICS@
MPE_INCDIR  = /projects/comp422/mpi/mpe2-2.4.9b/BUILD/include
MPE_LIBDIR  = /projects/comp422/mpi/mpe2-2.4.9b/BUILD/lib
PROF_LIBS   = -L$(MPE_LIBDIR) -lmpe -lX11 $(MPI_LIBS) -lm
#
INCLUDE_DIR = -I$(MPE_INCDIR)
LDFLAGS     = -lpthread

### End User configurable options ###

RM          = rm

VPATH=.:$(srcdir)

srcdir         = ../../../../../src/graphics/contrib/mandel

# MPE include path, x_INCS, goes in front of MPI include path, MPI_xFLAGS,
# in case MPI include path is in MPI_xFLAGS containing old MPE include path.
MPICC_FLAGS    = $(INCLUDE_DIR) $(MPI_CFLAGS) $(MPE_COPTS)
MPICC_LDFLAGS  = $(LDFLAGS) $(MPI_CFLAGS) $(MPE_LDOPTS)
EXECS          = pmandel

.SUFFIXES: .c .o 

.c.o:
	$(MPI_CC) $(MPICC_FLAGS) -c $<

default: $(EXECS)

fract_gen.o: fract_gen.h pmandel.h

pm_genproc.o: pmandel.h fract_gen.h

pmandel.o: pmandel.h fract_gen.h lists.h pm_genproc.h

pm_separate_rect.o: pmandel.h pm_genproc.h

pmandel: pmandel.o pm_genproc.o fract_gen.o pm_separate_rect.o lists.o args.o
	$(MPI_CLINKER) $(MPICC_LDFLAGS) -o $@ $? $(PROF_LIBS)

clean:
	@-$(RM) -f *.o *~ PI* $(EXECS) *.log *.out *.ln

distclean: clean
	@-$(RM) -f Makefile

lint:
	lint -chv $(MPE_GRAPH) $(INCLUDE_DIR) $(MPI_CFLAGS) pmandel.c \
	          pm_genproc.c fract_gen.c pm_separate_rect.c lists.c args.c

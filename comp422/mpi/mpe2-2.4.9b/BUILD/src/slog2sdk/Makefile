MAKE   = make --no-print-directory
RM     = rm -f
SHELL  = /bin/sh
srcdir = ../../../src/slog2sdk

# needed here to properly support distclean
libbuild_dir=/projects/comp422/mpi/mpe2-2.4.9b/BUILD/src/slog2sdk/lib
binbuild_dir=/projects/comp422/mpi/mpe2-2.4.9b/BUILD/src/slog2sdk/bin
docbuild_dir=/projects/comp422/mpi/mpe2-2.4.9b/BUILD/src/slog2sdk/doc
etcbuild_dir=/projects/comp422/mpi/mpe2-2.4.9b/BUILD/src/slog2sdk/etc
sharebuild_dir=/projects/comp422/mpi/mpe2-2.4.9b/BUILD/src/slog2sdk/share
sbinbuild_dir=/projects/comp422/mpi/mpe2-2.4.9b/BUILD/src/slog2sdk/sbin

# The empty string, "", in PKGS is needed to avoid /bin/sh from complaining
# when @PKGS@ is/are really empty.
PKGS   = ""  trace_sample trace_rlog

all:
	@-for pkg in $(PKGS) ; do \
	      if [ -n "$$pkg" -a -s $$pkg/Makefile ] ; then \
	          echo "Making $$pkg" ; \
	          ( cd $$pkg && $(MAKE) $(MAKEOPTS) ) ; \
	      fi ; \
	  done

install:
	@if [ -n "$(PREFIX)" ] ; then \
	     ./sbin/install-package -prefix=$(PREFIX) ; \
	 else \
	     ./sbin/install-package ; \
	 fi

uninstall:
	@-./sbin/uninstall-package

clean:
	@-for pkg in $(PKGS) ; do \
	      if [ -n "$$pkg" -a -s $$pkg/Makefile ] ; then \
	          ( cd $$pkg && $(MAKE) $(MAKEOPTS) clean ) ; \
	      fi ; \
	  done

rmprog:
	@-for file in bin/* sbin/* ; do \
	      case "$$file" in \
	          *.in) ;; \
	          *) $(RM) $$file ;; \
	      esac ; \
	  done

# We only want to try to cleanup the FOObuild_dir directories when we are doing
# a VPATH build.  We chmod because the files are all copied to the build dir
# preserving permissions, so the rm will fail when running "make distcheck"
# because the distcheck process makes the temporary source tree read-only.
#
# In the long run it would be nice to avoid these FOObuild_dir copies.
distclean-misc:
	@-$(RM) sbin/install-package
	@-$(RM) Makefile
	@-$(RM) simple.class simple.jar simple_swing.class simple_swing.jar
	@-$(RM) config.log config.status
	@-$(RM) -r autom4te*.cache
	@-if test '$(srcdir)' != . ; then \
	    chmod -R u+w $(libbuild_dir) $(docbuild_dir) \
	        $(etcbuild_dir) $(sharebuild_dir) $(sbinbuild_dir) ; \
	    $(RM) -r -f $(libbuild_dir) $(docbuild_dir) \
	        $(etcbuild_dir) $(sharebuild_dir) $(sbinbuild_dir) ; \
	fi

distclean: rmprog distclean-misc
	@-for pkg in $(PKGS) ; do \
	      if [ -n "$$pkg" -a -s $$pkg/Makefile ] ; then \
	          echo "making distclean in $$pkg" ; \
	          ( cd $$pkg && $(MAKE) $(MAKEOPTS) distclean ) ; \
	      fi ; \
	  done

maintainer-clean: rmprog distclean-misc
	@-for pkg in $(PKGS) ; do \
	      if [ -n "$$pkg" -a -s $$pkg/Makefile ] ; then \
	          ( cd $$pkg && $(MAKE) $(MAKEOPTS) maintainer-clean ) ; \
	      fi ; \
	  done

owner-clean: rmprog distclean-misc
	@-for pkg in $(PKGS) ; do \
	      if [ -n "$$pkg" -a -s $$pkg/Makefile ] ; then \
	          ( cd $$pkg && $(MAKE) $(MAKEOPTS) owner-clean ) ; \
	      fi ; \
	  done
	@-$(RM) configure

MAKE         = make --no-print-directory
RM           = rm -f
SHELL        = /bin/sh
LIBTOOL_DEPS = ../../../../src/slog2sdk/trace_sample/./ltmain.sh

srcdir            = ../../../../src/slog2sdk/trace_sample
logfilesbuild_dir = /projects/comp422/mpi/mpe2-2.4.9b/BUILD/src/slog2sdk/trace_sample/share/examples/logfiles

all:	
	cd src && $(MAKE) $(MAKEOPTS)

install:
	@if [ -n "$(PREFIX)" ] ; then \
	     ./sbin/install-package -prefix=$(PREFIX) ; \
	 else \
	     ./sbin/install-package ; \
	 fi

uninstall:
	@-./sbin/uninstall-viewer

clean:
	-cd src && $(MAKE) $(MAKEOPTS) clean

rmprog:
	@-for file in bin/* sbin/* ; do \
	      case "$$file" in \
	          *.in) ;; \
	          *) $(RM) -f $$file ;; \
	      esac ; \
	  done

distclean: rmprog
	@-cd src && $(MAKE) $(MAKEOPTS) distclean
	@-$(RM) sbin/install-package
	@-$(RM) Makefile libtool
	@-$(RM) trace_impl.h
	@-$(RM) config.log config.status
	@-$(RM) -r autom4te*.cache
	@-test "$(srcdir)" != . && $(RM) -f $(logfilesbuild_dir)/*.txt

maintainer-clean: distclean
	@-$(RM) trace_impl.h.in

owner-clean: maintainer-clean
	@-$(RM) configure

libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck


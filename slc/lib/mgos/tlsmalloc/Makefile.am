
EXTRA_DIST = \
	tls_arch_mtalpha.h \
	tls_arch_pthreads.h \
	tls_arch_mipsel_sim.h \
	tls_arch_mtsparc_sim.h \
	tls_arch_mtsparc_fpga.h \
	tls_arch_leon2mt_fpga.h

CC = $(PTHREAD_CC)

if CHECK_TLSMALLOC
check_PROGRAMS = tlsmcheck
noinst_PROGRAMS = $(check_PROGRAMS)
TESTS = $(check_PROGRAMS)

tlsmcheck_CPPFLAGS = -DARCH_TLS_SERVICES=\"tls_arch_pthreads.h\" -DFOOTERS -DDEBUG_MALLOC
tlsmcheck_CFLAGS = $(PTHREAD_CFLAGS) $(AM_CFLAGS)
tlsmcheck_LDADD = $(PTHREAD_LIBS)
tlsmcheck_SOURCES = tlsmcheck.c tlsmalloc.c tlsmalloc.h tricks.h tlsconfig.h
endif

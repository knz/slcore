COMMON_TAG = mtsn

include ../common.mk

nobase_pkglib_LIBRARIES += \
	libmgos.a \
	libc.a \
	libm.a \
	seq_naked/libsl.a \
	mtsn_naked/libsl.a \
	mtsn_hybrid/libsl.a

nobase_dist_sllib_DATA += \
	libgcc.a

seq_naked_libsl_a_CFLAGS = -b mtsn_s
seq_naked_libsl_a_SOURCES = $(LIBSL_SRC)
seq_naked_libsl_a_LIBADD = $(libslcommon_a_OBJECTS)
seq_naked_libsl_a_DEPENDENCIES = $(seq_naked_libsl_a_LIBADD)

mtsn_naked_libsl_a_CFLAGS = -b mtsn_n
mtsn_naked_libsl_a_SOURCES = $(LIBSL_SRC)
mtsn_naked_libsl_a_LIBADD = $(libslcommon_a_OBJECTS)
mtsn_naked_libsl_a_DEPENDENCIES = $(mtsn_naked_libsl_a_LIBADD)

mtsn_hybrid_libsl_a_CFLAGS = -b mtsn
mtsn_hybrid_libsl_a_SOURCES = $(LIBSL_SRC)
mtsn_hybrid_libsl_a_LIBADD = $(libslcommon_a_OBJECTS)
mtsn_hybrid_libsl_a_DEPENDENCIES = $(mtsn_hybrid_libsl_a_LIBADD)

include ../mgos.mk
include ../libc.mk

#libgcc_a_CFLAGS = -b $(COMMON_TAG)
#libgcc_a_SOURCES = \
#	ffssi2.c ffsdi2.c \
#	clzsi2.c clzdi2.c \
#	ctzsi2.c ctzdi2.c \
#	div.h \
#	divdi3.c moddi3.c \
#	udivdi3.c umoddi3.c

include mtsmathobjs.mk
libm_a_CFLAGS  = -b $(COMMON_TAG)
libm_a_SOURCES = ../libc/math/missing_uclibc_math.c 
libm_a_LIBADD  = $(MTSMATHOBJS)
libm_a_DEPENDENCIES = $(libm_a_LIBADD)

MALLOC_DEFS_TARGET    = -DPAGESIZE=0x1000U
TLSMALLOC_DEFS_TARGET = -DARCH_TLS_SERVICES=\"tls_arch_mtsparc_sim.h\" 
include ../malloc.mk
include ../tlsmalloc.mk

libc_a_LIBADD += \
	$(libmalloc_a_OBJECTS) \
	$(libtlsmalloc_a_OBJECTS)
libc_a_DEPENDENCIES = $(libc_a_LIBADD)

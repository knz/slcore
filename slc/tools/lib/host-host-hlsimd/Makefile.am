COMMON_TAG = hlsd
include ../common.mk

nobase_pkglib_LIBRARIES += \
	ptl_naked/libslmain.a \
	ptl_hybrid/libslmain.a \
	seq_naked/libslmain.a

seq_naked_libslmain_a_CFLAGS = -b hlsd_s
seq_naked_libslmain_a_SOURCES = $(MAIN_SRC)

ptl_naked_libslmain_a_CFLAGS = -b hlsd_n
ptl_naked_libslmain_a_SOURCES = $(MAIN_SRC)

ptl_hybrid_libslmain_a_CFLAGS = -b hlsd
ptl_hybrid_libslmain_a_SOURCES = $(MAIN_SRC)

include ../host.mk

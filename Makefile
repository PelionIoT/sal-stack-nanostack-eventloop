#
# Makefile for eDTLS library
#

# Define compiler toolchain with CC or PLATFORM variables
# Example (GCC toolchains)
# make PLATFORM=arm-linux-gnueabi-
# OR
# make CC=ArmCC AR=ArmAR
# OR
# make CC=iccarm

LIB = libeventOS.a
SRCS += \
		src/event.c \
		src/ns_timer.c \
		src/system_timer.c\

include ../libService/toolchain_rules.mk

override CFLAGS += -I src/include -I include

#
# External sources from libService
#
SERVLIB_DIR := ../libService
override CFLAGS += -I$(SERVLIB_DIR)/include

$(eval $(call generate_rules,$(LIB),$(SRCS)))

.PHONY: release
release:
	7z a edtls-lib_$(VERSION).zip *.a *.lib include

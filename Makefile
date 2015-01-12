# Define compiler toolchain with CC or PLATFORM variables
# Example (GCC toolchains)
# make PLATFORM=arm-linux-gnueabi-
# OR
# make CC=ArmCC AR=ArmAR
# OR
# make CC=iccarm

LIB = libeventOS.a
SRCS += \
		source/event.c \
		source/ns_timer.c \
		source/system_timer.c\

include ../libService/toolchain_rules.mk

override CFLAGS += -I nanostack-event-loop

#
# External sources from libService
#
SERVLIB_DIR := ../libService
override CFLAGS += -I$(SERVLIB_DIR)/libService

$(eval $(call generate_rules,$(LIB),$(SRCS)))

.PHONY: release
release:
	7z a edtls-lib_$(VERSION).zip *.a *.lib include

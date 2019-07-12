
# vim: ft=make noexpandtab

OBJECTS := mfat.o mbr.o mext2.o
OBJECTS_TESTS := check.o check_mfat.o

BINDIR_LINUX = ../bin/linux
BINDIR_MSP430G2553 = ../bin/msp430g2553

MD=mkdir -v -p

$(BINDIR_LINUX)/libmfat.a: OBJDIR := ../obj/linux
$(BINDIR_LINUX)/libmfat.a: CFLAGS += \
	-DCHECK \
	-DCHECK_FAT_CONSISTENCY \
	-DCHECK_LONG_DATA

$(BINDIR_MSP430G2553)/libmfat.a: OBJDIR := ../obj/msp430g2553
$(BINDIR_MSP430G2553)/libmfat.a: AR := msp430-ar
$(BINDIR_MSP430G2553)/libmfat.a: CC := msp430-gcc
$(BINDIR_MSP430G2553)/libmfat.a: CFLAGS += \
	-mmcu=msp430g2553 \
	-g \
	-pg \
	-Os \
	-DUSE_DISK_RO \
	-fno-builtin \
	-ffreestanding \
	-nostdlib

test_mfat: LDFLAGS += $(shell pkg-config --libs check) -L. -lmfat
test_mfat: CFLAGS += -DCHECK -g -Wall -Werror
test_mfat: OBJDIR := ../obj/test_mfat

all: $(BINDIR_LINUX)/libmfat.a $(BINDIR_MSP430G2553)/libmfat.a

test_mfat: disk.o $(BINDIR_LINUX)/libmfat.a $(addprefix ../obj/linux/,$(OBJECTS_TESTS))
	$(CC) -o $@ $^ -L$(BINDIR_LINUX) $(LDFLAGS)

$(BINDIR_LINUX)/libmfat.a: $(addprefix ../obj/linux/,$(OBJECTS))
	$(MD) $(BINDIR_LINUX)
	$(AR) rcs $@ $^

$(BINDIR_MSP430G2553)/libmfat.a: $(addprefix ../obj/msp430g2553/,$(OBJECTS))
	$(MD) $(BINDIR_MSP430G2553)
	$(AR) rcs $@ $^

../obj/linux/check.o: ../tests/check.c
	$(MD) $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

../obj/linux/check_mfat.o: ../tests/check_mfat.c
	$(MD) $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

../obj/linux/%.o: $(notdir %.c)
	$(MD) $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

../obj/msp430g2553/%.o: $(notdir %.c)
	$(MD) $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean docs

docs:
	doxygen doxyfile

clean:
	rm -rf ../obj; \
	rm -f test_mfat; \
	rm -rf ../bin


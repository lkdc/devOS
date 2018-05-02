
ASFLAGS = --32 -g
CFLAGS = -std=gnu99 -ffreestanding -O0 -Wall -Wextra -g
LDFLAGS = -melf_i386

CC = /home/lkdc/opt/i686/bin/i686-elf-gcc
AS = /home/lkdc/opt/i686/bin/i686-elf-as
LD = /home/lkdc/opt/i686/bin/i686-elf-ld

# CC = gcc
# AS = as
# LD = ld

OBJDIR = build
SRCDIR = src

OBJECT_FILES = \
	$(OBJDIR)/loader.o \
	$(OBJDIR)/main.o \
	$(OBJDIR)/system.o \
	$(OBJDIR)/util.o \
	$(OBJDIR)/gdt.o \
	$(OBJDIR)/idt.o \
	$(OBJDIR)/asm.o \
	$(OBJDIR)/asmirq.o \
	$(OBJDIR)/page.o \
	$(OBJDIR)/timer.o \
	$(OBJDIR)/keyboard.o \
	$(OBJDIR)/scheduler.o

$(OBJECT_FILES): | $(OBJDIR)
$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.s
	$(AS) $(ASFLAGS) -o $@ $<

main.bin: $(SRCDIR)/setup.ld $(OBJECT_FILES)
	$(LD) $(LDFLAGS) -T $< -o $@ $(OBJECT_FILES)

devOS.iso: main.bin
	mkdir isofiles
	mkdir isofiles/boot
	mkdir isofiles/boot/grub
	cp main.bin isofiles/boot/main.bin
	echo 'set timeout=1'                      > isofiles/boot/grub/grub.cfg
	echo 'set default=0'                     >> isofiles/boot/grub/grub.cfg
	echo ''                                  >> isofiles/boot/grub/grub.cfg
	echo 'menuentry "devOS" {'               >> isofiles/boot/grub/grub.cfg
	echo '  multiboot /boot/main.bin'        >> isofiles/boot/grub/grub.cfg
	echo '  boot'                            >> isofiles/boot/grub/grub.cfg
	echo '}'                                 >> isofiles/boot/grub/grub.cfg
	grub-mkrescue --output=devOS.iso isofiles
	rm -rf isofiles

start: devOS.iso
	qemu-system-i386 -s -S -m 256M -cdrom devOS.iso

run: devOS.iso
	qemu-system-i386 -m 256M -cdrom /home/lkdc/git/github-repo/devOS.iso

.PHONY: clean
clean:
	rm -rf isofiles $(OBJDIR) main.bin devOS.iso

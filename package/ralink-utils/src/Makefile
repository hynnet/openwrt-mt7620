
all: switch ated reg mii_mgr pciephy i2ccmd i2scmd  spicmd mmap


switch: switch.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $@.c $(LDLIBS)

ated: ated.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $@.c $(LDLIBS)
	
reg: reg.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $@.c $(LDLIBS)
	
mii_mgr: mii_mgr.c	
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $@.c $(LDLIBS)
	
pciephy:pciephy.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $@.c $(LDLIBS)
	
i2ccmd:i2ccmd.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $@.c $(LDLIBS)

i2scmd:i2scmd.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $@.c $(LDLIBS)
	
spicmd:spicmd.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $@.c $(LDLIBS)
	
mmap:mmap.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $@.c $(LDLIBS)
	
clean:
	-rm -f switch ated gpio reg mii_mgr pciephy i2ccmd i2scmd  spicmd mmap *.elf *.gdb *.o


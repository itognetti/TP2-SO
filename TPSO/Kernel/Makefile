include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=$(wildcard asm/*.asm)
SOURCES_DRIVERS=$(wildcard drivers/*.c)
SOURCES_IDT=$(wildcard idt/*.c)
SOURCES_C=$(wildcard c/*.c)
SOURCES_MEMORY_MANAGERS=$(wildcard managers/memoryManager/*.c)
SOURCES_PROCESS_MANAGERS=$(wildcard managers/processManager/*.c)

OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
OBJECTS_DRIVERS=$(SOURCES_DRIVERS:.c=.o)
OBJECTS_IDT=$(SOURCES_IDT:.c=.o)
OBJECTS_C=$(SOURCES_C:.c=.o)
OBJECTS_MEMORY_MANAGERS=$(SOURCES_MEMORY_MANAGERS:.c=.o)
OBJECTS_PROCESS_MANAGERS=$(SOURCES_PROCESS_MANAGERS:.c=.o)
LOADERSRC=loader.asm 
MEMORY_MANAGER=
#MEMORY_MANAGER=-D USE_BUDDY

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) $(OBJECTS_DRIVERS) $(OBJECTS_IDT) $(OBJECTS_C) $(OBJECTS_MEMORY_MANAGERS) $(OBJECTS_PROCESS_MANAGERS)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) $(OBJECTS_DRIVERS) $(OBJECTS_IDT) $(OBJECTS_C) $(OBJECTS_MEMORY_MANAGERS) $(OBJECTS_PROCESS_MANAGERS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@ $(MEMORY_MANAGER)

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin drivers/*.o idt/*.o c/*.o managers/memoryManager/*.o managers/processManager/*.o

.PHONY: all clean

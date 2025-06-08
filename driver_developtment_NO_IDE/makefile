#	----------------------------------------------------------
#	Note: About <eabi>
#	EABI stands for Embedded Application Binary Interface.
#	It is a set of rules and conventions that define: 
#	1).How function calls are made 
#	2).How data is laid out in memory
#	3).How parameters and return values are passed (e.g., registers vs. stack)
#	4).How the stack is aligned
#	5).How exceptions and interrupts are handled
#	6).How to ensure object files (.o) and libraries (.a,.so) are binary-compatible
# <processor>-<Operating System>-<rules>-<compiler>-<compiler>
#	-----------------------------------------------------------

#	-mcpu=cortex-m4	This specifies the name of the target ARM processor.
#	-march=armv7e-m The name of the target ARM architecture
#	-mtune=cortex-m4 
#										This option specifies the name of the target ARM processor for which GCC
#										should tune the performance of the code. For some ARM implementations
#										better performance can be obtained by using this option. 
#	-mthumb	
#					compact instructions for smaller, more efficient code, while still using the same 32-bit registers and data.
#					It's essential for microcontrollers and useful in size-critical applications.

COMPILER=arm-none-eabi-gcc
ARCHITECTURE=armv7e-m
PROCESSOR=cortex-m4
OPTIMIZATION=0
CFLAGS=-c -mcpu=$(PROCESSOR) -march=$(ARCHITECTURE) -mthumb -O$(OPTIMIZATION)
STD=gnu11
SRCDIR=example

#target:dependency . Therefore, $^ = depency and $@ = target
main.o:$(SRCDIR)/main.c
	$(COMPILER)	$(CFLAGS) -std=$(STD) 	$^	-o	$@

clean:

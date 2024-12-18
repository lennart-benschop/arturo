# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		c.make
#		Purpose :	C Make operations.
#		Date :		12th December 2024
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

# *******************************************************************************************
#
#										Configuration
#
# *******************************************************************************************
#
#		Objects in the core code directory, and the local build directory
#
COREOBJECT = $(patsubst %.c,%.o,$(wildcard $(BASICCODEDIR)*.c $(BASICCODEDIR)*/*.c))
LOCALOBJECT = $(patsubst %.c,%.o,$(wildcard *.c */*.c))
#
#		C Flags
#
CFLAGS = -O2 -Wall -D_FORTIFY_SOURCE=2 -fstack-clash-protection -fstack-protector-strong -pipe -fmax-errors=5
#
#		Program has an int main() and calls CBI_Main(). 
#
BOOTER = $(BASICDIR)testing/boot.o
#
#		Directory containing the code to run it under RISCV on the board
#
RISCRUNDIR = $(ROOTDIR)hardware/risc_test/

# *******************************************************************************************
#
#					Build from multiple files as you normally do in 'C'
#
# *******************************************************************************************
#
#		Build an executable 'normally' with the core & local objects
#
build: $(OBJECT) $(COREOBJECT) $(LOCALOBJECT) $(BOOTER) $(LOCALPRELIM)
	gcc -o $(BINDIR)$(EXECUTABLE) $(COREOBJECT) $(LOCALOBJECT) $(BOOTER)
	echo "Built"
#
#		.c => .o rule
#
%.o : %.c
	$(CC) $(CFLAGS) $(XCFLAGS) -c -I $(INCLUDEDIR) $< -o $@	
#
#		Run the makefile in the scripts directory, which can generate token tables etc.
#
scripts:
	make $(MAKEFLAGS) -B -C $(BASICDIR)/scripts
#
#		Clean all object files
#
clean:
	rm -f $(BINDIR)*
	rm -Rf *.o
	cd $(BASICCODEDIR) ; rm -Rf *.o
#
#		Run the excutable built from multiple files.
#
run: build
	$(BINDIR)$(EXECUTABLE) $(COMMANDLINE)

# *******************************************************************************************
#
#						Combine into one big file and build using that.
#
# *******************************************************************************************
#
#		Combine all the COREOBJECT files into a single .c file, compile the local files and link it all
#		together.
#
one: $(BOOTER)
	$(PYTHON) $(BASICSCRIPTDIR)onefile.py --include=$(INCLUDEDIR) $(COREOBJECT) >$(BINDIR)_onefile.c
	$(CC) $(XCFLAGS) $(CFLAGS) -c -I $(INCLUDEDIR) $(BINDIR)_onefile.c -o _onefile.o
	gcc -o $(BINDIR)$(EXECUTABLE) $(LOCALOBJECT) $(BOOTER) _onefile.o 
	echo "Built one file executable"
#
#		Run the file created above.
#
one-run: one
	$(BINDIR)$(EXECUTABLE)	$(COMMANDLINE)

# *******************************************************************************************
#
#						Run on the RP2350 board using the 'one big file'
#
# *******************************************************************************************

risc-run:
	cp $(TESTSOURCE) $(RISCRUNDIR)__runner.c
ifneq "$(OTHERSOURCES)" ""
	cp $(OTHERSOURCES) $(RISCRUNDIR)
endif
	make one
	make -C $(RISCRUNDIR) runall
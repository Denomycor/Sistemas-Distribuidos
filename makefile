INCLUDEDIR = include
OBJDIR = object
SRCDIR = source
TESTDIR = test
BINDIR = binary
OBJECTFILES = data.o entry.o list-private.o list.o serialization.o table-private.o table.o
TESTSOBJ = test_data.o test_entry.o test_list.o test_serialization.o test_table.o
TESTS = test_data test_entry test_list test_serialization test_table
FLAGS = 



all: clean setup $(OBJECTFILES) $(TESTSOBJ) 
	make .TESTS
	
.TESTS: $(TESTS)

debug: FLAGS +=-g
debug: clean setup $(OBJECTFILES) $(TESTSOBJ)
	make .DEBUGT

.DEBUGT: FLAGS +=-g
.DEBUGT: .TESTS



%.o: $(SRCDIR)/%.c
	gcc $(FLAGS) -o $(addprefix $(OBJDIR)/,$@) -c $< -I $(INCLUDEDIR)

%.o: $(SRCDIR)/$(TESTDIR)/%.c
	gcc $(FLAGS) -o $(addprefix $(OBJDIR)/$(TESTDIR)/,$@) -c $< -I $(INCLUDEDIR)

test_%: $(OBJDIR)/$(TESTDIR)/test_%.o $(OBJDIR)/%.o
	gcc $(FLAGS) $(addprefix $(OBJDIR)/,$(OBJECTFILES))  $< -o $(addprefix $(BINDIR)/,$@.out)

clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)

setup:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)
	mkdir -p $(OBJDIR)/$(TESTDIR)
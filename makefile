INCLUDEDIR = include
OBJDIR = object
SRCDIR = source
TESTDIR = test
BINDIR = binary
OBJECTFILES = data.o entry.o list-private.o list.o serialization.o table-private.o table.o
TESTSOBJ = test_data.o test_entry.o test_list.o test_serialization.o test_table.o
TESTS = test_data test_entry test_list test_serialization test_table

all: setup $(OBJECTFILES) $(TESTSOBJ) $(TESTS)

%.o: $(SRCDIR)/%.c
	gcc -o $(addprefix $(OBJDIR)/,$@) -c $< -I $(INCLUDEDIR)

%.o: $(SRCDIR)/$(TESTDIR)/%.c
	gcc -o $(addprefix $(OBJDIR)/$(TESTDIR)/,$@) -c $< -I $(INCLUDEDIR)

test_%: $(OBJDIR)/$(TESTDIR)/test_%.o $(OBJDIR)/%.o
	gcc $(addprefix $(OBJDIR)/,$(OBJECTFILES))  $< -o $(addprefix $(BINDIR)/,$@)

clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)

setup:
	mkdir -p $(BINDIR)
	mkdir -p $(OBJDIR)
	mkdir -p $(OBJDIR)/$(TESTDIR)

 # Grupo 16
 # Afonso Esteves 54394
 # Vicente Sousa 55386
 # João Anjos 54476


INCLUDEDIR = include
OBJDIR = object
SRCDIR = source
TESTDIR = test
BINDIR = binary

OBJECTFILES = data.o entry.o list-private.o list.o serialization.o table-private.o table.o
OBJECTDEBUGFILES = data_debug.o entry_debug.o list-private_debug.o list_debug.o serialization_debug.o table-private_debug.o table_debug.o

TESTSOBJ = test_data.o test_entry.o test_list.o test_serialization.o test_table.o
TESTDEBUGOBJ = test_data_debug.o test_entry_debug.o test_list_debug.o test_serialization_debug.o test_table_debug.o

TESTS = test_data test_entry test_list test_serialization test_table
TESTSDEBUG = test_data_debug test_entry_debug test_list_debug test_serialization_debug test_table_debug

FLAGS = -g

all:
	make setup
	make m_objfiles
	make m_testobj
	make m_exe

m_objfiles: $(OBJECTFILES)
m_testobj: $(TESTSOBJ)
m_exe: $(TESTS)


debug:
	make setup
	make m_objdebugfiles
	make m_testdebugobj
	make m_exedebug

m_objdebugfiles: $(OBJECTDEBUGFILES)
m_testdebugobj: $(TESTDEBUGOBJ)
m_exedebug: $(TESTSDEBUG)

#.o files
%.o: $(SRCDIR)/%.c
	gcc -o $(addprefix $(OBJDIR)/,$@) -c $< -I $(INCLUDEDIR)

#.o test files
%_debug.o: $(SRCDIR)/%.c
	gcc $(FLAGS) -o $(addprefix $(OBJDIR)/,$@) -c $< -I $(INCLUDEDIR)

#.o debug files
%.o: $(SRCDIR)/$(TESTDIR)/%.c
	gcc -o $(addprefix $(OBJDIR)/$(TESTDIR)/,$@) -c $< -I $(INCLUDEDIR)

#.o debug test files
%_debug.o: $(SRCDIR)/$(TESTDIR)/%.c
	gcc $(FLAGS) -o $(addprefix $(OBJDIR)/$(TESTDIR)/,$@) -c $< -I $(INCLUDEDIR)

#exe files
test_%: $(OBJDIR)/$(TESTDIR)/test_%.o $(OBJDIR)/%.o
	gcc $(addprefix $(OBJDIR)/,$(OBJECTFILES))  $< -o $(addprefix $(BINDIR)/,$@)

#exe debug files
test_%_debug: $(OBJDIR)/$(TESTDIR)/test_%_debug.o $(OBJDIR)/%_debug.o
	gcc $(FLAGS) $(addprefix $(OBJDIR)/,$(OBJECTDEBUGFILES))  $< -o $(addprefix $(BINDIR)/,$@)

#clean directory
clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)

#setup directory
setup:
	mkdir -p $(BINDIR)
	mkdir -p $(OBJDIR)
	mkdir -p $(OBJDIR)/$(TESTDIR)

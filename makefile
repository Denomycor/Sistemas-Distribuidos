# Grupo 16
# Afonso Esteves 54394
# Vicente Sousa 55386
# João Anjos 54476

INCLUDEDIR = include
OBJDIR = object
SRCDIR = source
TESTDIR = test
BINDIR = binary

GETOBJECTFILES := $(shell find ./source -maxdepth 1 -name *.c)
OBJECTFILES := $(GETOBJECTFILES:./source/%.c=%.o)

GETTESTSOBJ := $(shell find ./source/test -name *.c)
TESTSOBJ := $(GETTESTSOBJ:./source/test/%.c=%.o)
TESTS := $(TESTSOBJ:%.o=%)

FLAGS = 
CC = gcc


all: clean setup $(OBJECTFILES) $(TESTSOBJ) 
	make .TESTS
	
.TESTS: $(TESTS)

debug: FLAGS +=-g
debug: clean setup $(OBJECTFILES) $(TESTSOBJ)
	make .DEBUGT

.DEBUGT: FLAGS +=-g
.DEBUGT: .TESTS


#obj files
%.o: $(SRCDIR)/%.c
	$(CC) $(FLAGS) -o $(addprefix $(OBJDIR)/,$@) -c $< -I $(INCLUDEDIR)

#test obj files
%.o: $(SRCDIR)/$(TESTDIR)/%.c
	$(CC) $(FLAGS) -o $(addprefix $(OBJDIR)/$(TESTDIR)/,$@) -c $< -I $(INCLUDEDIR)

#exe files
test_%: $(OBJDIR)/$(TESTDIR)/test_%.o $(addprefix $(OBJDIR)/,$(OBJECTFILES))
	$(CC) $(FLAGS) $^ -o $(addprefix $(BINDIR)/,$@)

#clean directory
clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)

#setup directory
setup:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)
	mkdir -p $(OBJDIR)/$(TESTDIR)
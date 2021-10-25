# Grupo 16
# Afonso Esteves 54394
# Vicente Sousa 55386
# João Anjos 54476

INCLUDEDIR = include
OBJDIR = object
SRCDIR = source
BINDIR = binary


SRCFILES := $(shell find ./source/ -type f \( -iname "*.c" ! -iname "test_*.c" \) )
OBJFILES := $(SRCFILES:./source/%.c=%.o)

#Objs related to table
TABLEOBJ = data.o entry.o list-private.o list.o serialization.o table.o table-private.o

#Objs needed to compile table_client
CLIENTOBJS = client_stub.o network_client.o priv-func.o sdmessage.pb-c.o table_client.o

#Objs needed to compile table_server
SERVEROBJS = table_skel.o network_server.o priv-func.o sdmessage.pb-c.o table_server.o

FLAGS = 
CC = gcc


all: clean setup table_client table_server
	
#.TESTS: $(TESTS)

debug: FLAGS +=-g
debug: clean setup $(OBJFILES) $(TESTSOBJ)
	make .DEBUGT

.DEBUGT: FLAGS +=-g -Wall
.DEBUGT: .TESTS


#obj files
%.o: $(SRCDIR)/%.c
	$(CC) $(FLAGS) -o $(addprefix $(OBJDIR)/,$@) -c $< -I $(INCLUDEDIR)

#table_client.exe
table_client: $(TABLEOBJ) $(CLIENTOBJS)
	$(CC) $(FLAGS) $(addprefix $(OBJDIR)/,$^) -o $(addprefix $(BINDIR)/,$@)

#table_server.exe
table_server: $(TABLEOBJ) $(SERVEROBJS)
	$(CC) $(FLAGS) $(addprefix $(OBJDIR)/,$^) -o $(addprefix $(BINDIR)/,$@)

#clean directory
clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)

#setup directory
setup:
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)
# Grupo 16
# Afonso Esteves 54394
# Vicente Sousa 55386
# João Anjos 54476

INCLUDEDIR = include
OBJDIR = object
SRCDIR = source
BINDIR = binary
LIBDIR = lib


SRCFILES := $(shell find ./source/ -type f \( -iname "*.c" ! -iname "test_*.c" \) )
OBJFILES := $(SRCFILES:./source/%.c=%.o)

#Objs related to table
TABLEOBJ = data.o entry.o list-private.o list.o serialization.o table.o table-private.o

#Objs needed to compile table_server
SERVEROBJS = table_skel.o network_server.o priv-func.o sdmessage.pb-c.o table_server.o message.o stats.o

#Objs needed to compiler client-lib.o
CLIENTLIBOBJS = client_stub.o network_client.o priv-func.o data.o entry.o sdmessage.pb-c.o serialization.o message.o stats.o

FLAGS = -pthread
CC = gcc


#Compile everything
all: clean setup sdmessage.pb-c.o table_client table_server

#Compile with debug flags and all warnings
debug: FLAGS +=-g -Wall
debug: all

#obj files
%.o: $(SRCDIR)/%.c 
	$(CC) $(FLAGS) -o $(addprefix $(OBJDIR)/,$@) -c $< -I $(INCLUDEDIR)

%.pb-c.o: %.proto
	protoc --c_out=. sdmessage.proto && mv sdmessage.pb-c.c $(SRCDIR) && mv sdmessage.pb-c.h $(INCLUDEDIR) && $(CC) $(FLAGS) -o $(OBJDIR)/$@ -c $(SRCDIR)/$(@:.o=.c) -I $(INCLUDEDIR)

#obj client_lib
client-lib.o: $(CLIENTLIBOBJS) 
	ld -r $(addprefix $(OBJDIR)/,$(CLIENTLIBOBJS)) -o $(LIBDIR)/$@

#table_client.exe
table_client: table_client.o client-lib.o
	$(CC) $(FLAGS) -I/usr/include $(OBJDIR)/$< $(LIBDIR)/client-lib.o -o $(addprefix $(BINDIR)/,$@) -L/usr/lib -lprotobuf-c

#table_server.exe
table_server: $(TABLEOBJ) $(SERVEROBJS)
	$(CC) $(FLAGS) -I/usr/include  $(addprefix $(OBJDIR)/,$^) -o $(addprefix $(BINDIR)/,$@) -L/usr/lib -lprotobuf-c

#generate protobuf files
protobuf: sdmessage.proto
	protoc --c_out=. sdmessage.proto
	mv sdmessage.pb-c.c $(SRCDIR)
	mv sdmessage.pb-c.h $(INCLUDEDIR)

#clean directory
clean:
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)
	rm -rf $(LIBDIR)

#setup directory
setup: 
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)
	mkdir -p $(LIBDIR)

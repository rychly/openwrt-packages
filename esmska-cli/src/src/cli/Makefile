CPPFLAGS=-I../include
CFLAGS=-Wall -pedantic -std=c11 -Os
LDFLAGS=-lcurl -lm

OBJDIR=../../objects
OBJECTS= $(addprefix $(OBJDIR)/, main_cli.o parameter_handler.o)

static: $(OBJECTS)
	$(CC) $(CFLAGS) -o ../../esmska-cli $^ $(OBJDIR)/libesmska.a -lcurl -lm

dynamic: $(OBJECTS)
	$(CC) $(CFLAGS) -o ../../esmska-cli $^ ../../libesmska.so.1.0.0

$(OBJDIR)/main_cli.o $(OBJDIR)/parameter_handler.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJDIR)/main_cli.o: main.c
$(OBJDIR)/parameter_handler.o: parameter_handler.c
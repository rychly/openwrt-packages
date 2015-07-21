CPPFLAGS=-I../include
CFLAGS=-Wall -pedantic -std=c11 -Os

OBJDIR=../../objects
OBJECTS= $(addprefix $(OBJDIR)/, main_service.o http_tools.o)

../../esmskad: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ ../../libesmska.so.1.0.0

$(OBJDIR)/main_service.o $(OBJDIR)/http_tools.o:
	$(CC) $(CPPFLAGS) $(OPTIONS) $(CFLAGS) -c $< -o $@

$(OBJDIR)/main_service.o: main.c
$(OBJDIR)/http_tools.o: http_tools.c

SYSCONF_LINK = g++
CPPFLAGS     =
LDFLAGS      =
LIBS         = -lm

DESTDIR = ./
TARGET  = main

OBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: $(DESTDIR)$(TARGET)

$(DESTDIR)$(TARGET): $(OBJECTS)
	$(SYSCONF_LINK) -Wall $(LDFLAGS) -o $(DESTDIR)$(TARGET) $(OBJECTS) $(LIBS)

$(OBJECTS): %.o: %.cpp
	$(SYSCONF_LINK) -Wall $(CPPFLAGS) -c $(CFLAGS) $< -o $@

preview: $(DESTDIR)$(TARGET)
	$(DESTDIR)$(TARGET)
	open *.tga

$(DESTDIR)matrixTest: matrixTest.o geometry.o
	$(SYSCONF_LINK) -Wall $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	-rm -f $(OBJECTS)
	-rm -f $(TARGET)
	-rm -f matrixTest
	-rm -f *.tga


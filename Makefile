SYSCONF_LINK = g++
CPPFLAGS     =
LDFLAGS      =
LIBS         = -lm

DESTDIR = ./
TARGET  = main

all: $(DESTDIR)$(TARGET)

$(DESTDIR)$(TARGET): geometry.o main.o model.o renderer.o tgaimage.o
	$(SYSCONF_LINK) -Wall $(LDFLAGS) -o $(DESTDIR)$(TARGET) geometry.o main.o model.o renderer.o tgaimage.o $(LIBS)

geometry.o: geometry.cpp
	$(SYSCONF_LINK) -Wall $(CPPFLAGS) -c $(CFLAGS) geometry.cpp -o geometry.o

main.o: main.cpp
	$(SYSCONF_LINK) -Wall $(CPPFLAGS) -c $(CFLAGS) main.cpp -o main.o

model.o: model.cpp
	$(SYSCONF_LINK) -Wall $(CPPFLAGS) -c $(CFLAGS) model.cpp -o model.o

renderer.o: renderer.cpp
	$(SYSCONF_LINK) -Wall $(CPPFLAGS) -c $(CFLAGS) renderer.cpp -o renderer.o

tgaimage.o: tgaimage.cpp
	$(SYSCONF_LINK) -Wall $(CPPFLAGS) -c $(CFLAGS) tgaimage.cpp -o tgaimage.o

test.o: test.cpp
	$(SYSCONF_LINK) -Wall $(CPPFLAGS) -c $(CFLAGS) test.cpp -o test.o

preview1: $(DESTDIR)$(TARGET)
	$(DESTDIR)$(TARGET) obj/african_head/african_head.obj obj/african_head/african_head_diffuse.tga output.tga
	open output.tga

preview2: $(DESTDIR)$(TARGET)
	$(DESTDIR)$(TARGET) obj/diablo3_pose/diablo3_pose.obj obj/diablo3_pose/diablo3_pose_diffuse.tga output.tga
	open output.tga

$(DESTDIR)test: test.o geometry.o
	$(SYSCONF_LINK) -Wall $(LDFLAGS) -o $@ $^ $(LIBS)
	./test

clean:
	-rm -f geometry.o
	-rm -f main.o
	-rm -f model.o
	-rm -f renderer.o
	-rm -f test.o
	-rm -f tgaimage.o
	-rm -f $(TARGET)
	-rm -f test
	-rm -f output.tga


CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
CFLAGS = -O3 -g -std=c++11 -DGL_GLEXT_PROTOTYPES -DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED -DOSX -Wno-deprecated-register -Wno-deprecated-declarations -Wno-shift-op-parentheses
INCFLAGS = -I./glm-0.9.7.1 -I/usr/X11/include -I./include/
LDFLAGS = -L./lib/mac/ -lm -lstdc++ -lfreeimage
else
CFLAGS = -O3 -g -std=c++11 -DGL_GLEXT_PROTOTYPES
INCFLAGS = -I./glm-0.9.7.1 -I./include/ -I/usr/X11R6/include -I/sw/include \
		-I/usr/sww/include -I/usr/sww/pkg/Mesa/include
LDFLAGS = -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib -L./lib/nix/ \
		-L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib  -lX11 -lfreeimage
endif

RM = /bin/rm -f
all: transforms
transforms: main.o Transform.o readfile.o ray.o \
	 variables.h readfile.h Transform.h
	$(CC) $(CFLAGS) -o transforms main.o Transform.o readfile.o ray.o  $(INCFLAGS) $(LDFLAGS)
main.o: main.cpp Transform.h variables.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c main.cpp
ray.o: ray.cpp variables.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c ray.cpp
readfile.o: readfile.cpp readfile.h variables.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c readfile.cpp
Transform.o: Transform.cpp Transform.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c Transform.cpp
# display.o: display.cpp display.hpp variables.h Transform.h readfile.h
# 	$(CC) $(CFLAGS) $(INCFLAGS) -c display.cpp
clean:
	$(RM) *.o transforms *.png

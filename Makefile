SHELL= /bin/sh
CC= cc
FLAGS=
CFLAGS= -c -fPIC -Wall -Wextra -O2 -g 
LDFLAGS= -v -shared
RM = rm -f

TARGET_LIB = libgraphicc.so
SRCS= bezier.cpp \
	font.cpp 	\
	linearalgebra.cpp \
	pbm.cpp \
	pixelbuffer.cpp \
	raster_rgba.cpp \
	raster_rgba_polygon.cpp \
	raster_rgba_triangle.cpp

OBJECTS= $(SRCS:.cpp=.o)


all: $(TARGET_LIB) 

$(TARGET_LIB): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^


%.o:%.cpp
	$(CC) $(CFLAGS) $< >$@

.PHONY: clean
clean:
	-$(RM) $(OBJECTS) $(TARGET_LIB)


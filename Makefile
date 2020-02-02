# 
# Makefile for QR Code generator (C)
# 
# Copyright (c) Project Nayuki. (MIT License)
# https://www.nayuki.io/page/qr-code-generator-library
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
# - The above copyright notice and this permission notice shall be included in
#   all copies or substantial portions of the Software.
# - The Software is provided "as is", without warranty of any kind, express or
#   implied, including but not limited to the warranties of merchantability,
#   fitness for a particular purpose and noninfringement. In no event shall the
#   authors or copyright holders be liable for any claim, damages or other
#   liability, whether in an action of contract, tort or otherwise, arising from,
#   out of or in connection with the Software or the use or other dealings in the
#   Software.
# 


# ---- Configuration options ----

# External/implicit variables:
# - CC: The C compiler, such as gcc or clang.
# - CFLAGS: Any extra user-specified compiler flags (can be blank).

# Recommended compiler flags:
CFLAGS += -std=c99 -O 
CFLAGS += `sdl-config --cflags --libs`
# Extra flags for diagnostics:
# CFLAGS += -g -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -fsanitize=undefined,address


# ---- Controlling make ----

# Clear default suffix rules
.SUFFIXES:

# Don't delete object files
.SECONDARY:

# Stuff concerning goals
.DEFAULT_GOAL = all
.PHONY: all clean


# ---- Targets to build ----

LIB = qrcodegen 
LIBFILE = lib$(LIB).a 
LIBOBJ = qrcodegen.o
MAINS = qrcodegen-demo qrcodegen-test qrcodegen-worker

# Build all binaries
all: $(LIBFILE) $(MAINS)

# Delete build output
clean:
	rm -f -- $(LIBOBJ) $(LIBFILE) $(MAINS:=.o) $(MAINS)
	rm -rf .deps

# Executable files
%: %.o $(LIBFILE)
	$(CC) $(CFLAGS) -o $@ $< -L . -l $(LIB)

# Special executable
qrcodegen-test: qrcodegen-test.c $(LIBOBJ:%.o=%.c) `sdl-config --cflags --libs` 
	$(CC) $(CFLAGS) -DQRCODEGEN_TEST -o $@ $^

# The library
$(LIBFILE): $(LIBOBJ)
	$(AR) -crs $@ -- $^

# Object files
%.o: %.c .deps/timestamp
	$(CC) $(CFLAGS) -c -o $@ -MMD -MF .deps/$*.d $<

# Have a place to store header dependencies automatically generated by compiler
.deps/timestamp:
	mkdir -p .deps
	touch .deps/timestamp

# Make use of said dependencies if available
-include .deps/*.d

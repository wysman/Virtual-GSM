#
#	The MIT License (MIT)
#	
#	Copyright (c) 2012 William MARTIN
#	
#	Permission is hereby granted, free of charge, to any person obtaining a copy
#	of this software and associated documentation files (the "Software"), to deal
#	in the Software without restriction, including without limitation the rights
#	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#	copies of the Software, and to permit persons to whom the Software is
#	furnished to do so, subject to the following conditions:
#	
#	The above copyright notice and this permission notice shall be included in
#	all copies or substantial portions of the Software.
#	
#	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#	THE SOFTWARE.
#

RAGEL_SOURCES = protocole.rl
RAGEL_GENERATED_SOURCES = $(patsubst %.rl, %.c, ${RAGEL_SOURCES})
RAGEL_CHARTS = protocole.pdf

SOURCES=modem.c \
        cmd.c \
        $(RAGEL_GENERATED_SOURCES)
OBJECTS=$(patsubst %.c, %.o, ${SOURCES})
EXEC = virtualGSM

CFLAGS = -Wall -Wextra -std=c99

.SUFFIXES:
.SUFFIXES: .o .c .rl .pdf
.PRUNE:

all: charts
all: ${OBJECTS}
	$(CC) $(LDFLAGS) -o ${EXEC} ${OBJECTS}

dev: CFLAGS += -g -O0
dev: all

clean:
	rm -f ${OBJECTS} ${EXEC}
	
mrproper: clean
	rm -f ${RAGEL_GENERATED_SOURCES}

.rl.c:
	@printf "RAGEL\t$<\n"
	@ragel -G2 $<
	
.c.o:
	@printf "CC\t$<\n"
	@$(CC) $(CFLAGS) -c $< -o $@
	
charts: ${RAGEL_CHARTS}

.rl.pdf:
	ragel -V -M main $< | sed -r '/EOF|DEF|eof_|err_/d' | dot -Tpdf -o $@
	
.SECONDARY:


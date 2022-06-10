DEBUGER=colour-valgrind --num-callers=30 --leak-check=full --show-leak-kinds=all --show-error-list=yes --read-inline-info=yes --read-var-info=yes --keep-debuginfo=yes --track-origins=yes

CC=gcc
CFLAGS =-Wall -Wextra -Wno-unused-parameter -Wmissing-prototypes -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wrestrict -Wnull-dereference -Wjump-misses-init -Wdouble-promotion -Wshadow -Wformat=2
CFLAGS+= -fPIC -I ./src/
CFLAGS+= -Werror

.PHONY: iso8583
iso8583: lib/libiso8583.a

lib/libiso8583.a: obj/iso8583.o obj/mti.o obj/bitmaps.o obj/data.o obj/data_type.o obj/functions.o
	$(AR) -crus $@ $^

obj/%.o: src/%.c src/%.h
	$(CC) $(CFLAGS) -o $@ -c $<

bin/example: CFLAGS+= -g3 -DDEBUG -D_DEBUG -finstrument-functions
bin/example: LFLAGS+= -Wl,no_pie
bin/example: src/example/example.c src/test/strings.c lib/libiso8583.a
	$(CC) $(CFLAGS) -o $@ $^

bin/test: CFLAGS+= -g3 -DDEBUG -D_DEBUG -finstrument-functions
bin/test: LFLAGS+= -Wl,no_pie
bin/test: .EXTRA_PREREQS= src/test/test.h src/test/strings.h src/test/globals.h src/test/print.h src/test/globals.h
bin/test: src/test/test.c src/test/strings.c src/test/print.c lib/libiso8583.a
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: example
example: bin/example
	$(DEBUGER) bin/example

.PHONY: test
test: bin/test
	export COLUMNS; $(DEBUGER) ./bin/test --file src/test/input.txt

.PHONY: install
install: lib/libiso8583.a src/iso8583.h
	@echo TODO: Install (copy libiso8583.a and iso8583.h to respective directory)
#ld --verbose | /usr/bin/grep SEARCH_DIR | tr -s " ;" "\n"

.PHONY: clean
clean:
	rm -f bin/* obj/*.o lib/libiso8583.a

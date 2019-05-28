PROG = net_server
SOURCES = $(PROG).c ./op_db.c ./sqlite3.c ./mongoose.c
CFLAGS = -W -Wall $(CFLAGS_EXTRA)

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
  CFLAGS += -ldl -lm -pthread
endif

all: $(PROG)

$(PROG): $(SOURCES)
	$(CC) $(SOURCES) -o $@ $(CFLAGS)

clean:
	rm -rf *.gc* *.dSYM *.exe *.obj *.o a.out $(PROG)

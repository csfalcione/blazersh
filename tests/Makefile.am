TESTS = check_blazersh
check_PROGRAMS = check_blazersh
check_blazersh_SOURCES = check_main.c helpers.c \
     $(top_builddir)/src/strarray.h $(top_builddir)/src/parser.h \
     $(top_builddir)/src/tokenizer.h
check_blazersh_CFLAGS = @CHECK_CFLAGS@
check_blazersh_LDADD = $(top_builddir)/src/libblazersh.la @CHECK_LIBS@
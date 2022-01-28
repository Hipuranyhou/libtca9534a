ACLOCAL_AMFLAGS = -Im4

AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = -I$(top_srcdir)/include -Wall -Wextra -pedantic

lib_LTLIBRARIES = libtca9534a.la

libtca9534a_la_SOURCES =         \
    $(top_srcdir)/src/i2c.c      \
    $(top_srcdir)/src/tca9534a.c
    
include_HEADERS =                    \
    $(top_srcdir)/include/tca9534a.h

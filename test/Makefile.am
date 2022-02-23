ACLOCAL_AMFLAGS = -I m4

# Adjust as needed
AM_CPPFLAGS = $(HDF5_CPPFLAGS) -I$(top_srcdir)/src -I$(top_builddir)/src
AM_CFLAGS = -g -Wall -Wextra $(HDF5_CFLAGS)

check_SCRIPTS = test_tutorial.sh

check_PROGRAMS = simple_tests
simple_tests_LDFLAGS = $(AM_LDFLAGS) $(HDF5_LDFLAGS)
simple_tests_LDADD = $(HDF5_LIBS)

TESTS = test_tutorial.sh

DISTCLEANFILES = test_tutorial.sh

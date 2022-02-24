ACLOCAL_AMFLAGS = -I m4

# Adjust as needed
AM_CPPFLAGS = $(HDF5_CPPFLAGS)
AM_CFLAGS = -g -Wall -Wextra -fPIC $(HDF5_CFLAGS)

# Public header
include_HEADERS = tutorial_vol_connector.h

# Tutorial VOL connector
lib_LTLIBRARIES = libtutorial_vol_connector.la
libtutorial_vol_connector_la_SOURCES = \
	tutorial_util.c \
	tutorial_vol_connector.c
libtutorial_vol_connector_la_LDFLAGS = $(AM_LDFLAGS) $(HDF5_LDFLAGS) -avoid-version -module -shared -export-dynamic
libtutorial_vol_connector_la_LIBADD = $(HDF5_LIBS)


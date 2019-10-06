lib_LTLIBRARIES = safeGenocide.la

safeGenocide_la_SOURCES = safeGenocide.cpp
safeGenocide_la_CPPFLAGS= -I$(top_srcdir)/include -I$(top_srcdir)/plugins/plugin_utils
safeGenocide_la_LDFLAGS = -module -avoid-version -shared
safeGenocide_la_LIBADD = $(top_builddir)/plugins/plugin_utils/libplugin_utils.la

AM_CPPFLAGS = $(CONF_CPPFLAGS)
AM_CFLAGS = $(CONF_CFLAGS)
AM_CXXFLAGS = $(CONF_CXXFLAGS)

EXTRA_DIST = \
	LICENSE.md \
	README.md \
	safeGenocide.def \
	safeGenocide.sln \
	safeGenocide.vcxproj \
	safeGenocide.vcxproj.filters

MAINTAINERCLEANFILES =	\
	Makefile.in

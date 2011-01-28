## Process this file with automake to generate Makefile.in

AUTOMAKE_OPTIONS = foreign

noinst_PROGRAMS = \
old_parser_test \
set_operators

old_parser_test_SOURCES = old_parser_test.cc
set_operators_SOURCES = set_operators.cc

CXXFLAGS = @CXXFLAGS@ @CWD_FLAGS@
INCLUDES = -I$(builddir)/.. -I$(srcdir)/..

clean-local:
	@MAINTAINER_MODE_TRUE@rm -f core.[0-9]*

MAINTAINERCLEANFILES = Makefile.in

all:


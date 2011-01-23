## Process this file with automake to generate Makefile.in

AUTOMAKE_OPTIONS = foreign

noinst_PROGRAMS = \
CTopTest \
old_parser_test \
set_operators

CTopTest_SOURCES = CTopTest.cc
old_parser_test_SOURCES = old_parser_test.cc
set_operators_SOURCES = set_operators.cc

CXXFLAGS = @CXXFLAGS@ @CWD_FLAGS@
INCLUDES = -I$(builddir)/.. -I$(srcdir)/..

CTopTest_LDADD = $(builddir)/../contribmerge-ContributionsTxt.o $(builddir)/../contribmerge-ContributionsTxt_operators.o $(builddir)/../contribmerge-ostream_operators.o
CTopTest_CXXFLAGS = -DSRCDIR="$(srcdir)"

clean-local:
	@MAINTAINER_MODE_TRUE@rm -f core.[0-9]*

MAINTAINERCLEANFILES = Makefile.in

all:


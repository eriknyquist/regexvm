AUTOMAKE_OPTIONS = foreign
SUBDIRS = librxvm examples tests/src
TEST_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) \
				  $(top_srcdir)/build-aux/tap-driver.sh

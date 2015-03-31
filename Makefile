all: configure build build/Makefile 
	make -C build all
	
clean:
	test -d build && make -C build clean

build: configure
	mkdir -p build
	(cd build; ../configure)
	
configure: configure.ac Makefile.am src/Makefile.am
	aclocal && automake --add-missing && autoconf
	
dist-clean:
	rm -fr *~ build configure compile depcomp install-sh missing aclocal.m4 autom4te.cache \
	    src/*~ src/Makefile.in Makefile.in 
	
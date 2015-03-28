
builddir=./objs

AM_CXXFLAGS = \
-I/usr/include/giomm-2.4 \
-I/usr/lib/x86_64-linux-gnu/giomm-2.4/include \
-I/usr/include/glibmm-2.4 \
-I/usr/lib/x86_64-linux-gnu/glibmm-2.4/include \
-I/usr/include/glib-2.0 \
-I/usr/lib/x86_64-linux-gnu/glib-2.0/include \
-I/usr/include/sigc++-2.0 \
-I/usr/lib/x86_64-linux-gnu/sigc++-2.0/include 



AM_LDFLAGS =  -lgiomm-2.4  -lgio-2.0 -lglibmm-2.4 -lgobject-2.0 -lsigc-2.0 -lglib-2.0 -lz -lpthread 

bin_PROGRAMS = canmed 

canmed_SOURCES = main.cpp \
 appcontext.cpp  handler_base.cpp  handler_factory.cpp \
 canmessage.cpp  handler.cpp       keyvaluetable.cpp    utils.cpp



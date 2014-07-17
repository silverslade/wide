# Makefile for UNIX systems
#
# Found to work fine under Linux with unicode version of wxGTK 2.8

#CXX   = g++ -DSHARED=0 -DRUNTIME_LIBS=static -DMONOLITHIC=1 -DUNICODE=1 
CXX   = g++
STRIP = strip
PARAMS = -DSHARED=0 -DRUNTIME_LIBS=static -DMONOLITHIC=1 -DUNICODE=1

WX_FLAGS = `wx-config --debug=no --cxxflags`
WX_LIBS  = `wx-config --debug=no --libs aui,stc`

# UNCOMMENT THIS IN CASE OF LINKING ERRORS
WX_LIBS =  -pthread -Wl,-O1,--sort-common,-z,relro -lwx_gtk2u_richtext-2.8 -lwx_gtk2u_aui-2.8 -lwx_gtk2u_xrc-2.8 -lwx_gtk2u_qa-2.8 -lwx_gtk2u_html-2.8 -lwx_gtk2u_adv-2.8 -lwx_gtk2u_core-2.8 -lwx_baseu_xml-2.8 -lwx_baseu_net-2.8 -lwx_baseu-2.8  -lwx_gtk2u_aui-2.8 -lwx_gtk2u_stc-2.8 -lwx_baseu-2.8 

CXXFLAGS = $(WX_FLAGS) 
LIBS     = $(WX_LIBS)


all: wide

clean:
	rm -f edit.o main.o prefs.o wide

edit.o: defsext.h edit.cpp edit.h
	$(CXX) $(CXXFLAGS) -c edit.cpp

main.o: defsext.h main.cpp main.h
	$(CXX) $(CXXFLAGS) -c main.cpp

prefs.o: defsext.h prefs.cpp prefs.h
	$(CXX) $(CXXFLAGS) -c prefs.cpp

wide: edit.o main.o prefs.o
	$(CXX) $(LIBS) -o wide edit.o main.o prefs.o
	$(STRIP) -s wide

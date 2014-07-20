# Makefile for UNIX systems
#
# Found to work fine under Linux with unicode version of wxGTK 3.0

#CXX   = g++ -DSHARED=0 -DRUNTIME_LIBS=static -DMONOLITHIC=1 -DUNICODE=1 
CXX   = g++
STRIP = strip
PARAMS = -DSHARED=0 -DRUNTIME_LIBS=static -DMONOLITHIC=1 -DUNICODE=1 

WX_FLAGS = `wx-config --debug=no --cxxflags`
WX_LIBS  = `wx-config --debug=no --libs aui,stc`

# UNCOMMENT THIS IN CASE OF LINKING ERRORS
WX_LIBS =  -pthread -Wl,-O1,--sort-common,-z,relro -lwx_gtk2u_richtext-3.0 -lwx_gtk2u_aui-3.0 -lwx_gtk2u_xrc-3.0 -lwx_gtk2u_qa-3.0 -lwx_gtk2u_html-3.0 -lwx_gtk2u_adv-3.0 -lwx_gtk2u_core-3.0 -lwx_baseu_xml-3.0 -lwx_baseu_net-3.0 -lwx_baseu-3.0  -lwx_gtk2u_aui-3.0 -lwx_gtk2u_stc-3.0 -lwx_baseu-3.0 

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

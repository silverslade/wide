WIDE - WxWidgets Ide for Inform 6
=================================

install on Linux:

1) install runtime wxgtk2.8
2) make
3) in case of errors uncomment line starting by "WX_LIBS" (see the makefile)
you shouldn't use --as-needed, you --no-as-needed instead


install on Windows:

1) install and compile wxwidgets (using static=0 monolithic=1)
2) install MinGW
3) compile stc oomponent
4) cd wide
5) edit makefile.gcc to fit to your paths
6) mingw32-make -f makefile.gcc


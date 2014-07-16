WIDE - WxWidgets IDE for Inform 6
=================================

WIDE is an Integrated Development Environment (IDE) for the Inform 6 Language.

Wide is a win32 Editor for Inform6: is an Integrated Development Environment (IDE), written entirely in C++/WxWidgets, for the creation of text adventures based on Graham Nelson's Inform6 Language. With Wide it's possible to edit, compile and run a text adventure in the z-code and glulx formats.

Main Features:

* Written in C++ (using the wxWidgets Library)
* Wide runs on Win32 / Linux systems
* Wide is localized in spanish, english, italian, german and french (simply customizing its wide.ini file...)
* Auto-completion window, in edit mode
* Wide is just an exe file: no DLL, no virtual machines. Wide is a stand alone application.
* Wrap lines, show number of lines, code folding
* Inform/Glulx support: wide supports zcode (5 and 8 version), ulx, blb, bres
* Objects tree and Syntax highlight is customizable
* Wide is portable: it can be started from an USB key/pen

Authors:
* Alessandro Schillaci
* Paolo Lucchesi

License:
Wide is an open source project (GNU GPL License).

Thanks to:
Urbatain, Christof Menear and Eric Forgeot for the wide translation in spanish, german and french.
Josè Manuel, for hints, suggestions and testing


INSTALL

Install on Linux:

1) install runtime wxgtk2.8
2) make
3) in case of errors uncomment line starting by "WX_LIBS" (see the makefile)
you shouldn't use "--as-needed" flag, use "--no-as-needed" instead


Install on Windows:

1) install and compile wxwidgets (using static=0 monolithic=1)
2) install MinGW
3) compile stc oomponent
4) cd wide
5) edit makefile.gcc to fit to your paths
6) mingw32-make -f makefile.gcc



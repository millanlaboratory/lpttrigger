EXTRA_DIST = libwinio.a WinIo.lib WinIo.def WinIo.dll WinIo.sys WINIO.VXD

if INSTALL_WINIO
install-exec-local:
	cp WinIo.dll $(DESTDIR)$(bindir)/WinIo.dll
	cp WinIo.sys $(WINSYSDIR)/WinIo.sys
	cp WINIO.VXD $(WINSYSDIR)/WINIO.VXD

uninstall-local:
	rm -f $(DESTDIR)$(bindir)/WinIo.dll
	rm -f $(WINSYSDIR)/WinIo.sys
	rm -f $(WINSYSDIR)/WINIO.VXD
endif

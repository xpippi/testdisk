SUBDIRS = icons man src
docdir ?= $(datadir)/doc/$(PACKAGE)

EXTRA_DIST = AUTHORS COPYING ChangeLog INFO INSTALL NEWS README.md README_dev_photorec.txt THANKS compile.sh \
	progsreiserfs-journal.patch progsreiserfs-file-read.patch \
	darwin/readme_mac_powerpc.txt darwin/readme_mac_intel.txt \
	documentation.html \
	dos/readme.txt \
	linux/testdisk.spec.in \
	linux/qphotorec.desktop \
	src/icon_ph.rc src/icon_qph.rc src/icon_tst.rc \
	src/photorec_64x64.png \
	src/gnome/README \
	src/gnome/application-exit.png  \
	src/gnome/application-x-cd-image.png \
	src/gnome/drive-harddisk.png \
	src/gnome/folder.png \
	src/gnome/go-next.png \
	src/gnome/go-previous.png \
	src/gnome/help-about.png \
	src/gnome/image-x-generic.png \
	src/lang/qphotorec.ca.ts \
	src/lang/qphotorec.cs.ts \
	src/lang/qphotorec.el.ts \
	src/lang/qphotorec.es.ts \
	src/lang/qphotorec.fi.ts \
	src/lang/qphotorec.fr.ts \
	src/lang/qphotorec.it.ts \
	src/lang/qphotorec.ja.ts \
	src/lang/qphotorec.pl.ts \
	src/lang/qphotorec.pt.ts \
	src/lang/qphotorec.ru.ts \
	src/lang/qphotorec.sv_SE.ts \
	src/lang/qphotorec.tr.ts \
	src/lang/qphotorec.zh_CN.ts \
	src/lang/qphotorec.zh_TW.ts \
	win/qt.conf \
	win/readme.txt win/photorec_win.exe.manifest win/qphotorec_win.exe.manifest \
	win/testdisk_win.exe.manifest \
	Android.mk jni/Android.mk \
	.travis.yml

install-data-hook:
	$(mkinstalldirs) $(DESTDIR)$(docdir)
	$(INSTALL_DATA) $(top_srcdir)/AUTHORS $(top_srcdir)/ChangeLog $(top_srcdir)/NEWS $(top_srcdir)/README.md $(top_srcdir)/THANKS $(top_srcdir)/documentation.html $(DESTDIR)$(docdir)
if USEQT
	$(mkinstalldirs) $(DESTDIR)$(datadir)/applications
	$(INSTALL_DATA) $(top_srcdir)/linux/qphotorec.desktop $(DESTDIR)$(datadir)/applications
endif

uninstall-hook:
	rm -rf $(DESTDIR)$(docdir)
if USEQT
	rm -f $(DESTDIR)$(datadir)/applications/qphotorec.desktop
endif

static:
	rm -f src/testdisk src/testdisk.exe src/photorec src/photorec.exe src/fidentify src/fidentify.exe src/qphotorec src/qphotorec.exe
	$(MAKE) LDFLAGS="$(LDFLAGS) -static" LIBS="$(PTHREAD_LIBS) $(LIBS)" CFLAGS="$(PTHREAD_CFLAGS) $(CFLAGS)" CXXFLAGS="$(PTHREAD_CFLAGS) $(CXXFLAGS)"

smallstatic:
	$(MAKE) static
	(cd src && $(MAKE) small) || exit 1;

extras:
	(cd src && $(MAKE) extras) || exit 1;

extrasstatic:
	$(MAKE) LDFLAGS="$(LDFLAGS) -static" LIBS="$(PTHREAD_LIBS) $(LIBS)" CFLAGS="$(PTHREAD_CFLAGS) $(CFLAGS)" CXXFLAGS="$(PTHREAD_CFLAGS) $(CXXFLAGS)" extras

session_%.framac:
	(cd src && $(MAKE) $@) || exit 1;

frama-c-%:
	(cd src && $(MAKE) $@) || exit 1;

cppcheck:
	cppcheck --quiet --enable=all -DHAVE_CONFIG_H -I$(builddir) -I/usr/include $(srcdir)/src

DISTCLEANFILES = *~ core

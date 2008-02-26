SUBDIRS = src po

EXTRA_DIST = config.rpath  m4/ChangeLog \
		bootstrap esteidutil.spec.in esteidutil.spec esteidutil.desktop	\
		esteidutil.xpm EsteidUtil.sln COPYING \
		debian/changelog  debian/control  debian/copyright \
		debian/rules debian/menu debian/docs debian/compat \
		debian/EsteidUtil.sgml esteidutil.desktop \
		mac/EsteidUtil.app/Contents/MacOS/EsteidUtil \
		mac/EsteidUtil.app/Contents/Resources/wxmac.icns \
		mac/EsteidUtil.app/Contents/Info.plist \
		mac/EsteidUtil.app/Contents/PkgInfo

docdir = ${datadir}/EsteidUtil
doc_DATA = README

pixmapdir = $(datadir)/pixmaps
pixmap_DATA = esteidutil.xpm

#humm .. this im not so sure about
xdg_appsdir = ${datadir}/applications
xdg_apps_DATA = esteidutil.desktop

EsteidUtil.app: all

ACLOCAL_AMFLAGS = -I m4

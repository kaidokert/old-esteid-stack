SUBDIRS = src po
#m4 
EXTRA_DIST = config.rpath  m4/ChangeLog \
		bootstrap esteidutil.spec.in esteidutil.spec esteidutil.desktop	\
		esteidutil.xpm EsteidUtil.sln COPYING \
		esteidutil.desktop \
		mac/EsteidUtil.app/Contents/MacOS/EsteidUtil \
		mac/EsteidUtil.app/Contents/Resources/wxmac.icns \
		mac/EsteidUtil.app/Contents/Info.plist \
		mac/EsteidUtil.app/Contents/PkgInfo \
		premake4.lua \
		build/vs2005/ build/vs2008/ build/codeblocks/ \
		esteidcsp EstEIDSigningPluginBHO qt-id-updater 

docdir = ${datadir}/esteidutil
doc_DATA = README

pixmapdir = $(datadir)/pixmaps
pixmap_DATA = esteidutil.xpm

#humm .. this im not so sure about
xdg_appsdir = ${datadir}/applications
xdg_apps_DATA = esteidutil.desktop

esteidutil.app: all

ACLOCAL_AMFLAGS = -I m4

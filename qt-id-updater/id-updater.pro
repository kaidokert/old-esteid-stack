TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release
QT += network xml

FORMS	= idupdater.ui
HEADERS = idupdater.h
SOURCES = idupdater.cpp
SOURCES += main.cpp 

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



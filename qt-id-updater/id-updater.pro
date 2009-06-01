TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release
QT += network xml

FORMS	= idupdater.ui
HEADERS = idupdater.h   InstallChecker.h   ProcessStarter.h   ScheduledUpdateTask.h
SOURCES = idupdater.cpp InstallChecker.cpp ProcessStarter.cpp ScheduledUpdateTask.cpp
SOURCES += main.cpp
INCLUDEPATH += ../src/cardlib/
HEADERS += ../src/cardlib/DynamicLibrary.h  precompiled.h
SOURCES += ../src/cardlib/DynamicLibrary.cpp

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



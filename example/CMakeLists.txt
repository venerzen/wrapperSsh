# set project's name
cmake_minimum_required(VERSION 2.8)
PROJECT( test )
# with SET() command you can change variables or define new ones
# here we define SAMPLE_SRCS variable that contains a list of all .cpp files
# note that we don't need \ at the end of line
SET( SRCS
	./main.cpp
	./libs/CtrlSsh.cpp
	./libs/CtrlSshException.cpp
	./libs/WrapperSsh.cpp
	./libs/WrapperSshException.cpp
	./libs/string.cpp
)
#	Patch pour pouvoir compiler sous osx avec la libssh2
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /opt/local/lib /usr/local/lib)
set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} /opt/local/include /usr/local/include)
link_directories(/usr/lib/ /opt/local/lib/ /usr/local/lib)
include_directories(/usr/include/ /opt/local/include/ /usr/local/include/)
# another list, this time it includes all header files that should be treated with moc
#SET( MOC_HDRS
#	./libs/checkHardware.h
#	./libs/checkHardwareIpmi.h
#	./libs/checkHardwareSsh.h
#	./libs/checkHardwareSnmp.h
#	./libs/QCoreConsole.h
#	./libs/NagiosAbstract/QCoreNagios.h
#	./libs/QGetOptEx/QGetOptEx.h
#	./libs/QThreadEx/QThreadEx.h
#	./libs/abstractThreadedObj.h 
#)
# enable warnings
ADD_DEFINITIONS( -Wall ${QT_DEFINITIONS} )

# Ajout de la gestion des langues, soit, ajout de la liste des fichiers de traduction dont on doit tenir compte
SET(LANG
#	./translations/checkHardware_fr.ts
#	./translations/checkHardware_en.ts
)
# by default only QtCore and QtGui modules are enabled
# other modules must be enabled like this:
#SET( QT_USE_QTHREAD TRUE )
#SET (QT_USE_QTCORE TRUE)

# this command finds Qt4 libraries and sets all required variables
# note that it's Qt4, not QT4 or qt4
#FIND_PACKAGE( Qt4 REQUIRED )
FIND_PATH(myPath libssh2.h )

# add some useful macros and variables
# (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
#INCLUDE( ${QT_USE_FILE} )

# and finally this will run moc:
#QT4_WRAP_CPP( MOC_SRCS ${MOC_HDRS} )
# & now, the translations 
#QT4_ADD_TRANSLATION(TRANSLATIONS ${LANG})
#QT4_CREATE_TRANSLATION(TRANSLATIONS ${LANG})

# we need this to be able to include headers produced by uic in our code
# (CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES() works just like INCLUDEPATH from qmake)
INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} )
#	Ajout de certaines librairies non liées à QT
#file(
#	GLOB_RECURSE
#	WrapperTds_files
#	libs/WrapperTds/*
#)
# here we instruct CMake to build "sample" executable from all of the source files
ADD_EXECUTABLE( test ${SRCS} )

# last thing we have to do is to tell CMake what libraries our executable needs,
# luckily FIND_PACKAGE prepared QT_LIBRARIES variable for us:
TARGET_LINK_LIBRARIES( test pthread ssh2)

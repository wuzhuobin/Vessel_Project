cmake_minimum_required(VERSION 2.8)

IF(COMMAND CMAKE_POLICY)
	   CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

project(View)

find_package(Qt5Core)
find_package(Qt5Widgets)

include_directories(
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}
	)

set(
	VIEW_H
	MainWindow.h
)

set(
	VIEW_SRC
	MainWindow.cpp
)

set(
	VIEW_UI
	MainWindow.ui
)
#Generate moc and ui file
qt5_wrap_cpp(
	VIEW_MOC
	${VIEW_H}
)

qt5_wrap_ui(
	VIEW_UI_H
	${VIEW_UI}
)

add_library(
	View
	${VIEW_H}
	${VIEW_SRC}
	${VIEW_UI}
	${VIEW_MOC}
	${VIEW_UI_H}
)

#link libraries
target_link_libraries(
	View
	Qt5::Core
	Qt5::Widgets
	${LINK_LIBRARIE}
	)

#Grouping in MSVC
SOURCE_GROUP(
	"Generated Files"
	FILES
	${VIEW_MOC}
	${VIEW_UI_H}
	)
SOURCE_GROUP(
	"Forms FIles" 	
	FILES 
	${VIEW_UI}
	)
SOURCE_GROUP(
	"Header Files" 
	FILES 
	${VIEW_H}
	)
SOURCE_GROUP(
	"Source Files" 
	FILES 
	${VIEW_SRC}
	)
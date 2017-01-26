project(Controller)
#VTK
find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
	message(FATAL_ERROR "error: VTK is not found")
endif()
include(${VTK_USE_FILE})
#ITK
find_package(ITK REQUIRED)
if (NOT ITK_FOUND)
	message(FATAL_ERROR "error: ITK is not found")
endif()
include(${ITK_USE_FILE})
find_package(Qt5Core)
find_package(Qt5Widgets REQUIRED)
if(NOT Qt5Widgets_FOUND)
	message(FATAL_ERROR "error: Qt5Widgets is not found")
endif()

include_directories(
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}
	InteractorStyles
	)

add_subdirectory(InteractorStyles)

SET(
	CONTROLLER_SRC
	# ${PROJECT_SOURCE_DIR}/MyImageManager.cpp
	${PROJECT_SOURCE_DIR}/IOManager.cpp
	${PROJECT_SOURCE_DIR}/ImageRegistration.cxx
	${PROJECT_SOURCE_DIR}/ImageViewer.cxx
	${PROJECT_SOURCE_DIR}/InteractorStyleSwitch.cxx
	# ${PROJECT_SOURCE_DIR}/InteractorStyleSwitch3D.cxx
	# ${PROJECT_SOURCE_DIR}/Overlay.cxx
	# ${PROJECT_SOURCE_DIR}/RegistrationWizard.cxx
	)
SET(
	CONTROLLER_H
	# ${PROJECT_SOURCE_DIR}/MyImageManager.h
	${PROJECT_SOURCE_DIR}/IOManager.h
	${PROJECT_SOURCE_DIR}/ImageRegistration.h
	${PROJECT_SOURCE_DIR}/ImageViewer.h
	${PROJECT_SOURCE_DIR}/InteractorStyleSwitch.h
	# ${PROJECT_SOURCE_DIR}/InteractorStyleSwitch3D.h
	${PROJECT_SOURCE_DIR}/StyleSwitchMacro.h
	# ${PROJECT_SOURCE_DIR}/Overlay.h
	# ${PROJECT_SOURCE_DIR}/RegistrationWizard.h
	)

qt5_wrap_cpp(
	CONTROLLER_MOC 
	${CONTROLLER_H}
	)

add_library(
	Controller
	SHARED
	${CONTROLLER_SRC}
	${CONTROLLER_H}
	${CONTROLLER_MOC}
	)

target_link_libraries(
	Controller
	${ITK_LIBRARIES}
	${VTK_LIBRARIES}
	Qt5::Core
	Qt5::Widgets
	InteractorStyles
)

set_target_properties(
    Controller
    PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS 
    true
)

source_group(
	"Generated FIles" 
	FILES
	${CONTROLLER_MOC}
)
source_group(
	"Header Files" 
	FILES 
	${CONTROLLER_H}
	)
source_group(
	"Source Files"
	FILES
	${CONTROLLER_SRC}
	)
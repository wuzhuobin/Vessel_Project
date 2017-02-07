cmake_minimum_required(VERSION 2.8)

IF(COMMAND CMAKE_POLICY)
	   CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

project(IADE_Analyzer)
#ITK
find_package(ITK REQUIRED)
if (NOT ITK_FOUND)
	message(FATAL_ERROR "error: ITK is not found")
endif()
include(${ITK_USE_FILE})

#VTK
find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
	message(FATAL_ERROR "error: VTK is not found")
endif()
include(${VTK_USE_FILE})

#QT5
find_package(Qt5Core)
find_package(Qt5Widgets)


# #VMTK
# find_package(VMTK REQUIRED)
# if (NOT VMTK_FOUND)
# 	message(FATAL_ERROR "error: VMTK is not found")
# endif()
# include(${VMTK_USE_FILE})

#QT
# if(${VTK_VERSION} VERSION_GREATER "6" AND VTK_QT_VERSION VERSION_GREATER "4")
#   # Instruct CMake to run moc automatically when needed.
#   # set(CMAKE_AUTOMOC ON)
# #   find_package(Qt5PrintSupport REQUIRED)
#   find_package(Qt5Widgets REQUIRED QUIET)
#   # add_definitions(-DQT5_IS_LOADED)
# else()
#   find_package(Qt4 REQUIRED)
#   include(${QT_USE_FILE})
# endif()

# multi-processor build
set(
	CMAKE_CXX_FLAGS 
	"${CMAKE_CXX_FLAGS} /MP"
)
set(
	CMAKE_ARCHIVE_OUTPUT_DIRECTORY
	${CMAKE_SOURCE_DIR}/lib
)

set(
	CMAKE_LIBRARY_OUTPUT_DIRECTORY
	${CMAKE_SOURCE_DIR}/lib
)
set(
	CMAKE_RUNTIME_OUTPUT_DIRECTORY 
	${CMAKE_SOURCE_DIR}/bin
)

set(
	MAIN_LINK_DIRECTORIES
	${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
	${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

set(
	MAIN_INCLUDE_DIRECTORIES
	### Directories for MOC Sources ###
	${CMAKE_BINARY_DIR}
	${CMAKE_BINARY_DIR}/codes/Main
	${CMAKE_BINARY_DIR}/codes/Core
	${CMAKE_BINARY_DIR}/codes/View
	${CMAKE_BINARY_DIR}/codes/Controller
	${CMAKE_BINARY_DIR}/codes/Model

	### Directories for headers ###
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/codes/Main
	${CMAKE_SOURCE_DIR}/codes/Core
	${CMAKE_SOURCE_DIR}/codes/View
	${CMAKE_SOURCE_DIR}/codes/Controller
	${CMAKE_SOURCE_DIR}/codes/Model
	
	)

set(
	MAIN_LINK_LIBRARIES
	${ITK_LIBRARIES}  
	${VTK_LIBRARIES}
	# ${VMTK_LIBRARIES} 
	Qt5::Core
	Qt5::Widgets 
	# Qt5::PrintSupport
	View
	Core
	Controller
	Model
)

include_directories(
	${MAIN_INCLUDE_DIRECTORIES}
)

# link_directories(
# 	${MAIN_LINK_DIRECTORIES}
# 	)

add_subdirectory(codes/Main)
add_subdirectory(codes/View)
add_subdirectory(codes/Core)
add_subdirectory(codes/Controller)
add_subdirectory(codes/Model)
add_subdirectory(codes/LumenExtraction)




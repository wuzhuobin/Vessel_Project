
project(Measurements)


#ITK
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

#VTK
find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
	message(FATAL_ERROR "error: VTK is not found")
endif()
include(${VTK_USE_FILE})

set(
	MEASUREMENTFOR3D_SRC
	${PROJECT_SOURCE_DIR}/MeasurementFor3D.cxx
	${PROJECT_SOURCE_DIR}/MeasurementFor2D.cxx
)

set(
	MEASUREMENTFOR3D_INC
	${PROJECT_SOURCE_DIR}/MeasurementFor3D.h
	${PROJECT_SOURCE_DIR}/MeasurementFor2D.h
)



add_library(
	Measurements 
	${MEASUREMENTFOR3D_INC}
	${MEASUREMENTFOR3D_SRC}
	)

#link libraries
target_link_libraries(
	Measurements
	${VTK_LIBRARIES}
	)

target_include_directories(
	Measurements
	PUBLIC
	${PROJECT_SOURCE_DIR}
	${PROJECT_BINARY_DIR}
)

#Grouping in MSVC
SOURCE_GROUP(
	"Header Files" 
	FILES 
	${MEASUREMENTFOR3D_INC}
	)
SOURCE_GROUP(
	"Source Files" 
	FILES 
	${MEASUREMENTFOR3D_SRC}
	)


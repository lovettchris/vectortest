cmake_minimum_required(VERSION 3.2.2)
project(VectorMultiplyTest)


IF(UNIX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ")
ELSE()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_CONSOLE ")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
ENDIF()


include_directories(
    ../../OpenBLAS
)

add_executable(${PROJECT_NAME} 
			VectorMultiplyTest.cpp
			)
			
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} c++abi ../../OpenBLAS/libopenblas.a )
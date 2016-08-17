cmake_minimum_required(VERSION 2.8)
project(SURVIVOR_ant)

#include_directories (../lib/bamtools-2.3.0/src)
include_directories(../lib/tclap-1.2.1/include)

configure_file( Version.h.in ${CMAKE_SOURCE_DIR}/src/Version.h )

add_executable(survivor_ant 	
                    tree/IntervallTree.cpp
     				SURVIVOR_ant.cpp
     				Parser/BEDParser.cpp
     				Parser/GFFParser.cpp
     				Parser/VCFParser.cpp
					print/VCFPrinter.cpp
					)										
					
#target_link_libraries(ngm-core pthread)	
#TARGET_LINK_LIBRARIES(survivor_ant BamTools-static)
TARGET_LINK_LIBRARIES(survivor_ant zlibstatic)
	
add_executable(survivor_ant-debug 	
                    tree/IntervallTree.cpp
     				SURVIVOR_ant.cpp
     				Parser/BEDParser.cpp
     				Parser/GFFParser.cpp
     				Parser/VCFParser.cpp
					print/VCFPrinter.cpp
					)	

SET_TARGET_PROPERTIES(survivor_ant-debug PROPERTIES COMPILE_FLAGS "-g3 -O0")

#target_link_libraries(sniffles-debug pthread)
# TARGET_LINK_LIBRARIES(survivor_ant-debug BamTools-static)
TARGET_LINK_LIBRARIES(survivor_ant-debug zlibstatic)


#https://github.com/meemknight/cmakeSetup
#Version 1.1.0

cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)

set(PRODUCTION_BUILD ON CACHE BOOL "Make this a production build" FORCE)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

#may give problems on linux
if(PRODUCTION_BUILD)
	set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) #link time optimization
else()
	set(CMAKE_INTERPROCEDURAL_OPTIMIZATION FALSE)
endif()


project(Paradox)

add_subdirectory(thirdparty/SDL2)
add_subdirectory(thirdparty/SDL2_image)
add_subdirectory(thirdparty/SDL2_ttf)


file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable("${CMAKE_PROJECT_NAME}" src/main.cpp)

if(PRODUCTION_BUILD)
	# setup the ASSETS_PATH macro to be in the root folder of your exe
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DATA_PATH="./data/")
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/")

	# remove the option to debug asserts.
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1) 
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DEVELOPLEMT_BUILD=0) 

else()
	# This is useful to get an ASSETS_PATH in your IDE during development
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DATA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data/") 
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0) 
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DEVELOPLEMT_BUILD=1) 

endif()

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${MY_SOURCES})

if(MSVC) # If using the VS compiler...
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)

	set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup") #no console
endif()

target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/Headers/")


target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE SDL2main SDL2 SDL2_image SDL2_ttf)

if(PRODUCTION_BUILD)
	set(DATA_DIR
		"${CMAKE_CURRENT_SOURCE_DIR}/data"
		"${CMAKE_CURRENT_SOURCE_DIR}/resources"
	)

	foreach(ddir ${DATA_DIR})
		get_filename_component(dirname "${ddir}" NAME)
		add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        		COMMAND ${CMAKE_COMMAND} -E copy_directory
            		"${ddir}"
            		"$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/${dirname}"
        		COMMENT "Copying directory to output folder..."
    		)
	endforeach()
	set(SDL_DLLS
    		"${CMAKE_CURRENT_SOURCE_DIR}/build/thirdparty/SDL2/Release/SDL2.dll"
    		"${CMAKE_CURRENT_SOURCE_DIR}/build/thirdparty/SDL2_image/Release/SDL2_image.dll"
    		"${CMAKE_CURRENT_SOURCE_DIR}/build/thirdparty/SDL2_ttf/Release/SDL2_ttf.dll"
	)
else()
	set(SDL_DLLS
    		"${CMAKE_CURRENT_SOURCE_DIR}/build/thirdparty/SDL2/Debug/SDL2d.dll"
    		"${CMAKE_CURRENT_SOURCE_DIR}/build/thirdparty/SDL2_image/Debug/SDL2_imaged.dll"
    		"${CMAKE_CURRENT_SOURCE_DIR}/build/thirdparty/SDL2_ttf/Debug/SDL2_ttfd.dll"
	)
endif()

foreach(dll ${SDL_DLLS})
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${dll}"
            "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>"
    )
endforeach()
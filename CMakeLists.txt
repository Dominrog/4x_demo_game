cmake_minimum_required(VERSION 3.15)
Project(demogame)

set(CMAKE_CXX_STANDARD 17)


set(SRC_FILES
	src/main.cpp
	src/glad.c
	src/utils.cpp
	src/block.cpp
	src/camera.cpp
	src/planet.cpp
	src/shader.cpp
	src/star.cpp
	src/selectionring.cpp
)

file(GLOB HEADER_FILES
	include/*.h
)


add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/extern/glm-1.0.1
)

if(WIN32)
	set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/extern/glfw-3.4/include")
 	set(GLFW_LIBRARY "${CMAKE_SOURCE_DIR}/extern/glfw-3.4/build/src/libglfw3dll.a")
    	set(GLFW_DLL "${CMAKE_SOURCE_DIR}/extern/glfw-3.4/build/src/glfw3.dll")

    	target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIR})
    	target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW_LIBRARY} opengl32 gdi32)

    	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        	COMMAND ${CMAKE_COMMAND} -E copy_if_different
        	${GLFW_DLL}
        	$<TARGET_FILE_DIR:${PROJECT_NAME}>
    	)
elseif(UNIX)
    	find_package(OpenGL REQUIRED)
    	find_package(PkgConfig REQUIRED)
    	pkg_check_modules(GLFW REQUIRED glfw3)
    	find_package(Freetype REQUIRED)

    	target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS})
    	target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL ${GLFW_LIBRARIES} ${FREETYPE_LIBRARIES} dl pthread X11)
endif()

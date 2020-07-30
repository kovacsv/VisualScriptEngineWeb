cmake_minimum_required (VERSION 3.6)

function (SetCompilerOptions module)
	target_compile_options (${module} PUBLIC "$<$<CONFIG:Debug>:-DDEBUG>")
	if (WIN32)
		target_compile_options (${module} PUBLIC /W4 /WX)
	else ()
		target_compile_options (${module} PUBLIC -std=c++11 -Wall -Wextra -Werror)
	endif ()
endfunction ()

set_property (GLOBAL PROPERTY USE_FOLDERS ON)

set (CMAKE_SUPPRESS_REGENERATION 1)
set (CMAKE_CONFIGURATION_TYPES Debug;Release;RelWithDebInfo)
set (CMAKE_DEBUG_POSTFIX "Debug")

add_definitions (-DUNICODE -D_UNICODE)

project (EmscriptenTest)

set (LibSourcesFolder Libs)
set (SDLLibFolder ${LibSourcesFolder}/SDL2-2.0.12)
set (SDLTTFLibFolder ${LibSourcesFolder}/SDL2_ttf-2.0.15)
link_directories (${SDLLibFolder}/lib/x64)
link_directories (${SDLTTFLibFolder}/lib/x64)

# DemoApp

set (DemoAppSourcesFolder Sources/DemoApp)
file (GLOB DemoAppHeaderFiles ${DemoAppSourcesFolder}/*.hpp)
file (GLOB DemoAppSourceFiles ${DemoAppSourcesFolder}/*.cpp)
set (
	DemoAppFiles
	${DemoAppHeaderFiles}
	${DemoAppSourceFiles}
)
source_group ("Sources" FILES ${DemoAppFiles})
add_executable (DemoApp ${DemoAppFiles})
set_target_properties (DemoApp PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
if (${EMSCRIPTEN})
	target_compile_options (DemoApp PUBLIC -sUSE_SDL=2 -sUSE_SDL_TTF=2)
	target_link_options (DemoApp PUBLIC -sUSE_SDL=2 -sUSE_SDL_TTF=2 --preload-file Assets)
else ()
	set_target_properties (DemoApp PROPERTIES LINK_FLAGS /SUBSYSTEM:WINDOWS)
	target_include_directories (DemoApp PUBLIC ${SDLLibFolder}/include ${SDLTTFLibFolder}/include)
	target_link_libraries (DemoApp SDL2 SDL2main SDL2_ttf)
	get_filename_component (SDLDllFullPath "${SDLLibFolder}/lib/x64/SDL2.dll" ABSOLUTE)
	get_filename_component (SDLTTFDllFullPath "${SDLTTFLibFolder}/lib/x64/SDL2_ttf.dll" ABSOLUTE)
	get_filename_component (LibFreeTypeDllFullPath "${SDLTTFLibFolder}/lib/x64/libfreetype-6.dll" ABSOLUTE)
	add_custom_command (TARGET DemoApp POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${SDLDllFullPath}" $<TARGET_FILE_DIR:DemoApp>
		COMMAND ${CMAKE_COMMAND} -E copy "${SDLTTFDllFullPath}" $<TARGET_FILE_DIR:DemoApp>
		COMMAND ${CMAKE_COMMAND} -E copy "${LibFreeTypeDllFullPath}" $<TARGET_FILE_DIR:DemoApp>
	)
endif ()
get_filename_component (SourcesFolderAbsolute "${DemoAppSourcesFolder}" ABSOLUTE)
add_custom_command (TARGET DemoApp PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${SourcesFolderAbsolute}/Assets" $<TARGET_FILE_DIR:DemoApp>/Assets
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${SourcesFolderAbsolute}/Assets" ${CMAKE_BINARY_DIR}/Assets
)

SetCompilerOptions (DemoApp)

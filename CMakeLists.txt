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
set (VSE_DEVKIT_DIR $ENV{VSE_DEVKIT_DIR} CACHE PATH "VisualScriptEngine devkit directory.")

add_definitions (-DUNICODE -D_UNICODE)

project (VisualScriptEngineWeb)

set (LibSourcesFolder Libs)
set (SDLLibFolder ${LibSourcesFolder}/SDL2-2.0.12)
set (SDLTTFLibFolder ${LibSourcesFolder}/SDL2_ttf-2.0.15)
link_directories (${SDLLibFolder}/lib/x64)
link_directories (${SDLTTFLibFolder}/lib/x64)

# WebApp

set (WebAppSourcesFolder Sources/WebApp)
file (GLOB
	WebAppClientFiles
	${WebAppSourcesFolder}/*.hpp
	${WebAppSourcesFolder}/*.cpp
)
file (GLOB
	VisualScriptEngineFiles
	${VSE_DEVKIT_DIR}/include/NE_*.hpp
	${VSE_DEVKIT_DIR}/source/NE_*.cpp
	${VSE_DEVKIT_DIR}/include/NUIE_*.hpp
	${VSE_DEVKIT_DIR}/source/NUIE_*.cpp
	${VSE_DEVKIT_DIR}/include/BI_*.hpp
	${VSE_DEVKIT_DIR}/source/BI_*.cpp
)
set (
	WebAppFiles
	${WebAppClientFiles}
	${VisualScriptEngineFiles}
)
source_group ("Sources" FILES ${WebAppFiles})
source_group ("VisualScriptEngine" FILES ${VisualScriptEngineFiles})
add_executable (WebApp ${WebAppFiles})
set_target_properties (WebApp PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
target_include_directories (WebApp PUBLIC ${VSE_DEVKIT_DIR}/include)
if (${EMSCRIPTEN})
	set (SDLUsedLibraries -sUSE_SDL=2 -sUSE_SDL_TTF=2)
	target_compile_options (WebApp PUBLIC ${SDLUsedLibraries})
	target_link_options (WebApp PUBLIC ${SDLUsedLibraries})
	target_link_options (WebApp PUBLIC "-sEXPORTED_FUNCTIONS=[\"_main\", \"_CreateNode\", \"_ContextMenuResponse\"]")
	target_link_options (WebApp PUBLIC "-sEXTRA_EXPORTED_RUNTIME_METHODS=[\"ccall\", \"cwrap\"]")
	target_link_options (WebApp PUBLIC --preload-file Assets)
	add_definitions (-DSIZEOFSIZET=4)
else ()
	set_target_properties (WebApp PROPERTIES LINK_FLAGS /SUBSYSTEM:WINDOWS)
	target_include_directories (WebApp PUBLIC ${SDLLibFolder}/include ${SDLTTFLibFolder}/include)
	target_link_libraries (WebApp SDL2 SDL2main SDL2_ttf)
	get_filename_component (SDLDllFullPath "${SDLLibFolder}/lib/x64/SDL2.dll" ABSOLUTE)
	get_filename_component (SDLTTFDllFullPath "${SDLTTFLibFolder}/lib/x64/SDL2_ttf.dll" ABSOLUTE)
	get_filename_component (LibFreeTypeDllFullPath "${SDLTTFLibFolder}/lib/x64/libfreetype-6.dll" ABSOLUTE)
	add_custom_command (TARGET WebApp POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${SDLDllFullPath}" $<TARGET_FILE_DIR:WebApp>
		COMMAND ${CMAKE_COMMAND} -E copy "${SDLTTFDllFullPath}" $<TARGET_FILE_DIR:WebApp>
		COMMAND ${CMAKE_COMMAND} -E copy "${LibFreeTypeDllFullPath}" $<TARGET_FILE_DIR:WebApp>
	)
endif ()
get_filename_component (SourcesFolderAbsolute "${WebAppSourcesFolder}" ABSOLUTE)
add_custom_command (TARGET WebApp PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${SourcesFolderAbsolute}/Assets" $<TARGET_FILE_DIR:WebApp>/Assets
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${SourcesFolderAbsolute}/Assets" ${CMAKE_BINARY_DIR}/Assets
)

SetCompilerOptions (WebApp)

cmake_minimum_required (VERSION 3.6)

set_property (GLOBAL PROPERTY USE_FOLDERS ON)

set (CMAKE_SUPPRESS_REGENERATION 1)
set (CMAKE_CONFIGURATION_TYPES Debug;Release;RelWithDebInfo)
set (CMAKE_DEBUG_POSTFIX "Debug")

add_definitions (-DUNICODE -D_UNICODE)

project (VisualScriptEngineWeb)

set (LibSourcesFolder Libs)
set (SDLLibFolder ${LibSourcesFolder}/SDL2-2.0.12)
set (SDLTTFLibFolder ${LibSourcesFolder}/SDL2_ttf-2.0.15)
set (SDLImageLibFolder ${LibSourcesFolder}/SDL2_image-2.0.5)
set (SDLGfxLibFolder ${LibSourcesFolder}/SDL2_gfx-bbcsdl)
set (RapidJsonLibFolder ${LibSourcesFolder}/rapidjson-1.1.0)
set (VSELibFolder ${LibSourcesFolder}/VisualScriptEngine)
link_directories (${SDLLibFolder}/lib/x64)
link_directories (${SDLTTFLibFolder}/lib/x64)
link_directories (${SDLImageLibFolder}/lib/x64)

# WebApp

set (WebAppSourcesFolder Sources/WebApp)
set (WebAppFrameworkSourcesFolder Sources/WebApp/AppFramework)
set (WebSiteSourcesFolder Sources/WebSite)
file (GLOB
	WebAppFrameworkFiles
	${WebAppFrameworkSourcesFolder}/*.hpp
	${WebAppFrameworkSourcesFolder}/*.cpp
)
file (GLOB
	WebAppClientFiles
	${WebAppSourcesFolder}/*.hpp
	${WebAppSourcesFolder}/*.cpp
)
file (GLOB
	SDLGfxFiles
	${SDLGfxLibFolder}/*.h
	${SDLGfxLibFolder}/*.c
)
file (GLOB
	VisualScriptEngineFiles
	${VSELibFolder}/*.hpp
	${VSELibFolder}/*.cpp
)
set (
	WebAppFiles
	${WebAppFrameworkFiles}
	${WebAppClientFiles}
	${SDLGfxFiles}
	${VisualScriptEngineFiles}
)

source_group ("AppFramework" FILES ${WebAppFrameworkFiles})
source_group ("SDLGfx" FILES ${SDLGfxFiles})
source_group ("VisualScriptEngine" FILES ${VisualScriptEngineFiles})
add_executable (WebApp ${WebAppFiles})
set_target_properties (WebApp PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIG>")
target_precompile_headers (WebApp PUBLIC ${WebAppFrameworkSourcesFolder}/Precompiled.hpp)
target_include_directories (WebApp PUBLIC ${WebAppFrameworkSourcesFolder})
target_include_directories (WebApp PUBLIC ${SDLGfxLibFolder})
target_include_directories (WebApp PUBLIC ${VSELibFolder})
target_include_directories (WebApp PUBLIC ${RapidJsonLibFolder}/include)

get_filename_component (WebAppFrameworkSourcesFolderAbsolute "${WebAppFrameworkSourcesFolder}" ABSOLUTE)
if (${EMSCRIPTEN})
	add_custom_command (TARGET WebApp PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${WebAppFrameworkSourcesFolderAbsolute}/Assets" ${CMAKE_BINARY_DIR}/Assets
	)
	set (SDLUsedLibraries -sUSE_SDL=2 -sUSE_SDL_TTF=2 -sUSE_SDL_IMAGE=2)
	target_compile_options (WebApp PUBLIC ${SDLUsedLibraries})
	target_link_options (WebApp PUBLIC ${SDLUsedLibraries})
	target_link_options (WebApp PUBLIC "-sEXPORTED_FUNCTIONS=[\"_main\", \"_ResizeWindow\", \"_ExecuteCommand\", \"_OpenFile\", \"_CreateNode\", \"_ContextMenuResponse\", \"_ParameterSettingsResponse\", \"_NeedToSave\"]")
	target_link_options (WebApp PUBLIC "-sEXTRA_EXPORTED_RUNTIME_METHODS=[\"ccall\", \"cwrap\"]")
	target_link_options (WebApp PUBLIC -sALLOW_MEMORY_GROWTH=1 --no-heap-copy --use-preload-plugins --preload-file Assets)
	add_definitions (-DSIZEOFSIZET=4)
else ()
	add_custom_command (TARGET WebApp PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${WebAppFrameworkSourcesFolderAbsolute}/Assets" $<TARGET_FILE_DIR:WebApp>/Assets
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${WebAppFrameworkSourcesFolderAbsolute}/Assets" ${CMAKE_BINARY_DIR}/Assets
	)
	set_target_properties (WebApp PROPERTIES LINK_FLAGS /SUBSYSTEM:WINDOWS)
	target_include_directories (
		WebApp PUBLIC
		${SDLLibFolder}/include
		${SDLTTFLibFolder}/include
		${SDLImageLibFolder}/include
	)
	target_link_libraries (WebApp SDL2 SDL2main SDL2_ttf SDL2_image)
	get_filename_component (SDLLibFolderFullPath "${SDLLibFolder}" ABSOLUTE)
	get_filename_component (SDLTTFLibFolderFullPath "${SDLTTFLibFolder}" ABSOLUTE)
	get_filename_component (SDLImageLibFolderFullPath "${SDLImageLibFolder}" ABSOLUTE)
	add_custom_command (TARGET WebApp POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${SDLLibFolderFullPath}/lib/x64/SDL2.dll" $<TARGET_FILE_DIR:WebApp>
		COMMAND ${CMAKE_COMMAND} -E copy "${SDLTTFLibFolderFullPath}/lib/x64/SDL2_ttf.dll" $<TARGET_FILE_DIR:WebApp>
		COMMAND ${CMAKE_COMMAND} -E copy "${SDLTTFLibFolderFullPath}/lib/x64/libfreetype-6.dll" $<TARGET_FILE_DIR:WebApp>
		COMMAND ${CMAKE_COMMAND} -E copy "${SDLImageLibFolderFullPath}/lib/x64/SDL2_image.dll" $<TARGET_FILE_DIR:WebApp>
		COMMAND ${CMAKE_COMMAND} -E copy "${SDLImageLibFolderFullPath}/lib/x64/libjpeg-9.dll" $<TARGET_FILE_DIR:WebApp>
		COMMAND ${CMAKE_COMMAND} -E copy "${SDLImageLibFolderFullPath}/lib/x64/libpng16-16.dll" $<TARGET_FILE_DIR:WebApp>
		COMMAND ${CMAKE_COMMAND} -E copy "${SDLImageLibFolderFullPath}/lib/x64/libtiff-5.dll" $<TARGET_FILE_DIR:WebApp>
		COMMAND ${CMAKE_COMMAND} -E copy "${SDLImageLibFolderFullPath}/lib/x64/libwebp-7.dll" $<TARGET_FILE_DIR:WebApp>
		COMMAND ${CMAKE_COMMAND} -E copy "${SDLImageLibFolderFullPath}/lib/x64/zlib1.dll" $<TARGET_FILE_DIR:WebApp>
	)
endif ()
target_compile_options (WebApp PUBLIC "$<$<CONFIG:Debug>:-DDEBUG>")
if (WIN32)
	target_compile_options (WebApp PUBLIC /W4 /WX)
	set_source_files_properties (${SDLGfxFiles} PROPERTIES COMPILE_FLAGS "/wd4028 /wd4100 /wd4211 /wd4244 /wd4267 /wd4305 /wd4310 /wd4456 /wd4701")
else ()
	target_compile_options (WebApp PUBLIC -Wall -Wextra -Werror)
	set_source_files_properties (${SDLGfxFiles} PROPERTIES COMPILE_FLAGS "-Wno-unused-parameter")
endif ()

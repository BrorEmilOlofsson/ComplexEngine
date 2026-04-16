
-- In future if this file gets too big I may split them into multiple premake in each project. For now they are very small and follow a pattern which is easier to follow for beginner

workspace "FootballGame" -- Workspace, is not same as Project. Anything configurated here is in Global scope (affects all projects)

	startproject "Launcher" -- Start up Project
	architecture "x64" -- 64-bit architecture
	language "C++" -- Global language across all Projects in this workspace
	cppdialect "C++23" -- Global C++ Version
	cdialect "C17"
	warnings "Extra" -- Global Highest Warning Level
	objdir "Temp" -- Location for garbage created by Visual Studio

	local bin_dir = path.join(_SCRIPT_DIR, "Bin/") -- Returns absolute path to where this premake file is plus "Bin/" example -> C:\Users\panda\Desktop\SimpleEngine\Bin\
	local shader_dir = path.join(_SCRIPT_DIR, "Bin/Shaders/") 
	local settings_dir = path.join(_SCRIPT_DIR, "Bin/Settings/")
	local minidump_dir = path.join(_SCRIPT_DIR, "Temp/Minidumps/")
	local profiler_dir = path.join(_SCRIPT_DIR, "Temp/Profilers/")
	local stacktraces_dir = path.join(_SCRIPT_DIR, "Temp/StackTraces/")

	os.mkdir(bin_dir)  -- Create these folders on call "generate_project.bat" (current .bat file name)
	os.mkdir(shader_dir)
	os.mkdir(settings_dir)
	os.mkdir(profiler_dir)
	os.mkdir(minidump_dir)
	os.mkdir(stacktraces_dir)
	
	dirs = {} -- Absolute path to specific folder
	dirs["root"]				= os.realpath("/")
	dirs["Bin"]					= os.realpath("Bin/")
	dirs["Local"]				= os.realpath("Local/")
	dirs["Dependencies"]		= os.realpath("Dependencies/")
	dirs["Lib"]					= os.realpath("Dependencies/Lib/")
	dirs["DLL"]					= os.realpath("Dependencies/DLL/")
	dirs["SimpleLib"]			= os.realpath("Bin/")
	dirs["Bin_Settings"]		= os.realpath("Bin/Settings/")
	dirs["Build"]               = os.realpath("Bin/Test/")
	
	debugdir(dirs.Bin)

	configurations { -- Create Configurations For This WorkSpace
		"Debug_Editor",
		"Release_Editor",
		"Debug_Game",
		"Release_Game"
	}

	defines { -- Create Global Macros For Strings.

		'SIMPLE_DIR_ROOT="' .."../" .. '"',

		'SIMPLE_DIR_DEPENDENCIES="' .."../Dependencies/" .. '"',
		'SIMPLE_DIR_DEPENDENCIES_FORCE="' .."../Dependencies/Settings/Always_Force_Copy/" .. '"',
		'SIMPLE_DIR_DEPENDENCIES_SETTINGS="' .."../Dependencies/Settings/" .. '"',
		'SIMPLE_DIR_SETTINGS="' .."Settings/" .. '"',
		'SIMPLE_DIR_SHADERS="' .. "Shaders/" .. '"',

		'SIMPLE_SETTINGS_IMGUI="' .."Settings/imgui.ini" .. '"',
		'SIMPLE_SETTINGS_GAME="' .. "Settings/game_settings.json" .. '"',
		'SIMPLE_SETTINGS_EDITOR="' .. "Settings/editor_settings.json" .. '"',
		'SIMPLE_SETTINGS_DEBUG="' .. "Settings/debug_settings.json" .. '"',
		'SIMPLE_SETTINGS_ENGINE="' .. "Settings/engine_settings.json" .. '"',
		
		'SIMPLE_DIR_MINIDUMP="' .."../Temp/Minidumps/".. '"',
		'SIMPLE_FILENAME_STACKTRACES="' .."../Temp/StackTraces/stack_traces.txt".. '"',
		'SIMPLE_FILENAME_PROFILER="' .. "../Temp/Profilers/profiler_data.prof" .. '"',
		'SIMPLE_FILENAME_NEWSCENE="NewScene.scene"',
		'SIMPLE_FILENAME_DEBUGJSON="debug_settings.json"',
		"IMGUI_DEFINE_MATH_OPERATORS"
	}

	filter { "configurations:*Game" }
		defines {
			'SIMPLE_DIR_ASSETS="' .. "Assets/" .. '"',
			'SIMPLE_DIR_TEXTURES="' .. "Assets/Textures/" .. '"',
			'SIMPLE_DIR_NAVMESH="' .. "Assets/Navmesh/" .. '"',
			'SIMPLE_DIR_AUDIO="' .. "Assets/Audio/" .. '"',
			'SIMPLE_DIR_MODELS="' .. "Assets/Models/" .. '"',
			'SIMPLE_DIR_SCENES="' .. "Assets/Scenes" .. '"',
		}

	filter { "configurations:*Editor" }
		defines {
			'SIMPLE_DIR_ASSETS="' .. "../Assets/" .. '"',
			'SIMPLE_DIR_TEXTURES="' .. "../Assets/Textures/" .. '"',
			'SIMPLE_DIR_NAVMESH="' .. "../Assets/Navmesh/" .. '"',
			'SIMPLE_DIR_AUDIO="' .. "../Assets/Audio/" .. '"',
			'SIMPLE_DIR_MODELS="' .. "../Assets/Models/" .. '"',
			'SIMPLE_DIR_SCENES="' .. "../Assets/Scenes" .. '"',
		}

	filter "configurations:Debug_Editor" -- Global Configurations Settings
		defines { "_DEBUG_EDITOR", "_DEBUG", "_EDITOR",  }
		symbols "On"
		optimize "Off"
		runtime "Debug"
		libdirs { dirs.Lib }

	filter "configurations:Release_Editor"
		defines { "_RELEASE_EDITOR", "NDEBUG", "_EDITOR" }
		symbols "On"
		optimize "Speed"
		runtime "Release"
		libdirs { dirs.Lib }

	filter "configurations:Debug_Game"
		defines { "_DEBUG_GAME", "_DEBUG" }
		symbols "On"
		optimize "Off"
		runtime "Debug"
		libdirs { dirs.Lib }

	filter "configurations:Release_Game"
		defines { "_RELEASE_GAME", "NDEBUG" }
		symbols "Off"
		optimize "Full"
		runtime "Release"
		libdirs { dirs.Lib }

    filter "configurations:Debug_*"
        defines { "BUILD_WITH_EASY_PROFILER" }

	filter "system:windows"
		systemversion "latest"
		
		flags {
			"MultiProcessorCompile"
		}

	filter "action:vs*"
		buildoptions { 
			"/utf-8" 
		}
  
	--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	project "External"
		kind "StaticLib"
		location  (dirs.Local)
		targetdir (dirs.SimpleLib)
		targetname("%{prj.name}_%{cfg.buildcfg}")
		cppdialect "C++20"
		fatalwarnings { "All" }
		--flags { "FatalCompileWarnings" } -- Treat CompileWarnings as error but not Lib/Link warnings as error

		defines {
			"FBXSDK_SHARED"  -- This define will only exist withing this "External" project
		}

		includedirs {
			"Source/",
			"Source/External/",
			"Source/External/**",
		}

		files {
			"Source/External/**.h", 
			"Source/External/**.cpp",
			"Source/External/**.hpp" 
		}

		links {
			"Lib/Common/easy_profiler",
			"Lib/Common/assimp-vc143-mt"
		}

		filter "configurations:Debug*" 
			links { 
				"Lib/Debug/libfbxsdk",
				"Lib/Debug/fmodL_vc",
				"Lib/Debug/fmodstudioL_vc",
				"Lib/Debug/DirectXTK",
				"Lib/Debug/fmtd.lib"
			}
			
		filter "configurations:Release*" 
			links { 
				"Lib/Release/libfbxsdk",
				"Lib/Release/fmod_vc",
				"Lib/Release/fmodstudio_vc",
				"Lib/Release/DirectXTK",
				"Lib/Release/fmt.lib"
			}

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	project "NodeScript"
		kind "StaticLib"
		location (dirs.Local)
		targetdir (dirs.SimpleLib)
		targetname("%{prj.name}_%{cfg.buildcfg}") 
		fatalwarnings { "All" }

		pchheader "NodeScript/Precompiled/NodeScriptPch.hpp"
		pchsource "Source/NodeScript/Precompiled/NodeScriptPch.cpp"

		forceincludes { "%{prj.location}/NodeScript/Precompiled/NodeScriptPch.hpp" } -- Force include "NodeScriptPch.hpp" in every .cpp files without having to explicit include it ourselves

		files {
			"Source/NodeScript/**.h", 
			"Source/NodeScript/**.hpp", 
			"Source/NodeScript/**.cpp",
			"Source/NodeScript/**.ixx"
		}

		includedirs { 
			"Source/",
			"Source/NodeScript/",
			"Source/NodeScript/**",
			"Source/External/",
			"Source/External/**"
		}

	--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	-- group "Engine"
	-- 	project "Utility"
	-- 		kind "StaticLib"
	-- 		location (dirs.Local)
	-- 		targetdir (dirs.SimpleLib)
	-- 		targetname("%{prj.name}_%{cfg.buildcfg}") 
	-- 		fatalwarnings { "All" }

	-- 		pchheader "Utility/Precompiled/UtilityPch.hpp" -- Force all Source (.cpp) files in this project to include "Engine/Precomplier/stdafx.h"
	-- 		pchsource "Source/Utility/Precompiled/UtilityPch.cpp" -- Telling this project to use precomplier header

	-- 		files {
	-- 			"Source/Utility/**.h",
	-- 			"Source/Utility/**.hpp",
	-- 			"Source/Utility/**.cpp",
	-- 			"Source/Utility/**.ixx"
	-- 		}

	-- 		includedirs {
	-- 			"Source/",
	-- 			"Source/External/"
	-- 		}

	--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	-- group "Engine"
	-- 	project "Graphics"
	-- 		kind "StaticLib"
	-- 		location (dirs.Local)
	-- 		targetdir (dirs.SimpleLib)
	-- 		targetname("%{prj.name}_%{cfg.buildcfg}") 
	-- 		fatalwarnings { "All" }

	-- 		pchheader "Graphics/Precompiled/GraphicsPch.hpp" -- Force all Source (.cpp) files in this project to include "Engine/Precomplier/stdafx.h"
	-- 		pchsource "Source/Graphics/Precompiled/GraphicsPch.cpp" -- Telling this project to use precomplier header

	-- 		files {
	-- 			"Source/Graphics/**.h",
	-- 			"Source/Graphics/**.hpp",
	-- 			"Source/Graphics/**.cpp",
	-- 			"Source/Graphics/**.hlsl",
	-- 			"Source/Graphics/**.hlsli",
	-- 			"Source/Graphics/**.ixx"
	-- 		}

	-- 		includedirs {
	-- 			"Source/",
	-- 			"Source/External/"
	-- 		}

	-- 		links {
	-- 			"d3d11"
	-- 		}

	--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	project "Engine"
		kind "StaticLib" -- Build type ".lib"
		location (dirs.Local) -- I only know that this is for where vcxproj, vcxproj.filter and vcxproj.user will be generated
		targetdir (dirs.SimpleLib) -- Location of where the build will be. Example -> C:\Users\panda\Desktop\SimpleEngine\Lib
		targetname("%{prj.name}_%{cfg.buildcfg}") -- Name of the Build plus Active Configuration type. Example -> "Engine_Debug"
		-- dependson { "External" } -- Ensure that "External" is built before this project
		fatalwarnings { "All" }

		pchheader "Engine/Precompiled/EnginePch.hpp" -- Force all Source (.cpp) files in this project to include "Engine/Precomplier/stdafx.h"
		pchsource "Source/Engine/Precompiled/EnginePch.cpp" -- Telling this project to use precomplier header

		includedirs { -- Included "Source/" so that we do not need "../../Engine/Graphics/Test.h" and instead can do "Engine/Graphics/Test.h" or just "Graphics/Test.h" or just "Test.h"
			"Source/",
			"Source/External/",
			"Source/External/**"
		} 

		files { -- Files that shown in Visual Studio's Solution
			"Source/Engine/**.h", 
			"Source/Engine/**.hpp", 
			"Source/Engine/**.cpp",
			"Source/Engine/**.ixx"
		}

		links {
			"d3d11",
			"Xinput"
		}

	--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	project "Shader"
		kind "StaticLib"
		location (dirs.Local)
		targetdir (dirs.SimpleLib)
		targetname ("%{prj.name}_%{cfg.buildcfg}")
		fatalwarnings { "All" }

		files {
			"Source/Shader/**.h",
			"Source/Shader/**.hpp",
			"Source/Shader/**.cpp",
			"Source/Shader/**.hlsl",
			"Source/Shader/**.hlsli"
		}

		includedirs {
			"Source/"
		}

		shadermodel("5.0")
		shaderoptions({"/WX"})

		filter("files:**.hlsl")
			local output_dir = path.join(shader_dir, "%{file.basename}.cso") -- Absolute path and name of the files. Example -> C:\Users\panda\Desktop\SimpleEngine\Bin\Shaders\DefaultPS.cso
			shaderobjectfileoutput(output_dir) -- Create .cso files at this location

		filter("files:Source/Shader/PixelShader/**.hlsl")
			shadertype("Pixel") -- Set all .hlsl shadertype in this folder to "Pixel"

		filter("files:Source/Shader/VertexShader/**.hlsl")
			shadertype("Vertex")  -- Set all .hlsl shadertype in this folder to "Vertex"

	--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	project "Game"
		kind "StaticLib"
		location (dirs.Local)
		targetdir (dirs.SimpleLib)
		targetname("%{prj.name}_%{cfg.buildcfg}") 
		fatalwarnings { "All" }

		pchheader "Game/Precompiled/GamePch.hpp"
		pchsource "Source/Game/Precompiled/GamePch.cpp"

		files {
			"Source/Game/**.h", 
			"Source/Game/**.hpp", 
			"Source/Game/**.cpp",
			"Source/Game/**.ixx"
		}

		includedirs { 
			"Source/",
			"Source/External/",
			"Source/External/**"
		}

	--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	project "Editor"
		kind "StaticLib"
		location (dirs.Local)
		targetdir (dirs.SimpleLib)
		targetname("%{prj.name}_%{cfg.buildcfg}")
		fatalwarnings { "All" }

		pchheader "Editor/Precompiled/EditorPch.hpp"
		pchsource "Source/Editor/Precompiled/EditorPch.cpp"

		files {
			"Source/Editor/**.h",
			"Source/Editor/**.hpp",
			"Source/Editor/**.cpp",
			"Source/Editor/**.ixx"
		}

		includedirs {
			"Source/",
			"Source/External/",
			"Source/External/**"
		}

	--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	project "Launcher"
		kind "WindowedApp"
		location (dirs.Local)
		targetdir (dirs.Bin)
		targetname "SimpleEngine_%{cfg.buildcfg}"
		fatalwarnings { "All" }

		--dependson {
		--	"External", 
		--	"Engine", 
		--	"GraphicsEngine",
		--	"Game"  
		--}
	
		files {
			"Source/Launcher/**.h", 
			"Source/Launcher/**.hpp", 
			"Source/Launcher/**.cpp", 
			"Source/Launcher/**.rc"
		}

		includedirs { 
			"Source/",
			"Source/External/",
			"Source/External/**"
		}

		externalincludedirs {
			"Source/External/"
		}

		links {
			"External", 
			--"Utility",
			"NodeScript",
			--"Graphics", 
			"Engine",
			"Shader",
			"Game",
			"Editor",

			"DbgHelp",
			"ws2_32"
		}

		postbuildcommands { 
			"{COPY} %{wks.location}Dependencies/DLL/Common/*.dll %{cfg.targetdir}", -- Copy files from this folder to targetdir (Bin)
		}

		prebuildcommands {
		}

		filter "configurations:Debug_*"
			postbuildcommands {
				"{COPY} %{wks.location}Dependencies/DLL/Debug/*.dll %{cfg.targetdir}"
			}

			prebuildcommands {
				"{COPY} %{wks.location}Dependencies/Lib/Debug/*.pdb %{cfg.targetdir}"
			}

		filter "configurations:Release_*"
			postbuildcommands {
				"{COPY} %{wks.location}Dependencies/DLL/Release/*.dll %{cfg.targetdir}"
			}

		filter "configurations:*Game"
			postbuildcommands {
				'xcopy /E /Q /Y /I "%{wks.location}Assets" "%{cfg.targetdir}\\Assets" > nul'
			}
			--postbuildcommands {
			--	"{COPY} %{wks.location}Assets %{cfg.targetdir}"
			--}

------------------------------------------------------------------------------------------------------------------------------------------

	group "Testing"
		project "UnitTesting"
			kind "ConsoleApp"
			location (dirs.Local)
			targetdir (dirs.SimpleLib)
			targetname("%{prj.name}_%{cfg.buildcfg}")
			dependson { "External", "Engine" }
			fatalwarnings { "All" }

			includedirs {
				"Source/",
				"Source/Engine/",
				"Source/External/",
			}
		
			--externalincludedirs {
			--	"Source/External/"
			--}

			links {
				"External",
				"NodeScript",
				--"Utility",
				--"Graphics", 
				"Engine",
				"Editor"
			}

			-- Define the test files
			files {
				"Source/UnitTesting/**.h",
				"Source/UnitTesting/**.hpp",
				"Source/UnitTesting/**.cpp"
			}

			postbuildcommands { 
				"{COPY} %{wks.location}/Dependencies/DLL/Common/*.dll %{cfg.targetdir}", -- Copy files from this folder to targetdir (Bin)
			}

			prebuildcommands {
			}

			filter "configurations:Debug"
				postbuildcommands {
					"{COPY} %{wks.location}/Dependencies/DLL/Debug/*.dll %{cfg.targetdir}"
				}

				prebuildcommands {
					"{COPY} %{wks.location}/Dependencies/Lib/Debug/*.pdb %{cfg.targetdir}"
				}

			filter "configurations:Release"
				postbuildcommands {
					"{COPY} %{wks.location}/Dependencies/DLL/Release/*.dll %{cfg.targetdir}"
				}

			filter "configurations:Simple"
				postbuildcommands {
					"{COPY} %{wks.location}/Dependencies/DLL/Release/*.dll %{cfg.targetdir}"
				}

	------------------------------------------------------------------------------------------------------------------------------------------

	group "Testing"
		project "Benchmarking"
			kind "ConsoleApp"
			location (dirs.Local)
			targetdir (dirs.SimpleLib)
			targetname("%{prj.name}_%{cfg.buildcfg}")
			dependson { "External", "Engine" }
			fatalwarnings { "All" }

			includedirs {
				"Source/",
				"Source/Engine/",
				"Source/External/",
			}
		
			--externalincludedirs {
			--	"Source/External/"
			--}

			links {
				"External",
				--"Utility",
				"NodeScript",
				--"Graphics", 
				"Engine"
			}

			-- Define the test files
			files {
				"Source/Benchmarking/**.h",
				"Source/Benchmarking/**.hpp",
				"Source/Benchmarking/**.cpp"
			}

			postbuildcommands { 
				"{COPY} %{wks.location}/Dependencies/DLL/Common/*.dll %{cfg.targetdir}", -- Copy files from this folder to targetdir (Bin)
			}

			prebuildcommands {
			}

			filter "configurations:Debug"
				postbuildcommands {
					"{COPY} %{wks.location}/Dependencies/DLL/Debug/*.dll %{cfg.targetdir}"
				}
				prebuildcommands {
					"{COPY} %{wks.location}/Dependencies/Lib/Debug/*.pdb %{cfg.targetdir}"
				}

			filter "configurations:Release"
				postbuildcommands {
					"{COPY} %{wks.location}/Dependencies/DLL/Release/*.dll %{cfg.targetdir}"
				}

			filter "configurations:Simple"
				postbuildcommands {
					"{COPY} %{wks.location}/Dependencies/DLL/Release/*.dll %{cfg.targetdir}"
				}
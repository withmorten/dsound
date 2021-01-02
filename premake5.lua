workspace "dsound"
	configurations
	{
		"Debug",
		"Release",
	}

	location "build"

project "dsound"
	kind "SharedLib"
	language "C++"
	targetname "dsound"
	targetdir "bin/%{cfg.buildcfg}"
	targetextension ".dll"

	files { "src/dsound.cpp" }
	files { "src/MemoryMgr.h" }
	files { "src/dsound.def" }

	characterset ("MBCS")
	toolset ("v141_xp")
	floatingpoint "Fast"
	buildoptions { "/Zc:sizedDealloc-" }
	links { "legacy_stdio_definitions" }
	linkoptions { "/SAFESEH:NO /IGNORE:4222" }
	defines { "WIN32_LEAN_AND_MEAN", "_CRT_SECURE_NO_WARNINGS", "_CRT_NONSTDC_NO_DEPRECATE", "_USE_32BIT_TIME_T", "NOMINMAX" }

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "full"
		optimize "off"
		runtime "debug"
		editAndContinue "off"
		flags { "NoIncrementalLink" }
		staticruntime "on"

	filter "configurations:Release"
		defines { "NDEBUG" }
		symbols "on"
		optimize "speed"
		runtime "release"
		staticruntime "on"
		flags { "LinkTimeOptimization" }
		linkoptions { "/OPT:NOICF" }

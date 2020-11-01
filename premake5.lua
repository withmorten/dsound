workspace "dsound"
	configurations
	{
		"Debug",
		"Release",
	}

	location "build"

	files { "src/dsound.cpp" }
	files { "src/MemoryMgr.h" }
	files { "src/dsound.def" }

local function debugConfig()
	return
	{
		defines { "DEBUG" },
		symbols "full",
		optimize "off",
		runtime "debug",
		editAndContinue "off",
		flags { "NoIncrementalLink" },
		staticruntime "on",
		libdirs { "lib/xasdk/static/mtd" },
	}
end

local function releaseConfig()
	return
	{
		defines { "NDEBUG" },
		symbols "on",
		optimize "speed",
		runtime "release",
		staticruntime "on",
		flags { "LinkTimeOptimization" },
		linkoptions { "/OPT:NOICF" },
		libdirs { "lib/xasdk/static/mt" },
	}
end

project "dsound"
	kind "SharedLib"
	language "C++"
	targetname "dsound"
	targetdir "bin/%{cfg.buildcfg}"
	targetextension ".dll"

	characterset ("MBCS")
	toolset ("v141_xp")
	floatingpoint "Fast"
	buildoptions { "/Zc:sizedDealloc-" }
	links { "legacy_stdio_definitions" }
	linkoptions { "/SAFESEH:NO /IGNORE:4222" }
	defines { "WIN32_LEAN_AND_MEAN", "_CRT_SECURE_NO_WARNINGS", "_CRT_NONSTDC_NO_DEPRECATE", "_USE_32BIT_TIME_T", "NOMINMAX" }

	filter "configurations:Debug"
		debugConfig()

	filter "configurations:Release"
		releaseConfig()

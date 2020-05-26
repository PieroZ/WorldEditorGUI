workspace "WorldEditorGUI"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["SDL2"] = "vendor/SDL2/include"
IncludeDir["lua"] = "vendor/lua/include"

LibsDir = {}
LibsDir["SDL2"] = "vendor/SDL2/lib/x64"
LibsDir["lua"] = "vendor/lua"

project "WorldEditorGUI"
	location "WorldEditorGUI"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	
	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.SDL2}",
		"%{IncludeDir.lua}"
	}
	
	libdirs
	{
		"%{LibsDir.SDL2}",
		"%{LibsDir.lua}"
	}
	
	links
	{
		"SDL2_mixer.lib",
		"SDL2_ttf.lib",
		"SDL2_image.lib",
		"SDL2.lib",
		"SDL2main.lib",
		"lua53.lib";
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		symbols "On"

	filter "configurations:Release"
		optimize "On"
	
	filter "configurations:Dist"
		symbols "On"
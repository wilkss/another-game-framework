workspace "AnotherGameFramework"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "AnotherGameFramework"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/**.h",
        "src/**.cpp",
        "media/shaders/**.vertex",
        "media/shaders/**.fragment",
        "media/shaders/**.geometry"
    }

    includedirs
    {
        "include",
        "dependencies/glew/include",
        "dependencies/glfw/include",
        "dependencies/glm/include",
        "dependencies/stb/include",
    }

    libdirs
    {
        "dependencies/glew/lib",
        "dependencies/glfw/lib"
    }

    links
    {
        "opengl32",
        "glfw3",
        "glew32s"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "Off"
        systemversion "10.0.17134.0"

		defines
		{
			"AGF_PLATFORM_WINDOWS",
            "AGF_BUILD_DLL",
            "GLEW_STATIC",
            "GLM_FORCE_RADIANS",
            "GLM_FORCE_DEPTH_ZERO_TO_ONE"
        }

    filter "configurations:Debug"
        defines "AGF_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "AGF_RELEASE"
        optimize "On"

project "AnotherGameFrameworkDemo"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        "demo/**.h",
        "demo/**.cpp"
    }

    includedirs
    {
        "include",
        "dependencies/glm/include"
    }

    links
    {
        "AnotherGameFramework"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "Off"
        systemversion "10.0.17134.0"

        defines
        {
            "AGF_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "AGF_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "AGF_RELEASE"
        optimize "On"

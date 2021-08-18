-- Helpers

function os.winSdkVersion()
    local reg_arch = iif( os.is64bit(), "\\Wow6432Node\\", "\\" )
    local sdk_version = os.getWindowsRegistry( "HKLM:SOFTWARE" .. reg_arch .."Microsoft\\Microsoft SDKs\\Windows\\v10.0\\ProductVersion" )
    if sdk_version ~= nil then return sdk_version end
end

--premake

workspace "Engine"
    configurations { "Debug", "Profile", "Release" }
    platforms { "Windows" }

    cppdialect "c++20"

    location ".build"
    characterset "Unicode"

    includedirs { "src" }

    disablewarnings { 
        "4251" -- MSVC C4251 ... needs to have dll-interface to be sued by clients class ... (std::atomic, etc)
    }

    flags { "FatalCompileWarnings", "MultiProcessorCompile" }
    
    filter "configurations:Debug"
        defines { "DEBUG=1" }
        optimize "Off"
        symbols "On"
        targetdir "bin/debug"

    filter "configurations:Profile"
        defines { "DEBUG=1", "PROFILE_=1" }
        optimize "Full"
        symbols "On"
        targetdir "bin/profile"

    filter "configurations:Release"
        defines { "NDEBUG=1", "RELEASE_=1" }
        optimize "Full"
        symbols "Off"
        targetdir "bin/release"

    filter "platforms:Windows"
        system "windows"
        architecture "x86_64"
        toolset (iif(_ACTION == "vs2019", "v142", "v141"))
        systemversion (os.winSdkVersion() .. ".0")
        defines { "PLATFORM_WINDOWS=1" }

project "Core"
    kind "SharedLib"
    language "C++"

    defines { "EXPORT_CORE=1" }
    
    files { "src/core/**.h", "src/core/**.inl", "src/core/**.cpp" }
    location ".build/Engine/Core"
    
project "Start"
    kind "ConsoleApp"
    language "C++"

    files { "src/start/**.h", "src/start/**.inl", "src/start/**.cpp" }
    location ".build/Engine/Start"

    links { "Core" }
    dependson { "Core" }

project "Unittest"
    kind "ConsoleApp"
    language "C++"

    files { "src/unittest/**.h", "src/unittest/**.inl", "src/unittest/**.cpp" }
    location ".build/Engine/Unittest"

    includedirs { "third-party/googletest/googletest/include" }

    links { "Core" }
    dependson { "Core" }

    filter "configurations:Debug"
        libdirs { "third-party/googletest/build/lib/Debug/" }
        links { "gtestd.lib", "gtest_maind.lib" }

    filter "configurations:Profile"
        libdirs { "third-party/googletest/build/lib/Release/" }
        links { "gtest.lib", "gtest_main.lib" }

    filter "configurations:Release"
        libdirs { "third-party/googletest/build/lib/Release/" }
        links { "gtest.lib", "gtest_main.lib" }

project "Benchmark"
    kind "ConsoleApp"
    language "C++"

    files { "src/benchmark/**.h", "src/benchmark/**.inl", "src/benchmark/**.cpp" }
    location ".build/Engine/Benchmark"

    includedirs { "third-party/googlebench/include" }

    links { "Core" }
    dependson { "Core" }

    libdirs { "third-party/googlebench/build/src/Release" }
    links { "benchmark.lib" }

    filter "platforms:Windows"
        links { "Shlwapi.lib" }



-- Helpers

function os.winSdkVersion()
    local reg_arch = iif( os.is64bit(), "\\Wow6432Node\\", "\\" )
    local sdk_version = os.getWindowsRegistry( "HKLM:SOFTWARE" .. reg_arch .."Microsoft\\Microsoft SDKs\\Windows\\v10.0\\ProductVersion" )
    if sdk_version ~= nil then return sdk_version end
end

--premake

newoption {
    trigger = "unittest",
    description = "Compile unittests"
}

newoption {
    trigger = "benchmark",
    description = "Compile benchmarks"
}

workspace "Engine"
    configurations { "Debug", "Profile", "Release" }
    platforms { "Windows" }

    cppdialect "c++20"

    location ".build"
    characterset "Unicode"

    includedirs { "src" }

    disablewarnings { 
        "4251" -- MSVC C4251 ... needs to have dll-interface to be used by clients class ... (std::atomic, etc)
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
        vectorextensions "AVX2"
        toolset (iif(_ACTION == "vs2022", "v143", iif(_ACTION == "vs2019", "v142", "v141")))
        systemversion (os.winSdkVersion() .. ".0")
        defines { "PLATFORM_WINDOWS=1" }


    filter "language:C#"
        configmap {
            ["Profile"] = "Debug"
        }
        removeplatforms { "Windows" }
        removedefines { "DEBUG", "NDEBUG", "RELEASE_" }
        
    include "src/core" 
    include "src/start"

    if _OPTIONS["unittest"] then
        include "src/unittest"
    end

    if _OPTIONS["benchmark"] then
        include "src/benchmark"
    end

-- TOOLS
    include "src/tools"
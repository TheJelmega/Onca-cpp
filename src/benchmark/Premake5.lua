project "Benchmark"
    kind "ConsoleApp"
    language "C++"

    files { "**.h", "**.inl", "**.cpp"  }
    location (_MAIN_SCRIPT_DIR .. "/.build/Engine/Benchmark")
    objdir (_MAIN_SCRIPT_DIR .. "/bin-int/Benchmark")

    includedirs { _MAIN_SCRIPT_DIR .. "/third-party/googlebench/include" }

    links { "Core" }
    dependson { "Core" }

    libdirs { _MAIN_SCRIPT_DIR .. "/third-party/googlebench/build/src/Release" }
    links { "benchmark.lib" }

    filter "configurations:Debug"
        runtime "Release"

    filter "platforms:Windows"
        links { "Shlwapi.lib" }
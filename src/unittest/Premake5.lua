project "Unittest"
    kind "ConsoleApp"
    language "C++"

    files { "**.h", "**.inl", "**.cpp"  }
    location (_MAIN_SCRIPT_DIR .. "/.build/Engine/Unittest")
    objdir (_MAIN_SCRIPT_DIR .. "/bin-int/Unittest")

    includedirs { _MAIN_SCRIPT_DIR .. "/third-party/googletest/googletest/include" }

    links { "Core" }
    dependson { "Core" }

    filter "configurations:Debug"
        libdirs { _MAIN_SCRIPT_DIR .. "/third-party/googletest/build/lib/Debug/" }
        links { "gtestd.lib", "gtest_maind.lib" }

    filter "configurations:Profile"
        libdirs { _MAIN_SCRIPT_DIR .. "/third-party/googletest/build/lib/Release/" }
        links { "gtest.lib", "gtest_main.lib" }

    filter "configurations:Release"
        libdirs { _MAIN_SCRIPT_DIR .. "/third-party/googletest/build/lib/Release/" }
        links { "gtest.lib", "gtest_main.lib" }
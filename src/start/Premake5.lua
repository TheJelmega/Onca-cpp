project "Start"
    kind "ConsoleApp"
    language "C++"

    files { "**.h", "**.inl", "**.cpp"  }
    location (_MAIN_SCRIPT_DIR .. "/.build/Engine/Start")
    objdir (_MAIN_SCRIPT_DIR .. "/bin-int/Start")
    
    debugdir "../../workingdir"

    links { "Core" }
    dependson { "Core" }

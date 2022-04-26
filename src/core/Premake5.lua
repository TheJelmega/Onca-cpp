project "Core"
    kind "SharedLib"
    language "C++"

    defines { "EXPORT_CORE=1" }

    filter { "system:windows" }
        links { "Hid" }
    
    files { "**.h", "**.inl", "**.cpp", "**.natvis"  }
    location (_MAIN_SCRIPT_DIR .. "/.build/Engine/Core")
    objdir (_MAIN_SCRIPT_DIR .. "/bin-int/Core")
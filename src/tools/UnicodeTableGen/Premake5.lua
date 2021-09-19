function CreateLaunchSettings()
    file = io.open(_MAIN_SCRIPT_DIR .. "/.build/Tools/UnicodeTableGen/Properties/launchSettings.json", "w")

    file:write("{\n")
    file:write("\t\"profiles\": {\n")
    file:write("\t\t\"UnicodeTableGen\": {\n")
    file:write("\t\t\t\"commandName\": \"Project\",\n")
    file:write("\t\t\t\"commandLineArgs\": \"\\\"data/tools/unicode/CaseFolding.txt\\\" \\\"src/core/string/StringConstants.generated.h\\\"\",\n")
    file:write("\t\t\t\"workingDirectory\": \"" .. _MAIN_SCRIPT_DIR .. "\"\n")
    file:write("\t\t}\n")
    file:write("\t}\n")
    file:write("}")

    file:close()
end

project "UnicodeTableGen"
    language "C#"
    kind "ConsoleApp"
    dotnetframework "net5.0"

    removedefines { "NDEBUG", "DEBUG", "RELEASE_" }

    configmap {
        ["Profile"] = "Debug"
    }
    removeplatforms "Windows"

    location (_MAIN_SCRIPT_DIR .. "/.build/Tools/UnicodeTableGen")
    objdir (_MAIN_SCRIPT_DIR .. "/bin-int/Tools/UnicodeTableGen")

    files { "**.cs" }

    debugdir "data/"

    CreateLaunchSettings()
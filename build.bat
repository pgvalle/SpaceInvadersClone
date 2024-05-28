@echo off
setlocal enabledelayedexpansion

rem make sure we're executing inside project root
cd %0\..

rem Set the directory where the .cpp files are located
set "source_dir=.\src"

rem Set the compiler command
set "compiler=g++"

rem Set compiler flags
set "flags=-std=c++11 -IC:\Dev\MinGW\x86_64-w64-mingw32\include\SDL2 -I.\deps\NAGE\include -I%source_dir% -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image"

rem Set the output directory for executables
set "output_dir=.\build"

rem Create the output directory if it doesn't exist
if not exist "%output_dir%" mkdir "%output_dir%"

rem Compile each .cpp file recursively
for /r "%source_dir%" %%f in (*.cpp) do (
    rem Extract filename without extension
    set "filename=%%~nf"
    
    rem Check if the source file is newer than the object file
    if exist "%output_dir%\!filename!.obj" (
        for %%A in ("%%f") do set "source_timestamp=%%~tA"
        for %%B in ("%output_dir%\!filename!.obj") do set "object_timestamp=%%~tB"
        if "!source_timestamp!" gtr "!object_timestamp!" (
            rem Compile the file and generate object file in the output directory
            %compiler% -c %flags% "%%f" -o "%output_dir%\!filename!.obj"
            
            rem Check if compilation was successful
            if errorlevel 1 (
                echo Compilation failed for file: "%%f"
            ) else (
                echo Compiled "%%f"
            )
        ) else (
            echo No changes detected in "%%f"
        )
    ) else (
        rem Compile the file if the object file doesn't exist
        %compiler% -c "%%f" -o "%output_dir%\!filename!.obj" %flags%
        
        rem Check if compilation was successful
        if errorlevel 1 (
            echo Compilation failed for file: "%%f"
        ) else (
            echo Compiled "%%f"
        )
    )
)

rem Link all object files into a single executable
echo Linking object files...
%compiler% "%output_dir%\*.obj" -o "sic.exe" %flags%

echo Compilation and linking completed.

endlocal

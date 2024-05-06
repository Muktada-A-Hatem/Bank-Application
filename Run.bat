set RAYLIB_INCLUDE_DIR=include\
set RAYLIB_LIB_DIR=lib\

set SOURCE_CODE=src/Main.cpp src/Core.cpp src/Global.cpp src/Interface.cpp
set LIBS=-s -O2 -std=c99 -lraylib -lopengl32 -lgdi32 -lwinmm
::-DDEBUG

c++ %SOURCE_CODE% -o bin/Bank.exe -I %RAYLIB_INCLUDE_DIR% -L %RAYLIB_LIB_DIR% %LIBS%

if exist bin/Bank.exe start bin/Bank.exe
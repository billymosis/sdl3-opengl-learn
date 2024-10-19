cmake -S . -G "Unix Makefiles" -B build
cd build
make
if %errorlevel% neq 0 (
    echo Build failed, not launching the executable.
) else (
    learn_gl.exe
)

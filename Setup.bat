rmdir /s /q .\build
mkdir .\build
cd .\build
cmake .. -G "Visual Studio 16 2019" -A x64 -DPLATFORM:STRING=Windows -DCMAKE_BUILD_TYPE=Debug

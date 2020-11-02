rmdir /s /q .\build
mkdir .\build
cd .\build
cmake .. -DPLATFORM:STRING=Windows -DCMAKE_BUILD_TYPE=Debug
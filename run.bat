cmake -B . -DCMAKE_CXX_COMPILER=g++ -G "MinGW Makefiles"
cmake --build .
.\bin\%1

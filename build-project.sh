mkdir bin
cd bin
cmake -DCMAKE_BUILD_TYPE=Release ../src
make
find . -mindepth 1 ! -name "lang" -exec rm -rf {} +

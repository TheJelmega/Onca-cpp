:: Google Test
git clone https://github.com/google/googletest.git third-party/googletest
cd third-party/googletest
mkdir build
cd build
cmake -DBUILD_GMOCK=OFF -Dgtest_force_shared_crt=ON  ../
msbuild -m googletest/gtest.sln /property:Configuration=Debug
msbuild -m googletest/gtest.sln /property:Configuration=Release
cd ../../../

:: Google benchmark
git clone https://github.com/google/benchmark.git third-party/googlebench
robocopy third-party/googletest third-party/googlebench/googletest /E
cd third-party/googlebench
mkdir build
cmake -DCMAKE_BUILD_TYPE=Release -S . -B "build"
cmake --build "build" --config Release
cd ../../

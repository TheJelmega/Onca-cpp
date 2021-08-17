:: Google Test
git clone https://github.com/google/googletest.git third-party/googletest
cd third-party/googletest
mkdir build
cd build
cmake -DBUILD_GMOCK=OFF -Dgtest_force_shared_crt=ON  ../
msbuild -m googletest/gtest.sln /property:Configuration=Debug
msbuild -m googletest/gtest.sln /property:Configuration=Release
cd ../../


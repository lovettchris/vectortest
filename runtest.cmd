@echo off
msbuild /p:Configuration=Release /p:Platform=x64 VectorMultiplyTest\VectorMultiplyTest.sln /t:Rebuild  > nul  
sleep 1
echo =============== Double test =============================
.\VectorMultiplyTest\x64\Release\VectorMultiplyTest.exe  -i 100 -c
.\VectorMultiplyTest\x64\Release\VectorMultiplyTest.exe  -i 100 -c2
.\VectorMultiplyTest\x64\Release\VectorMultiplyTest.exe  -i 100 -cxx
.\VectorMultiplyTest\x64\Release\VectorMultiplyTest.exe  -i 100 -std::for_each
.\VectorMultiplyTest\x64\Release\VectorMultiplyTest.exe  -i 100 -std::transform
.\VectorMultiplyTest\x64\Release\VectorMultiplyTest.exe  -i 100 -blas
.\VectorMultiplyTest\x64\Release\VectorMultiplyTest.exe  -i 100 -avx

msbuild /p:Configuration=Release /p:Platform=x64 VectorMultiplyTest\VectorMultiplyTest.sln /t:Rebuild /P:FLOATTEST=1 > nul 
sleep 1
echo =============== Single test =============================
.\VectorMultiplyTest\x64\Release\VectorMultiplyTest.exe  -i 100 -c
.\VectorMultiplyTest\x64\Release\VectorMultiplyTest.exe  -i 100 -c2
.\VectorMultiplyTest\x64\Release\VectorMultiplyTest.exe  -i 100 -cxx
.\VectorMultiplyTest\x64\Release\VectorMultiplyTest.exe  -i 100 -std::for_each
.\VectorMultiplyTest\x64\Release\VectorMultiplyTest.exe  -i 100 -std::transform
.\VectorMultiplyTest\x64\Release\VectorMultiplyTest.exe  -i 100 -blas
.\VectorMultiplyTest\x64\Release\VectorMultiplyTest.exe  -i 100 -avx

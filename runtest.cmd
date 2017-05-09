@echo off
msbuild /p:Configuration=Release /p:Platform=x64 VectorMultiplyTest\VectorMultiplyTest.sln

D:\git\vectortest\VectorMultiplyTest\x64\Release\VectorMultiplyTest.exe  -i 100 -c
D:\git\vectortest\VectorMultiplyTest\x64\Release\VectorMultiplyTest.exe  -i 100 -c2
D:\git\vectortest\VectorMultiplyTest\x64\Release\VectorMultiplyTest.exe  -i 100 -cxx
D:\git\vectortest\VectorMultiplyTest\x64\Release\VectorMultiplyTest.exe  -i 100 -std::for_each
D:\git\vectortest\VectorMultiplyTest\x64\Release\VectorMultiplyTest.exe  -i 100 -std::transform
D:\git\vectortest\VectorMultiplyTest\x64\Release\VectorMultiplyTest.exe  -i 100 -blas
D:\git\vectortest\VectorMultiplyTest\x64\Release\VectorMultiplyTest.exe  -i 100 -avx
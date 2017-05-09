// VectorMultiplyTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Timer.h"
#include <string>
#include <vector>
#include <algorithm>    // std::transform
#include "MathHelpers.h"
#include "cblas.h"

enum TestType {
    None,
    CTest,
    C2Test,
    CxxTest,
    StdForEachTest,
    StdTransformTest,
    ValArrayTest,
    BlasTest,
    AvxTest
};
int iterations = 10;
size_t size = 10000000;
TestType test = None;
std::string testName;

bool parseArguments(int argc, char* argv[]) {

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "-c") {
            test = CTest;
            testName = "c-style with incrementing pointer";
        }
        else if (arg == "-c2") {
            test = C2Test;
            testName = "c-style with incrementing index";
        }
        else if (arg == "-cxx") {
            test = CxxTest;
            testName = "c++-style";
        }
        else if (arg == "-std::for_each") {
            test = StdForEachTest;
            testName = "std::for_each";
        }
        else if (arg == "-stdtransform") {
            test = StdTransformTest;
            testName = "std::transform";
        }
        else if (arg == "-valarray") {
            test = ValArrayTest;
            testName = "valarray";
        }
        else if (arg == "-blas") {
            test = BlasTest;
            testName = "Blas";
        }
        else if (arg == "-avx") {
            test = AvxTest;
            testName = "AVX";
        }
        else if (arg == "-i") {
            iterations = atoi(argv[++i]);
        }
        else if (arg == "-s") {
            size = atoi(argv[++i]);
        }
        else {
            printf("Unknown argument: %s\n", arg.c_str());
            return false;
        }
    }
    if (test == None) {
        printf("Please specify the test type\n");
        return false;
    }
    return true;
}

void printUsage() {
    printf("Usage: VectorMultiplyTest [-c|-cxx|-stdvector|-stdtransform|-valarray|-blas|-avx] [-i iterations] [-s size]\n");
}


void RunCTest(float* begin, float* end,  float s) {
    for (float* f = begin; f < end; f++)
    {
        *f *= s;
    }
}
void RunC2Test(float* v, size_t size, float s) {
    for (size_t i = 0; i < size; ++i)
    {
        v[i] *= s;
    }
}
void RunCxxTest(std::vector<float>& v, float s) {
    for (auto& e : v)
    {
        e *= s;
    }

}
void RunStdForEachTest(std::vector<float>& v, float s) {
    std::transform(v.begin(), v.end(), v.begin(), [s](float e) {return e * s; });
}
void RunStdTransformTest(std::vector<float>& v, float s) {
    std::transform(v.begin(), v.end(), v.begin(), [s](float e) {return e * s; });
}
void RunValArrayTest(float* v, float s) {

}
void RunBlasTest(float* v, blasint size, float s) {
    cblas_sscal(size, s, v, 1);
}
void RunAvxTest(float* begin, float* end, float s) {

    __m256 _s = { s, s, s, s, s, s, s, s };
    float* m256end = end - 8;
    float* p = begin;
    for (; p < m256end; p += 8)
    {
        __m256 _e = _mm256_loadu_ps(p);
        auto _r = _mm256_mul_ps(_e, _s);
        _mm256_store_ps(p, _r);
    }
    // finish with c-style
    for (; p < end; p++) {
        *p *= s;
    }
}

int main(int argc, char* argv[])
{
    if (!parseArguments(argc, argv)) {
        printUsage();
        return 1;
    }

    std::vector<double> times;
    Timer timer;

    printf("Running %s with %d iterations...", testName.c_str(), iterations);

    float* v = (float*)calloc(size + 1, sizeof(float));
    std::vector<float> vector(size);
    for (size_t j = 0; j < size; j++)
    {
        v[j] = (float)rand() / 32767.0f;
        vector[j] = v[j];
    }
    float e = (float)std::exp(1);

    // test if blas works..

    if (test == BlasTest) {
        RunBlasTest(v, (blasint)size, e);
        RunCxxTest(vector, e);
        for (size_t j = 0; j < size; j++)
        {
            if (vector[j] != v[j]) {
                printf("Blas results don't match at position %d, found %f, expecting %f\n", (int)j, v[j], vector[j]);
                return 1;
            }
        }
    }
    if (test == AvxTest) {
        RunAvxTest(v, &v[size], e);
        RunCxxTest(vector, e);
        for (size_t j = 0; j < size; j++)
        {
            if (vector[j] != v[j]) {
                printf("AVX results don't match at position %d, found %f, expecting %f\n", (int)j, v[j], vector[j]);
                return 1;
            }
        }
    }


    for (int i = 0; i < iterations; i++)
    {
        timer.start();
        switch (test)
        {
        case CTest:
            RunCTest(v, &v[size], e);
            break;
        case C2Test:
            RunC2Test(v, size, e);
            break;
        case CxxTest:
            RunCxxTest(vector, e);
            break;
        case StdForEachTest:
            RunStdForEachTest(vector, e);
            break;
        case StdTransformTest:
            RunStdTransformTest(vector, e);
            break;
        case ValArrayTest:
            RunValArrayTest(v, e);
            break;
        case BlasTest:
            RunBlasTest(v, (blasint)size, e);
            break;
        case AvxTest:
            RunAvxTest(v, &v[size], e);
            break;
        }
        timer.stop();
        times.push_back(timer.milliseconds());
    }

    printf("\nMin=%f, Max=%f, Avg=%f, StdDev=%f\n", MathHelpers::Min(times), MathHelpers::Max(times), MathHelpers::Mean(times), MathHelpers::StandardDeviation(times));

    free(v);
    return 0;
}


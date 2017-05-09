// VectorMultiplyTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Timer.h"
#include <string>
#include <vector>
#include <algorithm>    // std::transform
#include "MathHelpers.h"
#include "cblas.h"

//#define FLOATTEST
#ifdef FLOATTEST
typedef float ElementType;
#else
typedef double ElementType;
#endif
enum TestType {
    None,
    CTest,
    C2Test,
    CxxTest,
    StdForEachTest,
    StdTransformTest,
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
        else if (arg == "-std::transform") {
            test = StdTransformTest;
            testName = "std::transform";
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
    printf("Usage: VectorMultiplyTest [-c|-cxx|-std::for_each|-std::transform|-blas|-avx] [-i iterations] [-s size]\n");
}


void RunCTest(ElementType* begin, ElementType* end, ElementType s) {
    for (ElementType* f = begin; f < end; f++)
    {
        *f *= s;
    }
}
void RunC2Test(ElementType* v, size_t size, ElementType s) {
    for (size_t i = 0; i < size; ++i)
    {
        v[i] *= s;
    }
}
void RunCxxTest(std::vector<ElementType>& v, ElementType s) {
    for (auto& e : v)
    {
        e *= s;
    }

}
void RunStdForEachTest(std::vector<ElementType>& v, ElementType s) {
    std::transform(v.begin(), v.end(), v.begin(), [s](ElementType e) {return e * s; });
}
void RunStdTransformTest(std::vector<ElementType>& v, ElementType s) {
    std::transform(v.begin(), v.end(), v.begin(), [s](ElementType e) {return e * s; });
}
void RunBlasTest(ElementType* v, blasint size, ElementType s) {
#ifdef FLOATTEST
    cblas_sscal(size, s, v, 1);
#else
    cblas_dscal(size, s, v, 1);
#endif
}
void RunAvxTest(ElementType* begin, ElementType* end, ElementType s) {
#ifdef FLOATTEST
    __m256 _s = { s, s, s, s, s, s, s, s };
    const int step = 8;
#else
    __m256d _s = { s, s, s, s };
    const int step = 4;
#endif
    ElementType* p = begin;
    ElementType* m256end = end - step;
    for (; p < m256end; p += step)
    {
#ifdef FLOATTEST
        __m256 _e = _mm256_loadu_ps(p);
        auto _r = _mm256_mul_ps(_e, _s);
        _mm256_store_ps(p, _r);
#else
        __m256d _e = _mm256_loadu_pd(p);
        auto _r = _mm256_mul_pd(_e, _s);
        _mm256_store_pd(p, _r);
#endif
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
    
    printf("%s,\t", testName.c_str());

    ElementType* v = (ElementType*)calloc(size + 1, sizeof(ElementType));
    std::vector<ElementType> vector(size);
    for (size_t j = 0; j < size; j++)
    {
        v[j] = (ElementType)rand() / 32767.0f;
        vector[j] = v[j];
    }
    ElementType e = (ElementType)std::exp(1);

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

    double min = MathHelpers::Min(times);
    double max = MathHelpers::Max(times);
    double avg = MathHelpers::Mean(times);

    printf("%f, %f, %f\n", avg, avg - min, max - avg);

    free(v);
    return 0;
}


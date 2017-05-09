#pragma once
#include <vector>

class MathHelpers
{
public:
    /// <summary>
    /// Return the Minimum of the given numbers.
    /// </summary>
    static double Min(std::vector<double> values)
    {
        double min = 0;
        uint64_t count = 0;
        for (double& d : values)
        {
            if (count == 0) {
                min = d;
            }
            else if (d < min) {
                min = d;
            }
            count++;
        }
        return min;
    }

    /// <summary>
    /// Return the Maximum of the given numbers.
    /// </summary>
    static double Max(std::vector<double> values)
    {
        double max = 0;
        uint64_t count = 0;
        for (double& d : values)
        {
            if (count == 0) {
                max = d;
            }
            else if (d > max) {
                max = d;
            }
            count++;
        }
        return max;
    }
    /// <summary>
    /// Return the Mean of the given numbers.
    /// </summary>
    static double Mean(std::vector<double> values)
    {
        double sum = 0;
        double count = 0;
        for (double& d : values)
        {
            sum += d;
            count++;
        }
        if (count == 0) return 0;
        return sum / count;
    }

    static double StandardDeviation(std::vector<double> values)
    {
        double mean = Mean(values);
        double totalSquares = 0;
        int count = 0;
        for (double& d : values)
        {
            count++;
            double diff = mean - d;
            totalSquares += diff * diff;
        }
        if (count == 0)
        {
            return 0;
        }
        return sqrt((double)(totalSquares / (double)count));
    }

};
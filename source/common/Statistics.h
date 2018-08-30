#pragma once

#include "Log.h"

class Statistics {
    private:
        Text name;
        int count;
        double min;
        double mean;
        double median;
        double max;
        double variance;

    public:
        Statistics (const Text& _name, vector<double>& array) :
            name (_name), count (array.size ()), min (DBL_MAX), mean (0), median (0), max (-DBL_MAX), variance (0) {

            // none of this is meaningful unless there are samples
            if (count > 0) {
                // sort the array to make medians easy
                sort (array.begin (), array.end ());

                // compute the median
                int midpoint = count / 2;
                median = ((count & 0x01) == 0) ? ((array[midpoint - 1] + array[midpoint]) * 0.5) : array[midpoint];

                // compute the mean, capture min and max along the way
                for (vector<double>::iterator it = array.begin (); it != array.end (); ++it) {
                    double value = *it;
                    min = std::min (value, min);
                    max = std::max (value, max);
                    mean += value;
                }
                mean /= double (count);

                // compute the variance
                for (vector<double>::iterator it = array.begin (); it != array.end (); ++it) {
                    double value = *it;
                    double delta = value - mean;
                    variance += delta * delta;
                }
                variance /= double (count);
            }
        }

        int getCount () const { return count; }
        double getMin () const { return min; }
        double getMean () const { return mean; }
        double getMedian() const { return median; }
        double getMax () const { return max; }
        double getVariance () const { return variance; }
        double getStdDev () const { return sqrt (variance); }

        const Statistics& report () const {
            cerr << "STATS (" << name << " - ";
            if (count > 0) {
                cerr <<
                            "count: " << count << ", " <<
                            "mean: " << mean << ", " <<
                            "range: [" << min << " - " << max << "], " <<
                            "median: " << median << ", " <<
                            "variance: " << variance << ", " <<
                            "stddev: " << getStdDev ();
            } else {
                cerr << "count: 0";
            }
            cerr << ")" << endl;
            return *this;
        }
};

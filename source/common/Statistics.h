#pragma once

#include "Log.h"

class Statistics {
    public:
        enum Outliers {
            ELIMINATE_OUTLIERS,
            KEEP
        };

    private:
        vector<double> array;
        Text name;
        double min;
        double mean;
        double median;
        double max;
        double variance;

        // compute the median on a sorted array
        static double computeMedian (const double* array, uint count) {
            int midpoint = count >> 1;
            return ((count & 0x01) == 0) ? ((array[midpoint - 1] + array[midpoint]) * 0.5) : array[midpoint];
        }

        void init (const Text& _name, const double* _array, uint count, Outliers outliers) {
            name = _name;
            min = numeric_limits<double>::max ();
            max = numeric_limits<double>::lowest ();
            mean = median = variance = 0;

            // none of this is meaningful unless there are samples
            if (count > 0) {
                // copy, and sort the array to make medians easy
                array.assign(_array, _array + count);
                sort (array.begin (), array.end ());

                // if we want to eliminate outliers, and there are enough samples to perform IQR...
                if ((outliers == ELIMINATE_OUTLIERS) and (count > 5)) {
                    // using the Inter-Quartile Range (IQR) method for eliminating outliers
                    // https://en.wikipedia.org/wiki/Interquartile_range
                    int halfLength = count >> 1;
                    double q1 = computeMedian (array.data (), halfLength);
                    double q3 = computeMedian (array.data () + halfLength + (count & 0x01), halfLength);

                    // compute the IQR fences and get the result (1.5 is a "magic" number
                    // associated with the method, because John Tukey chose this number when he
                    // invented the box and whisker plot method, and it has worked well enough ever
                    // since)
                    double iqr = (q3 - q1) * 1.5;
                    double fenceLow = q1 - iqr;
                    double fenceHigh = q3 + iqr;

                    vector<double> filtered;
                    double lastValue = array.back();
                    Log& log = Log::debug () << "IQR (q1: " << q1 << ", q3: " << q3 << ", iqr: " << iqr << ", min: " << fenceLow << ", max: " << fenceHigh << ")";
                    for (vector<double>::iterator it = array.begin (); it != array.end (); ++it) {
                        double value = *it;
                        if (value != lastValue) {
                            log << endl << value << " ";
                            lastValue = value;
                        }

                        if ((value >= fenceLow) and (value <= fenceHigh)) {
                            filtered.push_back(value);
                            log << "+";
                        } else {
                            log << "-";
                        }
                    }
                    log << endl;

                    array = filtered;
                    count = array.size ();
                }

                // compute the median
                median = computeMedian (array.data (), count);

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

    public:
        Statistics (const Text& _name, const double* _array, uint _count, Outliers outliers = KEEP) {
            init (_name, _array, _count, outliers);
        }

        Statistics (const Text& _name, const vector<double>& _array, Outliers outliers = KEEP) {
            init (_name, _array.data (), _array.size (), outliers);
        }

        int getCount () const { return array.size (); }
        double getMin () const { return min; }
        double getMean () const { return mean; }
        double getMedian() const { return median; }
        double getMax () const { return max; }
        double getVariance () const { return variance; }
        double getStdDev () const { return sqrt (variance); }

        const Statistics& report () const {
            cerr << "STATS (" << name << " - ";
            uint count = array.size ();
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

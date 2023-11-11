// Copyright(C) 2023 - present The Project Contributors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iostream>
#include <vector>

class TimeSlice
{
public:
    TimeSlice discounted_rollback(int step) const { return TimeSlice(); }
};

TimeSlice max(const TimeSlice& s1, const TimeSlice& s2) { return TimeSlice(); }
TimeSlice max(const TimeSlice& s1, double s2) { return TimeSlice(); }
TimeSlice max(double s1, const TimeSlice& s2) { return TimeSlice(); }

TimeSlice operator+(const TimeSlice& s1, const TimeSlice& s2) { return TimeSlice(); }
TimeSlice operator+(double s1, const TimeSlice& s2) { return TimeSlice(); }
TimeSlice operator+(const TimeSlice& s1, double s2) { return TimeSlice(); }
TimeSlice operator-(const TimeSlice& s1, const TimeSlice& s2) { return TimeSlice(); }
TimeSlice operator-(double s1, const TimeSlice& s2) { return TimeSlice(); }
TimeSlice operator-(const TimeSlice& s1, double s2) { return TimeSlice(); }

class Model
{
public:
    TimeSlice get_underlying_time_slice(int step) const { return TimeSlice(); }
    TimeSlice get_const_time_slice(int step, double value) const { return TimeSlice(); }
    std::vector<double> get_underlying_path(int step) const { return std::vector<double>(step + 1, 1.0); }
    TimeSlice operator+(const TimeSlice& other) const { return TimeSlice(); }
    TimeSlice operator-(const TimeSlice& other) const { return TimeSlice(); }
    TimeSlice operator-(double other) const { return TimeSlice(); }
    double value() const { return 0.0; }
};

// EuropeanVanillaOption.1
double european_vanilla_option(const Model& model)
{
    double notl = 100000.0;
    int expiry_step = 50;
    int side = 1;
    double s = 1.5;

    std::vector<double> underlying = model.get_underlying_path(expiry_step);
    double payoff = side * notl * std::max(s - underlying[expiry_step], 0.0);
    return payoff;
}

// AmericanVanillaOption.1
TimeSlice american_vanilla_option(const Model& model)
{
    double notl = 100000.0;
    int expiry_step = 50;
    int side = 1;
    double s = 1.5;

    TimeSlice u = model.get_underlying_time_slice(expiry_step);
    TimeSlice option = max(s - u, 0.0);
    for (int i = 0; i < expiry_step; ++i)
    {
        option = option.discounted_rollback(i);
        TimeSlice u = model.get_underlying_time_slice(expiry_step);
        TimeSlice payoff = max(s - u, 0.0);
        option = max(option, payoff);
    }
    return option;
}

// AsianOption.1
double asian_option(const Model& model)
{
    double notl = 100000.0;
    int expiry_step = 50;
    int side = -1;
    double s = 1.5;

    std::vector<double> underlying = model.get_underlying_path(expiry_step);
    double avg = 0.0;
    for (int i = 0; i < expiry_step; ++i)
    {
        avg += underlying[i];
    }
    avg /= expiry_step;

    double payoff = side * notl * std::max(avg - s, 0.0);
    return payoff;
}

// LookbackOption.1
double lookback_option(const Model& model)
{
    double notl = 100000.0;
    int expiry_step = 50;
    int side = 1;
    double s = 1.5;

    std::vector<double> underlying = model.get_underlying_path(expiry_step);
    double m = underlying[0];
    for (int i = 1; i < expiry_step; ++i)
    {
        double u = underlying[i];
        if (u > m) u = m;
    }

    double payoff = side * notl * std::max(m - s, 0.0);
    return payoff;
}

// FadeInOption
double fade_in_option(const Model& model)
{
    double notl = 100000.0;
    int expiry_step = 50;
    int side = 1;
    double s = 1.5;
    double b = 1.5;

    std::vector<double> underlying = model.get_underlying_path(expiry_step);
    double mult = 0.0;
    for (int i = 0; i < expiry_step; ++i)
    {
        double u = underlying[i];
        if (u >= b) mult += 1.0;
    }
    mult /= expiry_step;

    double payoff = side * mult * notl * std::max(underlying[expiry_step] - s, 0.0);
    return payoff;
}

// OneTouchOption.1
double one_touch_option(const Model& model)
{
    double notl = 100000.0;
    int expiry_step = 50;
    int side = -1;
    double b = 1.5;

    std::vector<double> underlying = model.get_underlying_path(expiry_step);
    int alive = 1;
    for (int i = 0; i <= expiry_step; ++i)
    {
        double u = underlying[i];
        if (u >= b) alive = 0;
    }
    double payoff = side * notl * alive;
    return payoff;
}

// DoubleNoTouchOption.1
double double_no_touch_option(const Model& model)
{
    double notl = 100000.0;
    int expiry_step = 50;
    int side = -1;
    double b1 = 1.3;
    double b2 = 1.5;

    std::vector<double> underlying = model.get_underlying_path(expiry_step);
    int alive = 1;
    for (int i = 0; i <= expiry_step; ++i)
    {
        double u = underlying[i];
        if (u <= b1) alive = 0;
        if (u >= b2) alive = 0;
    }
    double payoff = side * notl * alive;
    return payoff;
}

int main()
{
    int step_count = 100;
    std::vector<double> fx_rate(step_count, 1.5);
    
    Model model;
    
    european_vanilla_option(model);
    asian_option(model);
    lookback_option(model);
    fade_in_option(model);
    one_touch_option(model);
    double_no_touch_option(model);
}
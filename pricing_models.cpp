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

class Date
{
public:
    Date(int year, int month, int day_of_month) {}
};

class TimeSlice
{
public:
    void discounted_rollback(int step) {}
    double value() const { return 0.0; }
    TimeSlice operator-() const { return TimeSlice(); }
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
TimeSlice operator*(const TimeSlice& s1, const TimeSlice& s2) { return TimeSlice(); }
TimeSlice operator*(double s1, const TimeSlice& s2) { return TimeSlice(); }
TimeSlice operator*(const TimeSlice& s1, double s2) { return TimeSlice(); }

class Model
{
public:
    double get_dcf(std::string basis, int start_step, int end_step) const { return 0; }
    TimeSlice get_underlying_time_slice(std::string, int step) const { return TimeSlice(); }
    TimeSlice get_const_time_slice(int step, double value) const { return TimeSlice(); }
    std::vector<double> get_underlying_path(std::string symbol, int step) const { return std::vector<double>(step + 1, 1.0); }
    TimeSlice operator+(const TimeSlice& other) const { return TimeSlice(); }
    TimeSlice operator-(const TimeSlice& other) const { return TimeSlice(); }
    TimeSlice operator-(double other) const { return TimeSlice(); }
    double value() const { return 0.0; }
};

// EuropeanVanillaOption.1
double european_vanilla_option_1(const Model& model)
{
double n = 300000.0;
int expiry_step = 50;
int side = 1;
double s = 1.5;

std::vector<double> underlying = model.get_underlying_path("SPX", expiry_step);
double payoff = side * n * std::max(s - underlying[expiry_step], 0.0);
return payoff;
}

// AmericanVanillaOption.1
TimeSlice american_vanilla_option_1(const Model& model)
{
    double n = 200000.0;
    int expiry_step = 50;
    int side = 1;
    double s = 1.5;

    TimeSlice option = model.get_const_time_slice(expiry_step, 0.0);
    for (int i = expiry_step; i >= 0; --i)
    {
        TimeSlice u = model.get_underlying_time_slice(i);
        TimeSlice payoff = s - u;
        option.discounted_rollback(i);
        option = max(payoff, option);
    }
    return side * n * option;
}

// AsianOption.1
double asian_option_1(const Model& model)
{
double n = 20000.0;
int expiry_step = 50;
int side = -1;
double s = 1.5;

std::vector<double> underlying = model.get_underlying_path("SPX", expiry_step);
double avg = 0.0;
for (int i = 0; i < expiry_step; ++i)
{
    avg += underlying[i];
}
avg /= expiry_step;

double payoff = side * n * std::max(avg - s, 0.0);
return payoff;
}

// LookbackOption.1
double lookback_option_1(const Model& model)
{
double n = 150000.0;
int expiry_step = 50;
int side = 1;
double s = 3.5;

std::vector<double> underlying = model.get_underlying_path("SPX", expiry_step);
double m = underlying[0];
for (int i = 1; i < expiry_step; ++i)
{
    double u = underlying[i];
    if (u > m) m = u;
}

double payoff = side * n * std::max(m - s, 0.0);
return payoff;
}

// FadeInOption
double fade_in_option_1(const Model& model)
{
double n = 50000.0;
int expiry_step = 50;
int side = 1;
double s = 1.5;
double b = 2.5;

std::vector<double> underlying = model.get_underlying_path("SPX", expiry_step);
double mult = 0.0;
for (int i = 0; i < expiry_step; ++i)
{
    double u = underlying[i];
    if (u >= b) mult += 1.0;
}
mult /= expiry_step;

double payoff = side * mult * n * std::max(underlying[expiry_step] - s, 0.0);
return payoff;
}

// OneTouchOption.1
double one_touch_option_1(const Model& model)
{
double n = 300000.0;
int expiry_step = 50;
int side = -1;
double b = 1.5;

std::vector<double> underlying = model.get_underlying_path("SPX", expiry_step);
int alive = 1;
for (int i = 0; i <= expiry_step; ++i)
{
    double u = underlying[i];
    if (u >= b) alive = 0;
}
double payoff = side * n * alive;
return payoff;
}

// DoubleNoTouchOption.1
double double_no_touch_option_1(const Model& model)
{
double n = 250000.0;
int expiry_step = 50;
int side = -1;
double b1 = 1.3;
double b2 = 1.5;

std::vector<double> underlying = model.get_underlying_path("SPX", expiry_step);
int alive = 1;
for (int i = 0; i <= expiry_step; ++i)
{
    double u = underlying[i];
    if (u <= b1) alive = 0;
    if (u >= b2) alive = 0;
}
double payoff = side * n * alive;
return payoff;
}

// ForwardRateAgreement.1
double forward_rate_agreement_1(const Model& model)
{
double n = 500000.0;
int start_step = 48;
int end_step = 60;
int side = 1;
double f = 0.05;

double dcf = model.get_dcf("ACT/365", start_step, end_step);
TimeSlice s = model.get_const_time_slice(start_step, 1.0);
TimeSlice e = model.get_const_time_slice(end_step, 1.0);
e.discounted_rollback(start_step);
TimeSlice time_slice = s - e * (1 + dcf * f);
time_slice.discounted_rollback(0);
double v = time_slice.value();
return (side * n) * v;
}

// NonCallableFixedForFloatingSwap.1
double non_callable_fixed_for_floating_swap(const Model& model)
{
double n = 1200000.0;
int start_step = 0;
int end_step = 60;
int freq_step = 6;
int side = -1;
double f = 0.03;
double s = 0.002;

TimeSlice result = model.get_const_time_slice(end_step, 0.0);
for (int step = end_step; step >= start_step; step -= freq_step)
{
    if (step < end_step) result.discounted_rollback(step);

    double dcf1 = model.get_dcf("30/360", step - freq_step, step);
    double dcf2 = model.get_dcf("ACT/365", step - freq_step, step);
    TimeSlice unit = model.get_const_time_slice(step, 1.0);
        
    if (step == end_step) result = -unit;
    if (step == start_step) result = result + unit;
    if (step > start_step) result = result - (dcf1 * f - dcf2 * s) * unit;
}
result.discounted_rollback(0);
double v = result.value();
return (side * n) * v;
}

// NonCallableFixedRateNote.1
double non_callable_fixed_rate_note(const Model& model)
{
double n = 750000.0;
int start_step = 0;
int end_step = 60;
int freq_step = 6;
int side = -1;
double f = 0.02;

TimeSlice result = model.get_const_time_slice(end_step, 0.0);
for (int step = end_step; step > start_step; step -= freq_step)
{
    if (step < end_step) result.discounted_rollback(step);

    double dcf = model.get_dcf("30/360", step - freq_step, step);
    TimeSlice unit = model.get_const_time_slice(step, 1.0);

    if (step == end_step) result = unit;
    result = result + (dcf * f) * unit;
}
result.discounted_rollback(0);
double v = result.value();
return (side * n) * v;
}

int main()
{
    int step_count = 100;
    std::vector<double> fx_rate(step_count, 1.5);
    
    Model model;
    
    european_vanilla_option_1(model);
    american_vanilla_option_1(model);
    asian_option_1(model);
    lookback_option_1(model);
    fade_in_option_1(model);
    one_touch_option_1(model);
    double_no_touch_option_1(model);
    forward_rate_agreement_1(model);
    non_callable_fixed_for_floating_swap(model);
    non_callable_fixed_rate_note(model);
}

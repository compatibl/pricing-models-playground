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

// European Vanilla Option
double european_vanilla_option(const std::vector<double>& underlying)
{
    double notl = 100000.0;
    int expiry_step = 50;
    int side = 1;
    double s = 1.5;

    double discounted_payoff = side * notl * std::max(s - underlying[expiry_step], 0.0);
    return discounted_payoff;
}

// Asian Option
double asian_option(const std::vector<double>& underlying)
{
    double notl = 100000.0;
    int expiry_step = 50;
    int side = 1;
    double s = 1.5;

    double avg = 0.0;
    for (int i = 0; i < expiry_step; ++i)
    {
        avg += underlying[i];
    }
    avg /= expiry_step;

    double discounted_payoff = side * notl * std::max(avg - s, 0.0);
    return discounted_payoff;
}

// Lookback Option
double lookback_option(const std::vector<double>& underlying)
{
    double notl = 100000.0;
    int expiry_step = 50;
    int side = 1;
    double s = 1.5;

    double max_underlying = underlying[0];
    for (int i = 1; i < expiry_step; ++i)
    {
        double u = underlying[i];
        if (u > max_underlying) u = max_underlying;
    }

    double discounted_payoff = side * notl * std::max(max_underlying - s, 0.0);
    return discounted_payoff;
}

// Fade-In Option
double fade_in_option(const std::vector<double>& underlying)
{
    double notl = 100000.0;
    int expiry_step = 50;
    int side = 1;
    double s = 1.5;
    double b = 1.5;

    double mult = 0.0;
    for (int i = 0; i < expiry_step; ++i)
    {
        double u = underlying[i];
        if (u >= b) mult += 1.0;
    }
    mult /= expiry_step;

    double discounted_payoff = side * mult * notl * std::max(underlying[expiry_step] - s, 0.0);
    return discounted_payoff;
}

// One-Touch Option
double one_touch_option(const std::vector<double>& underlying)
{
    double notl = 100000.0;
    int expiry_step = 50;
    int side = -1;
    double b = 1.5;

    int alive = 1;
    for (int i = 0; i <= expiry_step; ++i)
    {
        double u = underlying[i];
        if (u >= b) alive = 0;
    }
    double discounted_payoff = side * notl * alive;
    return discounted_payoff;
}

// Double No-Touch Option
double double_no_touch_option(const std::vector<double>& underlying)
{
    double notl = 100000.0;
    int expiry_step = 50;
    int side = -1;
    double b1 = 1.3;
    double b2 = 1.5;

    int alive = 1;
    for (int i = 0; i <= expiry_step; ++i)
    {
        double u = underlying[i];
        if (u <= b1) alive = 0;
        if (u >= b2) alive = 0;
    }
    double discounted_payoff = side * notl * alive;
    return discounted_payoff;
}

int main()
{
    int step_count = 100;
    std::vector<double> fx_rate(step_count, 1.5);
    
    {
        european_vanilla_option(fx_rate);
        asian_option(fx_rate);
        lookback_option(fx_rate);
        fade_in_option(fx_rate);
        one_touch_option(fx_rate);
        double_no_touch_option(fx_rate);
    }
}

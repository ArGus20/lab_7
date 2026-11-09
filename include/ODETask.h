#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <stdexcept>
#include <functional>

class ScopeLogger {
    std::string name;
public:
    explicit ScopeLogger(std::string n);
    ~ScopeLogger();
};

class ODETaskException : public std::runtime_error {
public:
    explicit ODETaskException(const std::string& msg);
};

class ODEInvalidArgumentException : public ODETaskException {
    std::string paramName;
    double invalidValue;
public:
    ODEInvalidArgumentException(const std::string& msg, std::string param, double val);
    const std::string& getParamName() const noexcept;
    double getInvalidValue() const noexcept;
};

class ODEComputationException : public ODETaskException {
    std::size_t stepIndex;
public:
    ODEComputationException(const std::string& msg, std::size_t step);
    std::size_t getStepIndex() const noexcept;
};

struct Point {
    double x;
    double y;
};

class ODETask {
private:
    double x0;
    double y0;
    double xEnd;
    double step;
    std::size_t maxAllowedSteps;
    std::vector<Point> trajectory;

public:
    ODETask(double x_start, double y_start, double x_end, double h, std::size_t maxSteps = 10000);

    void setStep(double h);
    void computeTrajectory(const std::function<double(double, double)>& f);

    std::size_t getTrajectorySize() const noexcept;
    const std::vector<Point>& getTrajectory() const noexcept;
};
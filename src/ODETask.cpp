#include "ODETask.h"

ScopeLogger::ScopeLogger(std::string n) : name(std::move(n)) {
    std::cout << "[ScopeLogger] Created: " << name << '\n';
}

ScopeLogger::~ScopeLogger() {
    std::cout << "[ScopeLogger] Destroyed: " << name << '\n';
}

ODETaskException::ODETaskException(const std::string& msg)
    : std::runtime_error(msg) {}

ODEInvalidArgumentException::ODEInvalidArgumentException(const std::string& msg, std::string param, double val)
    : ODETaskException(msg + " (" + param + " = " + std::to_string(val) + ")"),
      paramName(std::move(param)), invalidValue(val) {}

const std::string& ODEInvalidArgumentException::getParamName() const noexcept {
    return paramName;
}

double ODEInvalidArgumentException::getInvalidValue() const noexcept {
    return invalidValue;
}

ODEComputationException::ODEComputationException(const std::string& msg, std::size_t step)
    : ODETaskException(msg + " at step " + std::to_string(step)), stepIndex(step) {}

std::size_t ODEComputationException::getStepIndex() const noexcept {
    return stepIndex;
}

ODETask::ODETask(double x_start, double y_start, double x_end, double h, std::size_t maxSteps)
    : x0(x_start), y0(y_start), xEnd(x_end), step(h), maxAllowedSteps(maxSteps) {

    if (!std::isfinite(x0) || !std::isfinite(y0)) {
        throw std::invalid_argument("Initial conditions must be finite numbers");
    }
    if (h <= 0.0) {
        throw ODEInvalidArgumentException("Step must be positive", "h", h);
    }
    if (x_end <= x_start) {
        throw ODEInvalidArgumentException("xEnd must be greater than x0", "xEnd", x_end);
    }
}

void ODETask::setStep(double h) {
    if (h <= 0.0) {
        throw ODEInvalidArgumentException("Step must be positive", "h", h);
    }
    step = h;
}

void ODETask::computeTrajectory(const std::function<double(double, double)>& f) {
    std::vector<Point> tempTrajectory;
    double currentX = x0;
    double currentY = y0;
    tempTrajectory.push_back({currentX, currentY});

    std::size_t stepsCount = 0;
    while (currentX < xEnd) {
        if (++stepsCount > maxAllowedSteps) {
            throw ODEComputationException("Exceeded maximum allowed steps count", stepsCount);
        }

        double h = std::min(step, xEnd - currentX);

        double k1 = f(currentX, currentY);
        double k2 = f(currentX + h / 2.0, currentY + h * k1 / 2.0);
        double k3 = f(currentX + h / 2.0, currentY + h * k2 / 2.0);
        double k4 = f(currentX + h, currentY + h * k3);

        if (!std::isfinite(k1) || !std::isfinite(k2) || !std::isfinite(k3) || !std::isfinite(k4)) {
            throw ODEComputationException("Non-finite derivative value encountered", stepsCount);
        }

        currentY += (h / 6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
        currentX += h;

        tempTrajectory.push_back({currentX, currentY});
    }

    trajectory = std::move(tempTrajectory);
}

std::size_t ODETask::getTrajectorySize() const noexcept {
    return trajectory.size();
}

const std::vector<Point>& ODETask::getTrajectory() const noexcept {
    return trajectory;
}
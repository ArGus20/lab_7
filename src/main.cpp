#include "ODETask.h"
#include <iostream>
#include <limits>

void stepLowLevel(ODETask& task) {
    ScopeLogger logger("stepLowLevel");
    task.computeTrajectory([](double x, double y) {
        if (x > 0.05) return std::numeric_limits<double>::infinity();
        return y;
    });
}

void processMidLevel(ODETask& task) {
    ScopeLogger logger("processMidLevel");
    try {
        stepLowLevel(task);
    } catch (const ODEComputationException& e) {
        std::cout << "[MidLevel Catch] Error caught: " << e.what() << '\n';
        throw;
    }
}

void runHighLevel() {
    ScopeLogger logger("runHighLevel");
    ODETask task(0.0, 1.0, 1.0, 0.1);
    processMidLevel(task);
}

int main() {
    try {
        ODETask task(0.0, 1.0, 1.0, 0.1);
        task.computeTrajectory([](double x, double y) { return y; });
        std::cout << "Points count: " << task.getTrajectorySize() << '\n';
        std::cout << "y(1.0) = " << task.getTrajectory().back().y << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    try {
        runHighLevel();
    } catch (const ODEComputationException& e) {
        std::cout << "[main Catch] ODEComputationException: " << e.what() << '\n';
    } catch (const ODETaskException& e) {
        std::cout << "[main Catch] ODETaskException: " << e.what() << '\n';
    } catch (const std::exception& e) {
        std::cout << "[main Catch] std::exception: " << e.what() << '\n';
    } catch (...) {
        std::cout << "[main Catch] Unknown exception\n";
    }

    return 0;
}
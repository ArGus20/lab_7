#include <gtest/gtest.h>
#include "ODETask.h"
#include <limits>

TEST(ODETaskTest, ComputesCorrectSolution) {
    ODETask task(0.0, 1.0, 1.0, 0.01);
    EXPECT_NO_THROW(task.computeTrajectory([](double x, double y) { return y; }));
    EXPECT_NEAR(task.getTrajectory().back().y, std::exp(1.0), 1e-3);
}

TEST(ODETaskTest, ThrowsOnInvalidStep) {
    EXPECT_THROW(ODETask(0.0, 1.0, 1.0, -0.1), ODEInvalidArgumentException);
}

TEST(ODETaskTest, ThrowsOnInvalidInterval) {
    EXPECT_THROW(ODETask(1.0, 1.0, 0.0, 0.1), ODEInvalidArgumentException);
}

TEST(ODETaskTest, ThrowsOnNaNInitialConditions) {
    EXPECT_THROW(ODETask(NAN, 1.0, 1.0, 0.1), std::invalid_argument);
}

TEST(ODETaskTest, ThrowsOnMaxStepsExceeded) {
    ODETask task(0.0, 1.0, 100.0, 0.1, 5);
    EXPECT_THROW(task.computeTrajectory([](double x, double y) { return y; }), ODEComputationException);
}

TEST(ODETaskTest, ThrowsOnNaNInDerivative) {
    ODETask task(0.0, 1.0, 1.0, 0.1);
    EXPECT_THROW(task.computeTrajectory([](double x, double y) { 
        return std::numeric_limits<double>::infinity(); 
    }), ODEComputationException);
}

TEST(ODETaskTest, StrongGuaranteeOnFailure) {
    ODETask task(0.0, 1.0, 1.0, 0.1);
    task.computeTrajectory([](double x, double y) { return y; });
    std::size_t oldSize = task.getTrajectorySize();
    double oldLastY = task.getTrajectory().back().y;

    EXPECT_THROW(task.computeTrajectory([](double x, double y) {
        if (x > 0.3) return std::numeric_limits<double>::infinity();
        return y;
    }), ODEComputationException);

    EXPECT_EQ(task.getTrajectorySize(), oldSize);
    EXPECT_EQ(task.getTrajectory().back().y, oldLastY);
}
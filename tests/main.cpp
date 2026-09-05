// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>
#include <logger/log_factory.h>

int main(int argc, char **argv) {
    falconfix::LogFactory::disableLogging();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <iostream>
#include <config/fix_config.h>
#include <app/fix_application.h>
#include <FIX4.4/core/types.h>

int main() {
    falconfix::FIXConfig cfg;
    std::cout << "FIX Client!\n";

    // Simple integration test with FIX4.4 types from falconfix_core

    // Test basic types from FIX4.4
    fix44::types::INT seqnum = 42;
    fix44::types::PRICE price = 123.45;
    fix44::types::QTY quantity = 100.0;
    fix44::types::UTCDATEONLY date = 20260515;

    std::cout << "Successfully integrated with FIX4.4 core types!\n";
    std::cout << "SeqNum: " << seqnum << "\n";
    std::cout << "Price: " << price << "\n";
    std::cout << "Quantity: " << quantity << "\n";
    std::cout << "Date: " << date << "\n";

    return 0;
}

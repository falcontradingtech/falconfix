// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <string>
#include <vector>
#include "expected.h"

namespace utils {

class IFile {
public:
    virtual expected<std::vector<char>, std::string> read(const std::string &fileName) = 0;
    virtual expected<void, std::string> write(const std::string &fileName, const std::vector<char> &data) = 0;
};

}

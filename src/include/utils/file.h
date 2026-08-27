// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <fstream>

#include "ifile.h"

namespace utils {
class File : public IFile {
public:
    expected<std::vector<char>, std::string> read(const std::string &fileName) override {
        std::ifstream stream(fileName, std::ios::binary);
        if (!stream.is_open())
            return unexpected("Cannot open file: " + fileName);
        return std::vector<char>{std::istreambuf_iterator<char>(stream),
                                 std::istreambuf_iterator<char>()};
    }

    expected<void, std::string> write(const std::string &fileName,
                                           const std::vector<char> &data) override {
        std::ofstream stream(fileName, std::ios::binary);
        if (!stream.is_open())
            return unexpected("Cannot open file: " + fileName);

        auto &writtenStream = stream.write(data.data(), data.size());
        if (!writtenStream)
            return unexpected("Cannot write file: " + fileName);

        return {};
    }
};
}

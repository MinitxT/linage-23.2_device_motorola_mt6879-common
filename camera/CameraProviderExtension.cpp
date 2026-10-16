/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "CameraProviderExtension.h"

#include <fstream>
#include <sstream>
#include <unistd.h>

static const std::string kTorchBrightnessPath =
    "/sys/devices/virtual/flashlight_core/flashlight/flashlight_torch_level";

template <typename T>
static void writeValue(const std::string& path, const T& value) {
    std::ofstream file(path);
    file << value;
}

static int32_t readTorchStrength() {
    std::ifstream file(kTorchBrightnessPath);
    if (!file.is_open()) {
        return 0;
    }
    int32_t type, ct, part, level, max_level;
    file >> type >> ct >> part >> level >> max_level;
    return file.fail() ? 0 : level;
}

static void writeTorchStrength(int32_t torchStrength, bool enabled) {
    std::ofstream file(kTorchBrightnessPath);
    if (!file.is_open()) {
        return;
    }
    int32_t val = (enabled && torchStrength > 0) ? torchStrength : 0;
    file << "0 0 0 " << val << std::endl;
}

bool supportsTorchStrengthControlExt() {
    return access(kTorchBrightnessPath.c_str(), W_OK) == 0;
}

int32_t getTorchDefaultStrengthLevelExt() {
    return 3;
}

int32_t getTorchMaxStrengthLevelExt() {
    return 7;
}

int32_t getTorchStrengthLevelExt() {
    return readTorchStrength();
}

void setTorchStrengthLevelExt(int32_t torchStrength, bool enabled) {
    writeTorchStrength(torchStrength, enabled);
}
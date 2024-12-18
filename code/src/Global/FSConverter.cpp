#include "FSConverter.h"
#include <chrono>
#include <filesystem>
#include <iostream>
#include <thread>
#include <unistd.h>

std::string FSConverter::mCachedResourceDir;

FSConverter::FSConverter(std::string ResourceDir) {
    if (!mCachedResourceDir.empty()) {
        mResourceDir = mCachedResourceDir;
    } else {
        mResourceDir = ResourceDir;
        if (mResourceDir.empty()) {
            mResourceDir = findResourcesFolder();
        }

        if (mResourceDir.empty()) {
            std::cerr << "Error: Could not locate /Resources folder!" << std::endl;
            throw std::runtime_error("Resources folder not found.");
        } else {
            mCachedResourceDir = mResourceDir;
        }
    }
}

std::string FSConverter::findResourcesFolder() {
    std::filesystem::path execPath = executablePath();

    // Try to find the Resources folder in a few places relative to the executable
    std::filesystem::path potentialPaths[] = {
        execPath / "Resources",         // Resources in the same directory as the executable
        execPath / "../Resources",      // Resources in the parent directory of the executable
        execPath / "../../Resources",   // Resources two levels up (in case of a bin/debug layout)
        execPath / "../../../Resources" // Resources two levels up (in case of a bin/debug layout)
    };

    for (const auto& path : potentialPaths) {
        if (std::filesystem::exists(path)) {
            return path.string();
        }
    }

    std::cerr << "Resources folder not found in any of the checked paths." << std::endl;
    return ""; // Resources folder not found
}

std::string FSConverter::getResourcePath(const std::string& resourceName, bool aCheckExists) {
    std::filesystem::path fullPath = std::filesystem::path(mResourceDir) / resourceName;

    if (aCheckExists && !std::filesystem::exists(fullPath)) {
        std::cerr << "Error: Resource " << resourceName << " does not exist at " << fullPath.string() << std::endl;
        return "";
    }

    return fullPath.string();
}

std::string FSConverter::executablePath() {
    char path[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
    if (count == -1) {
        throw std::runtime_error("Unable to determine the executable path.");
    }
    return std::filesystem::path(path).parent_path().string();
}

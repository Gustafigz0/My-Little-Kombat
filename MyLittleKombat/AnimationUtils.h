#pragma once
#include "Animation.h"
#include <string>
#include <vector>
#include <sstream>

inline Animation CreateAnimation(const std::string& folder, int frameCount, float frameTime, bool loop = true, const std::string& pattern = "{i}.png") {
    std::vector<std::string> files;
    for (int i = 0; i < frameCount; ++i) {
        std::ostringstream oss;
        size_t pos = pattern.find("{i}");
        if (pos != std::string::npos) {
            oss << folder << "/";
            oss << pattern.substr(0, pos) << i << pattern.substr(pos + 3);
        } else {
            oss << folder << "/" << pattern;
        }
        files.push_back(oss.str());
    }
    return Animation(files, frameTime, loop);
}

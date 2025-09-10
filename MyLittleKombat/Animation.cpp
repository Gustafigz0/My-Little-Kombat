#include "Animation.h"
#include <iostream>

Animation::Animation(const std::vector<std::string>& frameFiles, float frameTime, bool loop)
    : currentFrame(0), frameTime(frameTime), timer(0.0f), loop(loop)
{
    for (const auto& file : frameFiles) {
        Texture2D tex = LoadTexture(file.c_str());
        if (tex.id == 0) {
            std::cerr << "Erro ao carregar textura: " << file << std::endl;
        }
        frames.push_back(tex);
    }
}

Animation::~Animation() {
    for (auto& tex : frames) {
        if (tex.id != 0) {
            UnloadTexture(tex);
        }
    }
}

void Animation::Update() {
    if (frames.empty()) return;
    timer += GetFrameTime();
    if (timer >= frameTime) {
        timer = 0.0f;
        if (loop) {
            currentFrame = (currentFrame + 1) % frames.size();
        }
        else {
            if (currentFrame < static_cast<int>(frames.size()) - 1) {
                currentFrame++;
            }
        }
    }
}

void Animation::Draw(int x, int y) {
    if (!frames.empty() && frames[currentFrame].id != 0) {
        DrawTexture(frames[currentFrame], x, y, WHITE);
    }
}

void Animation::Reset() {
    currentFrame = 0;
    timer = 0.0f;
}

bool Animation::GetFrameSize(int& width, int& height) const {
    if (!frames.empty() && frames[0].id != 0) {
        width = frames[0].width;
        height = frames[0].height;
        return true;
    }
    width = 0;
    height = 0;
    return false;
}

bool Animation::IsFinished() const {
    return !loop && currentFrame == static_cast<int>(frames.size()) - 1;
}

int Animation::GetCurrentFrameHeight() const {
    if (!frames.empty() && frames[currentFrame].id != 0) {
        return frames[currentFrame].height;
    }
    return 0;
}
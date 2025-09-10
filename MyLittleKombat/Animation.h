#pragma once
#include "raylib.h"
#include <vector>
#include <string>
#include <memory>

// Classe para animação de sprites
class Animation {
public:
    Animation(const std::vector<std::string>& frameFiles, float frameTime, bool loop = true);
    ~Animation();

    void Update();
    void Draw(int x, int y);
    void Reset();
    bool GetFrameSize(int& width, int& height) const;
    const Texture2D& GetCurrentFrameTexture() const {
        return frames[currentFrame];
    }
    bool HasFrames() const {
        return !frames.empty();
    }
    bool IsFinished() const;
    int GetCurrentFrameHeight() const;
    const std::vector<Texture2D>& GetFrames() const { return frames; }
    void SetCurrentFrame(int frame) {
        if (frame >= 0 && frame < static_cast<int>(frames.size())) {
            currentFrame = frame;
        }
    }

private:
    std::vector<Texture2D> frames;
    int currentFrame;
    float frameTime;
    float timer;
    bool loop;
};
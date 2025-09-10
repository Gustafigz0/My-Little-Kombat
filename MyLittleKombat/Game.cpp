#include "Game.h"
#include "raylib.h"
#include "Animation.h"
#include "AnimationUtils.h"
#include "Character.h"
#include "Config.h"
#include <vector>
#include <string>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <iostream>

Game::Game() {}

Game::~Game() {}

void Game::Run() {
    InitWindow(Config::ScreenWidth, Config::ScreenHeight, Config::WindowTitle);

    // Define o ícone da janela
    Image icon = LoadImage("assets/logo/0.png");
    if (icon.data != nullptr) {
        ImageResize(&icon, 32, 32); // Garante tamanho 32x32
        SetWindowIcon(icon);
    } else {
        std::cerr << "\033[31mErro ao carregar ícone: assets/logo/0.png\033[0m" << std::endl;
    }
    UnloadImage(icon);

    SetTargetFPS(Config::TargetFPS);

    InitAudioDevice();
    Music theme = LoadMusicStream(Config::ThemeMusic);
    if (theme.stream.buffer == nullptr) {
        std::cerr << "Erro ao carregar música: " << Config::ThemeMusic << std::endl;
    }
    else {
        PlayMusicStream(theme);
    }

    srand((unsigned int)time(nullptr));

    // Background animation
    Animation background = CreateAnimation("assets/background", 27, 0.1f);

    // Character
    Character player(100, 305);

    // Enemy character
    Character enemy(600, 305);

    // Enemy idle animation
    Animation enemyIdle = CreateAnimation("assets/enemy/idle", 48, 0.12f, true, "aframe_{i}_delay-0.04s.png");

    // Idle animation
    Animation playerIdle = CreateAnimation("assets/player/parado", 7, 0.12f);

    // Walk left animation
    Animation playerWalkLeft = CreateAnimation("assets/player/A", 8, 0.10f);

    // Walk right animation
    Animation playerWalkRight = CreateAnimation("assets/player/D", 8, 0.10f);

    // Jump animation
    Animation playerJump = CreateAnimation("assets/player/W", 18, 0.06f, false);

    // Crouch animation
    Animation playerCrouch = CreateAnimation("assets/player/S", 3, 0.08f, false);

    // Attack cycle
    Animation playerAttack1 = CreateAnimation("assets/player/attack1", 6, 0.1f, false);
    Animation playerAttack2 = CreateAnimation("assets/player/attack2", 7, 0.1f, false);
    Animation playerAttack3 = CreateAnimation("assets/player/attack3", 7, 0.1f, false);

    // Kicking animations
    Animation playerKicking1 = CreateAnimation("assets/player/kicking1", 10, 0.08f, false);
    Animation playerKicking2 = CreateAnimation("assets/player/kicking2", 9, 0.08f, false);

    // Kicking fly animation
    Animation playerKickingFly = CreateAnimation("assets/player/kickingFLY", 3, 0.15f, false);

    // Crouch attack animations
    Animation playerCrouchAttack = CreateAnimation("assets/player/attackS", 3, 0.2f, false);
    Animation playerCrouchAttack1 = CreateAnimation("assets/player/attackS1", 7, 0.09f, false);

    // Crouch kick animations
    Animation playerCrouchKick = CreateAnimation("assets/player/kicingS", 10, 0.07f, false);
    Animation playerCrouchKick1 = CreateAnimation("assets/player/kicingS1", 9, 0.07f, false);

    // Set animations for the player
    player.SetIdleAnimation(std::make_shared<Animation>(playerIdle));
    player.SetWalkLeftAnimation(std::make_shared<Animation>(playerWalkLeft));
    player.SetWalkRightAnimation(std::make_shared<Animation>(playerWalkRight));
    player.SetJumpAnimation(std::make_shared<Animation>(playerJump));
    player.SetCrouchAnimation(std::make_shared<Animation>(playerCrouch));
    player.SetAttackAnimations(
        std::make_shared<Animation>(playerAttack1),
        std::make_shared<Animation>(playerAttack2),
        std::make_shared<Animation>(playerAttack3)
    );
    player.SetKickAnimations(
        std::make_shared<Animation>(playerKicking1),
        std::make_shared<Animation>(playerKicking2)
    );
    player.SetKickFlyAnimation(std::make_shared<Animation>(playerKickingFly));
    player.SetCrouchAttackAnimations(
        std::make_shared<Animation>(playerCrouchAttack),
        std::make_shared<Animation>(playerCrouchAttack1)
    );
    player.SetCrouchKickAnimations(
        std::make_shared<Animation>(playerCrouchKick),
        std::make_shared<Animation>(playerCrouchKick1)
    );

    // Set animations for the enemy
    enemy.SetIdleAnimation(std::make_shared<Animation>(enemyIdle));


    while (!WindowShouldClose()) {
        bool movingLeft = IsKeyDown(KEY_A);
        bool movingRight = IsKeyDown(KEY_D);
        bool jumping = IsKeyPressed(KEY_W) || IsKeyPressed(KEY_SPACE);
        bool crouching = IsKeyDown(KEY_S);
        bool attacking = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        bool kicking = IsMouseButtonPressed(MOUSE_RIGHT_BUTTON);

        // Logging pressed keys
        std::string log = "Pressed:";
        if (movingLeft) log += " A";
        if (movingRight) log += " D";
        if (IsKeyPressed(KEY_W)) log += " W";
        if (IsKeyPressed(KEY_SPACE)) log += " Space";
        if (crouching) log += " S";
        if (attacking) log += " MouseLeft";
        if (kicking) log += " MouseRight";
        if (log != "Pressed:") {
            printf("%s\n", log.c_str());
        }

        // Handle attack sprite display
        if (attacking && !player.IsAttacking() && !player.IsKicking() && !crouching) {
            int attackIndex = rand() % 3;
            player.StartAttack(attackIndex);
        }
        // Chute: só permite se não estiver atacando
        if (kicking && !player.IsKicking() && !player.IsAttacking() && !player.IsKickingFly() && !crouching) {
            // Chute no ar
            if (player.isJumping) {
                player.StartKickFly();
            } else {
                int kickIndex = rand() % 2;
                player.StartKick(kickIndex);
            }
        }
        // Ataque agachado: só permite se estiver agachado e não estiver atacando ou chutando
        if (crouching && attacking && !player.IsAttacking() && !player.IsCrouchAttacking() && !player.IsKicking() && !player.IsKickingFly() && !player.IsCrouchKicking()) {
            int idx = rand() % 2;
            player.StartCrouchAttack(idx);
        }
        // Chute agachado: só permite se estiver agachado e não estiver atacando ou chutando
        if (crouching && kicking && !player.IsAttacking() && !player.IsCrouchAttacking() && !player.IsKicking() && !player.IsKickingFly() && !player.IsCrouchKicking()) {
            int idx = rand() % 2;
            player.StartCrouchKick(idx);
        }

        background.Update();
        player.Update(movingLeft, movingRight, jumping, crouching, attacking, kicking);
        enemy.Update(false, false, false, false, false, false);

        enemyIdle.Update();

        UpdateMusicStream(theme);

        BeginDrawing();
        ClearBackground(BLACK);
        background.Draw(0, 0);

        player.Draw();

        if (enemyIdle.HasFrames()) {
            float scale = 0.20f;
            Texture2D tex = enemyIdle.GetCurrentFrameTexture();
            int frameWidth = tex.width;
            int frameHeight = tex.height;
            int scaledWidth = static_cast<int>(frameWidth * scale);
            int scaledHeight = static_cast<int>(frameHeight * scale);

            int drawX = enemy.posX;
            int drawY = enemy.posY;
            if (drawX + scaledWidth > Config::ScreenWidth) {
                drawX = Config::ScreenWidth - scaledWidth;
            }
            if (drawY + scaledHeight > Config::ScreenHeight) {
                drawY = Config::ScreenHeight - scaledHeight;
            }
            if (drawX < 0) drawX = 0;
            if (drawY < 0) drawY = 0;

            Vector2 position = { static_cast<float>(drawX), static_cast<float>(drawY) };
            DrawTextureEx(
                tex,
                position,
                0.0f,
                scale,
                WHITE
            );
        }

        Rectangle hitbox = player.GetHitbox();
        DrawRectangleLines(
            static_cast<int>(hitbox.x),
            static_cast<int>(hitbox.y),
            static_cast<int>(hitbox.width),
            static_cast<int>(hitbox.height),
            GREEN
        );

        EndDrawing();
    }

    if (theme.stream.buffer != nullptr) {
        UnloadMusicStream(theme);
    }
    CloseAudioDevice();
    CloseWindow();
}
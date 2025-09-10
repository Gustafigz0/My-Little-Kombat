#pragma once
#include "Animation.h"
#include <vector>
#include <string>
#include <memory>

// Estados possíveis do personagem
enum class CharacterState {
    Idle,
    MovingLeft,
    MovingRight,
    Jumping,
    Crouching
};

// Classe que representa um personagem do jogo
class Character {
public:
    Character(int x, int y);

    void SetIdleAnimation(std::shared_ptr<Animation> anim);
    void SetWalkLeftAnimation(std::shared_ptr<Animation> anim);
    void SetWalkRightAnimation(std::shared_ptr<Animation> anim);
    void SetJumpAnimation(std::shared_ptr<Animation> anim);
    void SetCrouchAnimation(std::shared_ptr<Animation> anim);

    // Múltiplos ataques
    void SetAttackAnimations(std::shared_ptr<Animation> attack1, std::shared_ptr<Animation> attack2, std::shared_ptr<Animation> attack3);
    void StartAttack(int attackIndex);
    bool IsAttacking() const;

    void SetKickAnimations(std::shared_ptr<Animation> kick1, std::shared_ptr<Animation> kick2);
    void StartKick(int kickIndex);
    bool IsKicking() const;

    void SetKickFlyAnimation(std::shared_ptr<Animation> kickFlyAnim);
    void StartKickFly();
    bool IsKickingFly() const;

    void SetCrouchAttackAnimations(std::shared_ptr<Animation> attackS, std::shared_ptr<Animation> attackS1);
    void StartCrouchAttack(int index);
    bool IsCrouchAttacking() const;

    void SetCrouchKickAnimations(std::shared_ptr<Animation> kickS, std::shared_ptr<Animation> kickS1);
    void StartCrouchKick(int index);
    bool IsCrouchKicking() const;

    void Update(bool moveLeft, bool moveRight, bool jump, bool crouch, bool attack, bool kick);
    void Draw();

    void MoveLeft();
    void MoveRight();
    void Jump();
    void Stop();

    int GetX() const { return posX; }
    int GetY() const { return posY; }
    Rectangle GetHitbox() const;

    int posX, posY;
    int speed;
    CharacterState state;

    float CrouchOffset = 61.0f;

    // Torna isJumping público para uso externo (Game.cpp)
    bool isJumping = false; 

private:
    std::shared_ptr<Animation> idleAnimation;
    std::shared_ptr<Animation> walkLeftAnimation;
    std::shared_ptr<Animation> walkRightAnimation;
    std::shared_ptr<Animation> jumpAnimation;
    std::shared_ptr<Animation> crouchAnimation;

    // Múltiplos ataques
    std::shared_ptr<Animation> attackAnimations[3];
    int currentAttackIndex = -1;
    bool isAttacking = false;

    std::shared_ptr<Animation> kickAnimations[2];
    int currentKickIndex = -1;
    bool isKicking = false;

    std::shared_ptr<Animation> kickFlyAnimation;
    bool isKickingFly = false;

    std::shared_ptr<Animation> crouchAttackAnimations[2];
    int currentCrouchAttackIndex = -1;
    bool isCrouchAttacking = false;

    std::shared_ptr<Animation> crouchKickAnimations[2];
    int currentCrouchKickIndex = -1;
    bool isCrouchKicking = false;

    // Controle de pulo
    bool isLanding = false;
    float landingTimer = 0.0f;
    const float landingDuration = 0.2f;
    float velocityY = 0.0f;
    float gravity = 0.5f;
    int groundY;

    bool wasCrouching = false;
    bool showJumpLandFrame = false;
    float jumpLandTimer = 0.0f;
    const float jumpLandFrameDuration = 0.4f;
};
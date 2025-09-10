#include "Character.h"
#include "raylib.h"

Character::Character(int x, int y)
    : posX(x), posY(y), speed(3), state(CharacterState::Idle), groundY(y) {
}

void Character::SetIdleAnimation(std::shared_ptr<Animation> anim) {
    idleAnimation = anim;
}
void Character::SetWalkLeftAnimation(std::shared_ptr<Animation> anim) {
    walkLeftAnimation = anim;
}
void Character::SetWalkRightAnimation(std::shared_ptr<Animation> anim) {
    walkRightAnimation = anim;
}
void Character::SetJumpAnimation(std::shared_ptr<Animation> anim) {
    jumpAnimation = anim;
}
void Character::SetCrouchAnimation(std::shared_ptr<Animation> anim) {
    crouchAnimation = anim;
}

void Character::SetAttackAnimations(std::shared_ptr<Animation> attack1, std::shared_ptr<Animation> attack2, std::shared_ptr<Animation> attack3) {
    attackAnimations[0] = attack1;
    attackAnimations[1] = attack2;
    attackAnimations[2] = attack3;
}

void Character::SetKickAnimations(std::shared_ptr<Animation> kick1, std::shared_ptr<Animation> kick2) {
    kickAnimations[0] = kick1;
    kickAnimations[1] = kick2;
}

void Character::SetKickFlyAnimation(std::shared_ptr<Animation> kickFlyAnim) {
    kickFlyAnimation = kickFlyAnim;
}

void Character::SetCrouchAttackAnimations(std::shared_ptr<Animation> attackS, std::shared_ptr<Animation> attackS1) {
    crouchAttackAnimations[0] = attackS;
    crouchAttackAnimations[1] = attackS1;
}

void Character::SetCrouchKickAnimations(std::shared_ptr<Animation> kickS, std::shared_ptr<Animation> kickS1) {
    crouchKickAnimations[0] = kickS;
    crouchKickAnimations[1] = kickS1;
}

void Character::StartAttack(int attackIndex) {
    if (attackIndex >= 0 && attackIndex < 3 && attackAnimations[attackIndex]) {
        currentAttackIndex = attackIndex;
        attackAnimations[attackIndex]->Reset();
        isAttacking = true;
    }
}

void Character::StartKick(int kickIndex) {
    if (kickIndex >= 0 && kickIndex < 2 && kickAnimations[kickIndex]) {
        currentKickIndex = kickIndex;
        kickAnimations[kickIndex]->Reset();
        isKicking = true;
    }
}

void Character::StartKickFly() {
    if (kickFlyAnimation && isJumping && !isKickingFly && !isKicking && !isAttacking) {
        kickFlyAnimation->Reset();
        isKickingFly = true;
    }
}

void Character::StartCrouchAttack(int index) {
    if (index >= 0 && index < 2 && crouchAttackAnimations[index]) {
        currentCrouchAttackIndex = index;
        crouchAttackAnimations[index]->Reset();
        isCrouchAttacking = true;
    }
}

void Character::StartCrouchKick(int index) {
    if (index >= 0 && index < 2 && crouchKickAnimations[index]) {
        currentCrouchKickIndex = index;
        crouchKickAnimations[index]->Reset();
        isCrouchKicking = true;
    }
}

bool Character::IsAttacking() const {
    return isAttacking;
}

bool Character::IsKicking() const {
    return isKicking;
}

bool Character::IsKickingFly() const {
    return isKickingFly;
}

bool Character::IsCrouchAttacking() const {
    return isCrouchAttacking;
}

bool Character::IsCrouchKicking() const {
    return isCrouchKicking;
}

void Character::Update(bool moveLeft, bool moveRight, bool jump, bool crouch, bool attack, bool kick) {
    // Chute agachado tem prioridade máxima
    if (isCrouchKicking && currentCrouchKickIndex >= 0 && currentCrouchKickIndex < 2 && crouchKickAnimations[currentCrouchKickIndex]) {
        crouchKickAnimations[currentCrouchKickIndex]->Update();
        if (crouchKickAnimations[currentCrouchKickIndex]->IsFinished()) {
            isCrouchKicking = false;
            currentCrouchKickIndex = -1;
        }
        if (moveLeft) posX -= speed;
        if (moveRight) posX += speed;
        return;
    }
    // Ataque agachado
    if (isCrouchAttacking && currentCrouchAttackIndex >= 0 && currentCrouchAttackIndex < 2 && crouchAttackAnimations[currentCrouchAttackIndex]) {
        crouchAttackAnimations[currentCrouchAttackIndex]->Update();
        if (crouchAttackAnimations[currentCrouchAttackIndex]->IsFinished()) {
            isCrouchAttacking = false;
            currentCrouchAttackIndex = -1;
        }
        if (moveLeft) posX -= speed;
        if (moveRight) posX += speed;
        return;
    }
    // Chute
    if (isKicking && currentKickIndex >= 0 && currentKickIndex < 2 && kickAnimations[currentKickIndex]) {
        kickAnimations[currentKickIndex]->Update();
        if (kickAnimations[currentKickIndex]->IsFinished()) {
            isKicking = false;
            currentKickIndex = -1;
        }
        if (moveLeft) posX -= speed;
        if (moveRight) posX += speed;
        if (isJumping) {
            velocityY += gravity;
            posY += static_cast<int>(velocityY);
            if (posY >= groundY) {
                posY = groundY;
                isJumping = false;
                isLanding = true;
                landingTimer = 0.0f;
                if (jumpAnimation) {
                    jumpAnimation->SetCurrentFrame(static_cast<int>(jumpAnimation->GetFrames().size()) - 1);
                }
            }
        }
        return;
    }
    // Chute no ar (kickFly)
    if (isKickingFly && kickFlyAnimation) {
        if (!kickFlyAnimation->IsFinished()) {
            kickFlyAnimation->Update();
        }
        if (kickFlyAnimation->IsFinished() && isJumping) {
            // Mantém último frame
        } else if (kickFlyAnimation->IsFinished() && !isJumping) {
            isKickingFly = false;
        }
        if (moveLeft) posX -= speed;
        if (moveRight) posX += speed;
        if (isJumping) {
            velocityY += gravity;
            posY += static_cast<int>(velocityY);
            if (posY >= groundY) {
                posY = groundY;
                isJumping = false;
                isLanding = true;
                landingTimer = 0.0f;
                if (jumpAnimation) {
                    jumpAnimation->SetCurrentFrame(static_cast<int>(jumpAnimation->GetFrames().size()) - 1);
                }
            }
        }
        return;
    }
    // Ataque normal
    if (isAttacking && currentAttackIndex >= 0 && currentAttackIndex < 3 && attackAnimations[currentAttackIndex]) {
        attackAnimations[currentAttackIndex]->Update();
        if (attackAnimations[currentAttackIndex]->IsFinished()) {
            isAttacking = false;
            currentAttackIndex = -1;
        }
        if (moveLeft) posX -= speed;
        if (moveRight) posX += speed;
        if (isJumping) {
            velocityY += gravity;
            posY += static_cast<int>(velocityY);
            if (posY >= groundY) {
                posY = groundY;
                isJumping = false;
                isLanding = true;
                landingTimer = 0.0f;
                if (jumpAnimation) {
                    jumpAnimation->SetCurrentFrame(static_cast<int>(jumpAnimation->GetFrames().size()) - 1);
                }
            }
        }
        return;
    }
    // Início dos estados normais
    if (isJumping) {
        if (jumpAnimation) jumpAnimation->Update();
        velocityY += gravity;
        posY += static_cast<int>(velocityY);
        if (posY >= groundY) {
            posY = groundY;
            isJumping = false;
            isLanding = true;
            landingTimer = 0.0f;
            if (jumpAnimation) {
                jumpAnimation->SetCurrentFrame(static_cast<int>(jumpAnimation->GetFrames().size()) - 1);
            }
        } else {
            state = CharacterState::Jumping;
        }
    }
    if (isLanding) {
        landingTimer += GetFrameTime();
        if (landingTimer >= landingDuration) {
            isLanding = false;
            landingTimer = 0.0f;
            state = CharacterState::Idle;
            if (jumpAnimation) jumpAnimation->Reset();
        }
        if (moveLeft) posX -= speed;
        if (moveRight) posX += speed;
        return;
    }
    if (jump) {
        Jump();
    } else if (crouch) {
        state = CharacterState::Crouching;
        if (!wasCrouching && crouchAnimation) {
            crouchAnimation->Reset();
        }
        wasCrouching = true;
        if (crouchAnimation && !crouchAnimation->IsFinished()) {
            crouchAnimation->Update();
        }
    } else {
        wasCrouching = false;
        if (moveLeft) {
            state = CharacterState::MovingLeft;
            posX -= speed;
            if (walkLeftAnimation) walkLeftAnimation->Update();
        } else if (moveRight) {
            state = CharacterState::MovingRight;
            posX += speed;
            if (walkRightAnimation) walkRightAnimation->Update();
        } else {
            state = CharacterState::Idle;
            if (idleAnimation) idleAnimation->Update();
        }
    }
}

void Character::Draw() {
    // Desenha chute no ar se estiver ativo
    if (isKickingFly && kickFlyAnimation) {
        if (kickFlyAnimation->IsFinished()) {
            // Desenha o último frame enquanto estiver no ar
            const auto& frames = kickFlyAnimation->GetFrames();
            if (!frames.empty()) {
                DrawTexture(frames.back(), posX, posY, WHITE);
            }
        } else {
            kickFlyAnimation->Draw(posX, posY);
        }
        return;
    }
    // Desenha chute se estiver ativo
    if (isKicking && currentKickIndex >= 0 && currentKickIndex < 2 && kickAnimations[currentKickIndex]) {
        kickAnimations[currentKickIndex]->Draw(posX, posY);
        return;
    }
    // Desenha ataque normal se estiver ativo
    if (isAttacking && currentAttackIndex >= 0 && currentAttackIndex < 3 && attackAnimations[currentAttackIndex]) {
        attackAnimations[currentAttackIndex]->Draw(posX, posY);
        return;
    }
    // Desenha ataque agachado se estiver ativo
    if (isCrouchAttacking && currentCrouchAttackIndex >= 0 && currentCrouchAttackIndex < 2 && crouchAttackAnimations[currentCrouchAttackIndex]) {
        int idleW = 0, idleH = 0;
        if (idleAnimation) idleAnimation->GetFrameSize(idleW, idleH);
        int atkH = crouchAttackAnimations[currentCrouchAttackIndex]->GetCurrentFrameHeight();
        int yBase = posY + (idleH - atkH);
        crouchAttackAnimations[currentCrouchAttackIndex]->Draw(posX, yBase);
        return;
    }
    // Desenha chute agachado se estiver ativo
    if (isCrouchKicking && currentCrouchKickIndex >= 0 && currentCrouchKickIndex < 2 && crouchKickAnimations[currentCrouchKickIndex]) {
        int idleW = 0, idleH = 0;
        if (idleAnimation) idleAnimation->GetFrameSize(idleW, idleH);
        int kickH = crouchKickAnimations[currentCrouchKickIndex]->GetCurrentFrameHeight();
        int yBase = posY + (idleH - kickH);
        crouchKickAnimations[currentCrouchKickIndex]->Draw(posX, yBase);
        return;
    }

    if ((isJumping || isLanding) && jumpAnimation) {
        int idleW = 0, idleH = 0;
        if (idleAnimation) idleAnimation->GetFrameSize(idleW, idleH);
        int jumpW = jumpAnimation->GetCurrentFrameTexture().width;
        int jumpH = jumpAnimation->GetCurrentFrameTexture().height;
        int yBase = posY + (idleH - jumpH);

        if (isLanding) {
            const auto& frames = jumpAnimation->GetFrames();
            if (!frames.empty()) {
                DrawTexture(frames.back(), posX, yBase, WHITE);
            }
        }
        else {
            jumpAnimation->Draw(posX, yBase);
        }
        return;
    }

    int idleW = 0, idleH = 0;
    if (idleAnimation) idleAnimation->GetFrameSize(idleW, idleH);

    if (state == CharacterState::Crouching) {
        if (crouchAnimation) {
            int crouchFrameH = crouchAnimation->GetCurrentFrameHeight();
            int yBase = posY + (idleH - crouchFrameH);
            crouchAnimation->Draw(posX, yBase);
        }
    }
    else if (state == CharacterState::Jumping) {
        if (jumpAnimation) jumpAnimation->Draw(posX, posY);
    }
    else if (state == CharacterState::MovingLeft) {
        if (walkLeftAnimation) walkLeftAnimation->Draw(posX, posY);
    }
    else if (state == CharacterState::MovingRight) {
        if (walkRightAnimation) walkRightAnimation->Draw(posX, posY);
    }
    else {
        if (idleAnimation) idleAnimation->Draw(posX, posY);
    }
}

void Character::MoveLeft() {
    state = CharacterState::MovingLeft;
}
void Character::MoveRight() {
    state = CharacterState::MovingRight;
}
void Character::Jump() {
    if (!isJumping && !isLanding) {
        isJumping = true;
        velocityY = -10.0f;
        state = CharacterState::Jumping;
        if (jumpAnimation) jumpAnimation->Reset();
    }
}

void Character::Stop() {
    // Exemplo: reseta o estado para Idle
    state = CharacterState::Idle;
    // Se quiser, pode adicionar lógica para cancelar ataques, pulo, etc.
    // isAttacking = false;
    // isJumping = false;
    // currentAttackIndex = -1;
}

Rectangle Character::GetHitbox() const {
    int width = 50;
    int height = 80;
    float offsetY = 0.0f;

    // Prioridade correta para ataque agachado
    const Animation* currentAnim = nullptr;
    if (isCrouchAttacking && currentCrouchAttackIndex >= 0 && currentCrouchAttackIndex < 2 && crouchAttackAnimations[currentCrouchAttackIndex]) {
        currentAnim = crouchAttackAnimations[currentCrouchAttackIndex].get();
        int idleW = 0, idleH = 0;
        if (idleAnimation) idleAnimation->GetFrameSize(idleW, idleH);
        int atkH = crouchAttackAnimations[currentCrouchAttackIndex]->GetCurrentFrameHeight();
        offsetY = static_cast<float>(idleH - atkH);
        height = atkH;
        width = crouchAttackAnimations[currentCrouchAttackIndex]->GetCurrentFrameTexture().width;
    } else if (isKicking && currentKickIndex >= 0 && currentKickIndex < 2 && kickAnimations[currentKickIndex]) {
        currentAnim = kickAnimations[currentKickIndex].get();
        width = currentAnim->GetCurrentFrameTexture().width;
        height = currentAnim->GetCurrentFrameTexture().height;
    } else if (isKickingFly && kickFlyAnimation) {
        currentAnim = kickFlyAnimation.get();
        width = currentAnim->GetCurrentFrameTexture().width;
        height = currentAnim->GetCurrentFrameTexture().height;
    } else if (isAttacking && currentAttackIndex >= 0 && currentAttackIndex < 3 && attackAnimations[currentAttackIndex]) {
        currentAnim = attackAnimations[currentAttackIndex].get();
        width = currentAnim->GetCurrentFrameTexture().width;
        height = currentAnim->GetCurrentFrameTexture().height;
    } else if ((isJumping || isLanding) && jumpAnimation) {
        currentAnim = jumpAnimation.get();
        int idleW = 0, idleH = 0;
        if (idleAnimation) idleAnimation->GetFrameSize(idleW, idleH);
        int jumpH = jumpAnimation->GetCurrentFrameTexture().height;
        offsetY = static_cast<float>(idleH - jumpH);
        width = currentAnim->GetCurrentFrameTexture().width;
        height = jumpH;
    } else if (state == CharacterState::Crouching && crouchAnimation) {
        currentAnim = crouchAnimation.get();
        int idleW = 0, idleH = 0;
        if (idleAnimation) idleAnimation->GetFrameSize(idleW, idleH);
        int crouchH = crouchAnimation->GetCurrentFrameHeight();
        offsetY = static_cast<float>(idleH - crouchH);
        height = crouchH;
        width = crouchAnimation->GetCurrentFrameTexture().width;
    } else if (state == CharacterState::MovingLeft && walkLeftAnimation) {
        currentAnim = walkLeftAnimation.get();
        width = currentAnim->GetCurrentFrameTexture().width;
        height = currentAnim->GetCurrentFrameTexture().height;
    } else if (state == CharacterState::MovingRight && walkRightAnimation) {
        currentAnim = walkRightAnimation.get();
        width = currentAnim->GetCurrentFrameTexture().width;
        height = currentAnim->GetCurrentFrameTexture().height;
    } else if (idleAnimation) {
        currentAnim = idleAnimation.get();
        width = currentAnim->GetCurrentFrameTexture().width;
        height = currentAnim->GetCurrentFrameTexture().height;
    }

    return Rectangle{
        static_cast<float>(posX),
        static_cast<float>(posY) + offsetY,
        static_cast<float>(width),
        static_cast<float>(height)
    };
}
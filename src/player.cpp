#include "player.h"
#include "materials.h"
#include "meshes.h"
#include "raymath.h"
#include "settings.h"

Entity *PlayerEntity;
float inputX, elapsedCoyoteTime, lastJumpTime, step = 0.0f, bodyRotation, leftLegRotation, rightLegRotation;
bool doubleJump, unlockedDoubleJump = false, grounded, wallClimbing = false, unlockedWallClimbing = true;
b2Vec2 playerPosition, playerVelocity, bodyPos, leftLegPos, rightLegPos, bodyUp = {0.0f, 1.0f};

float leftLegPosX(float step)
{
    if (step < 0.25f)
        return Normalize(step, 0.0f, 0.25f);
    else if (step < 0.5f)
        return 1.0f;
    else if (step < 0.75f)
        return 1.0f - Normalize(step, 0.5f, 0.75f);
    
    return 0.0f;
}

float leftLegPosY(float step)
{
    if (step < 0.25f)
        return 0.0f;
    else if (step < 0.5f)
        return Normalize(step, 0.25f, 0.5f);
    else if (step < 0.75f)
        return 1.0f;
    
    return 1.0f - Normalize(step, 0.75f, 1.0f);
}

float rightLegPosX(float step)
{
    if (step < 0.25f)
        return Normalize(step, 0.0f, 0.25f);
    else if (step < 0.5f)
        return 1.0f;
    else if (step < 0.75f)
        return 1.0f - Normalize(step, 0.5f, 0.75f);
    
    return 0.0f;
}

float rightLegPosY(float step)
{
    if (step < 0.25f)
        return 1.0f;
    else if (step < 0.5f)
        return 1.0f - Normalize(step, 0.25f, 0.5f);
    else if (step < 0.75f)
        return 0.0f;
    
    return Normalize(step, 0.75f, 1.0f);
}

void UpdatePlayerAnimation()
{
    if (grounded && fabs(playerVelocity.x) > 0.1f)
    {
        step += GetFrameTime() * speedMultiplier * 3.0f;
        while (step >= 1.0f) 
            step -= 1.0f;
    }
    
    bodyRotation = 10.0f * DEG2RAD * (playerVelocity.x / -(PLAYER_SPEED));
    
    bodyPos = {playerPosition.x, playerPosition.y + 0.05f * cosf(2.0f * PI * step)};

    //float legPosY = sin(2.0f * PI * step);

    // TraceLog(LOG_INFO, "Step: %.2f, PosX: %.2f, PosY: %.2f", step, leftLegPosX(step), leftLegPosY(step));
    leftLegPos = {bodyPos.x - 0.6f/* + 0.2f * leftLegPosX(step)*/, bodyPos.y - 0.5f + (fabs(playerVelocity.x) > 0.1f ? 0.15f * leftLegPosY(step) : 0.0f)};
    rightLegPos = {bodyPos.x + 0.6f, bodyPos.y - 0.5f + (fabs(playerVelocity.x) > 0.1f ? 0.15f * rightLegPosY(step) : 0.0f)};
}

bool WallClimb(Platform** platforms, int platformCount)
{
    for (int i = 0; i < platformCount; ++i)
    {
        b2Vec2 platformPosition = b2Body_GetPosition(platforms[i]->body), offset = 0.5f * platforms[i]->size;
        Rectangle platformRect = {platformPosition.x - offset.x, platformPosition.y - offset.y, platforms[i]->size.x, platforms[i]->size.y};
        
        //b2Vec2 playerPosition = b2Body_GetPosition(PlayerEntity->body);
        offset = {0.5f * PlayerEntity->size.x + 0.05f, 0.0f};
        Rectangle leftRect = {playerPosition.x - offset.x, playerPosition.y - offset.y, 0.1f, PlayerEntity->size.y * 0.8f},
            rightRect = {playerPosition.x + offset.x, playerPosition.y - offset.y, 0.1f, PlayerEntity->size.y * 0.8f};

        if (CheckCollisionRecs(platformRect, leftRect))
        {
            bodyUp = {1.0f, 0.0f};
            return true;
        }
        else if (CheckCollisionRecs(platformRect, rightRect))
        {
            bodyUp = {-1.0f, 0.0f};
            return true;
        }
    }

    bodyUp = {0.0f, 1.0f};
    return false;
}

bool IsGrounded(Platform** platforms, int platformCount)
{
    for (int i = 0; i < platformCount; ++i)
    {
        b2Vec2 platformPosition = b2Body_GetPosition(platforms[i]->body), offset = 0.5f * platforms[i]->size;
        Rectangle platformRect = {platformPosition.x - offset.x, platformPosition.y - offset.y, platforms[i]->size.x, platforms[i]->size.y};
        
        //b2Vec2 playerPosition = b2Body_GetPosition(PlayerEntity->body);
        offset = {0.5f * PlayerEntity->size.x, PlayerEntity->size.y / 2.0f + 0.05f};
        Rectangle groundedRect = {playerPosition.x - offset.x, playerPosition.y - offset.y, PlayerEntity->size.x, 0.1f};

        if (CheckCollisionRecs(platformRect, groundedRect))
            return true;
    }

    return false;
}

void Jump()
{
    lastJumpTime = GetTime();

    float jumpingVelocity = sqrtf(2.0f * (PLAYER_JUMP_HEIGHT) * GAME_GRAVITY);

    playerVelocity.y = jumpingVelocity;
}

void DrawPlayer()
{
    Matrix sizeMat = MatrixScale(PlayerEntity->size.x, PlayerEntity->size.x, 1.0f), 
        bodyTransform = MatrixRotateZ(bodyRotation) * MatrixTranslate(PIXELS_PER_UNIT * bodyPos.x, - PIXELS_PER_UNIT * bodyPos.y, 0.0f) * sizeMat,
        leftLegTransform  = MatrixRotateZ(leftLegRotation) * MatrixTranslate(PIXELS_PER_UNIT * leftLegPos.x, - PIXELS_PER_UNIT * leftLegPos.y, 0.0f) * sizeMat,
        rightLegTransform = MatrixRotateZ(rightLegRotation) * MatrixTranslate(PIXELS_PER_UNIT * rightLegPos.x, - PIXELS_PER_UNIT * rightLegPos.y, 0.0f) * sizeMat;

    DrawMesh(PlayerBodyMesh, PlayerMaterial, bodyTransform);
    DrawMesh(PlayerLegMesh, PlayerMaterial, leftLegTransform);
    DrawMesh(PlayerLegMesh, PlayerMaterial, rightLegTransform);
}

void DrawPlayerDebug()
{
    //b2Vec2 playerPosition = b2Body_GetPosition(PlayerEntity->body), 
    b2Vec2 offset = {0.5f * PlayerEntity->size.x, PlayerEntity->size.y / 2.0f + 0.05f};
    Rectangle groundedRect = {playerPosition.x - offset.x, playerPosition.y - offset.y, PlayerEntity->size.x, 0.1f};
    DrawRectangleLines(groundedRect.x * PIXELS_PER_UNIT, -groundedRect.y * PIXELS_PER_UNIT, groundedRect.width * PIXELS_PER_UNIT, groundedRect.height * PIXELS_PER_UNIT, grounded ? LIME : RED);

    offset = {0.5f * PlayerEntity->size.x + 0.05f, 0.0f};
    Rectangle leftRect = {playerPosition.x - offset.x, playerPosition.y - offset.y, 0.1f, PlayerEntity->size.y * 0.8f},
        rightRect = {playerPosition.x + offset.x, playerPosition.y - offset.y, 0.1f, PlayerEntity->size.y * 0.8f};
    DrawRectangleLines(leftRect.x * PIXELS_PER_UNIT, -leftRect.y * PIXELS_PER_UNIT, leftRect.width * PIXELS_PER_UNIT, leftRect.height * PIXELS_PER_UNIT, bodyUp == b2Vec2({1.0f, 0.0f}) ? LIME : RED);
    DrawRectangleLines(rightRect.x * PIXELS_PER_UNIT, -rightRect.y * PIXELS_PER_UNIT, rightRect.width * PIXELS_PER_UNIT, rightRect.height * PIXELS_PER_UNIT, bodyUp == b2Vec2({-1.0f, 0.0f}) ? LIME : RED);
}

void UpdatePlayer(Platform **platforms, int platformCount)
{
    if (PlayerEntity == nullptr || !b2Body_IsValid(PlayerEntity->body))
        return;

    playerPosition = b2Body_GetPosition(PlayerEntity->body);

    grounded = IsGrounded(platforms, platformCount);
    if (unlockedWallClimbing) wallClimbing = WallClimb(platforms, platformCount);
    playerVelocity = b2Body_GetLinearVelocity(PlayerEntity->body);

    UpdatePlayerAnimation();

    if (grounded) 
    {
        if (elapsedCoyoteTime != 0.0f)
            elapsedCoyoteTime = 0.0f;
            doubleJump = true;
    } 
    else 
    {
        elapsedCoyoteTime += GetFrameTime();
    }

    //float movingX = grounded ? 0.0f : playerVelocity.x;

    if (IsGamepadAvailable(PLAYER_GAMEPAD))
    {
        float axisValue = GetGamepadAxisMovement(PLAYER_GAMEPAD, GAMEPAD_AXIS_LEFT_X);
        
#if DEBUG
        const char* gamepadName = GetGamepadName(PLAYER_GAMEPAD);
        TraceLog(LOG_INFO, "Gamepad: %s, Axis: %.3f", gamepadName ? gamepadName : "Unknown", axisValue);
#endif
        
        if (fabs(axisValue) > 0.1f)
            inputX = axisValue;
    }
    else if ((IsKeyDown(movingLeftKey) || IsKeyDown(movingLeftKeySecondary)) && !(IsKeyDown(movingRightKey) || IsKeyDown(movingRightKeySecondary)))
        inputX -= INPUT_ACCELERATION;
    else if (!(IsKeyDown(movingLeftKey) || IsKeyDown(movingLeftKeySecondary)) && (IsKeyDown(movingRightKey) || IsKeyDown(movingRightKeySecondary)))
        inputX += INPUT_ACCELERATION;
    else
        if (fabs(inputX) < 0.1f)
            inputX = 0.0f;
        else
            inputX += inputX > 0.0f ? -INPUT_ACCELERATION : INPUT_ACCELERATION;

    inputX = Clamp(inputX, - 1.0f, 1.0f);

    if ((GetTime() - lastJumpTime > PLAYER_JUMP_DELAY) && (IsKeyDown(KEY_SPACE) || (IsGamepadAvailable(PLAYER_GAMEPAD) && IsGamepadButtonDown(PLAYER_GAMEPAD, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))) && (grounded || elapsedCoyoteTime < PLAYER_COYOTE_TIME || (unlockedDoubleJump && doubleJump)))
    {
        if (!(grounded || elapsedCoyoteTime < PLAYER_COYOTE_TIME)) 
            doubleJump = false;
        Jump();
    }

    if (bodyUp == b2Vec2({1.0f, 0.0f}) && !(grounded && inputX > 0.0f)) b2Body_SetLinearVelocity(PlayerEntity->body, {0.0f, -inputX * PLAYER_SPEED});
    else if (bodyUp == b2Vec2({-1.0f, 0.0f}) && !(grounded && inputX < 0.0f)) b2Body_SetLinearVelocity(PlayerEntity->body, {0.0f, inputX * PLAYER_SPEED});
    else b2Body_SetLinearVelocity(PlayerEntity->body, {inputX * PLAYER_SPEED, playerVelocity.y});
}

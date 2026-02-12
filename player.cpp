#include "player.h"

Player::Player() : transform(), spriteRenderer(&transform), collider(Collider()), rigidBody(&transform, &collider) {}

void Player::Initialize()
{
    
}

void Player::Update()
{
    spriteRenderer.Draw();
}

void Player::FixedUpdate()
{
    rigidBody.ApplyPhysics();
}

void Player::Finalize()
{

}
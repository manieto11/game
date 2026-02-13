#include "player.h"

Player::Player() : transform(), spriteRenderer(&transform), collider(Collider()), rigidBody(&transform, &collider), controller(&rigidBody) {}

void Player::Initialize()
{
    
}

void Player::Update()
{
    controller.Update();
    spriteRenderer.Draw();
}

void Player::FixedUpdate()
{
    rigidBody.ApplyPhysics();
}

void Player::Finalize()
{

}
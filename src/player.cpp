#include "player.h"
#include "settings.h"

Player::Player() : transform(new Transform2D()), spriteRenderer(new SpriteRenderer(transform)), collider(new BoxCollider(transform)), rigidBody(new RigidBody(transform, collider)), controller(new PlayerController(rigidBody)) 
{
    collider->rectangle.x = -0.5;
    collider->rectangle.y = -0.5;
}

Player::~Player()
{
    delete transform;
    delete spriteRenderer;
    delete collider;
    delete rigidBody;
    delete controller;
}

void Player::Initialize()
{
    
}

void Player::Update()
{
    controller->Update();
}

void Player::FixedUpdate()
{
    rigidBody->ApplyPhysics();
}

void Player::Draw()
{
    spriteRenderer->Draw();
}

void Player::Finalize()
{

}
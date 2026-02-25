#include "collider.h"
#include "settings.h"

BoxCollider::BoxCollider() : transform(nullptr), rectangle({0.0f, 0.0f, 0.0f, 0.0f}) {}

BoxCollider::BoxCollider(BoxCollider &other)  : transform(other.transform), rectangle(other.rectangle) {}

BoxCollider::BoxCollider(Transform2D *transform) : transform(transform), rectangle({0.0f, 0.0f, 1.0f, 1.0f}) {}

BoxCollider::BoxCollider(Transform2D *transform, Rectangle rect) : transform(transform), rectangle(rect) {}

bool BoxCollider::Colliding(const Vector2 point)
{
    // If no transform is set, use rectangle as-is (world space)
    if (transform == nullptr)
    {
        return (point.x >= rectangle.x && point.x <= rectangle.x + rectangle.width &&
                point.y >= rectangle.y && point.y <= rectangle.y + rectangle.height);
    }

    // Transform rectangle to world space using the transform
    float left = transform->position.x + rectangle.x;
    float right = left + rectangle.width * transform->scale.x;
    float bottom = transform->position.y + rectangle.y;
    float top = bottom + rectangle.height * transform->scale.y;

    // Check if point is within the transformed rectangle
    return (point.x >= left && point.x <= right &&
            point.y >= bottom && point.y <= top);
}

bool BoxCollider::Colliding(Collider *other)
{
    if (other == nullptr)
        return false;

    // Try to cast to BoxCollider for AABB collision
    BoxCollider *boxOther = dynamic_cast<BoxCollider*>(other);
    if (boxOther != nullptr)
    {
        // Get world space bounds for this collider
        float left1 = (transform != nullptr) ? transform->position.x + rectangle.x : rectangle.x;
        float right1 = left1 + (transform != nullptr ? rectangle.width * transform->scale.x : rectangle.width);
        float bottom1 = (transform != nullptr) ? transform->position.y + rectangle.y : rectangle.y;
        float top1 = bottom1 + (transform != nullptr ? rectangle.height * transform->scale.y : rectangle.height);

        // Get world space bounds for other collider
        float left2 = (boxOther->transform != nullptr) ? boxOther->transform->position.x + boxOther->rectangle.x : boxOther->rectangle.x;
        float right2 = left2 + (boxOther->transform != nullptr ? boxOther->rectangle.width * boxOther->transform->scale.x : boxOther->rectangle.width);
        float bottom2 = (boxOther->transform != nullptr) ? boxOther->transform->position.y + boxOther->rectangle.y : boxOther->rectangle.y;
        float top2 = bottom2 + (boxOther->transform != nullptr ? boxOther->rectangle.height * boxOther->transform->scale.y : boxOther->rectangle.height);

        // AABB collision check
        return !(right1 < left2 || left1 > right2 ||
                 top1 < bottom2 || bottom1 > top2);
    }

    // Try to cast to GridCollider
    GridCollider *gridOther = dynamic_cast<GridCollider*>(other);
    if (gridOther != nullptr)
    {
        // Check corners and center of box against grid
        float left = (transform != nullptr) ? transform->position.x + rectangle.x : rectangle.x;
        float right = left + (transform != nullptr ? rectangle.width * transform->scale.x : rectangle.width);
        float bottom = (transform != nullptr) ? transform->position.y + rectangle.y : rectangle.y;
        float top = bottom + (transform != nullptr ? rectangle.height * transform->scale.y : rectangle.height);

        // Check all four corners and center
        return (gridOther->Colliding({left, bottom}) ||
                gridOther->Colliding({right, bottom}) ||
                gridOther->Colliding({left, top}) ||
                gridOther->Colliding({right, top}) ||
                gridOther->Colliding({(left + right) / 2.0f, (bottom + top) / 2.0f}));
    }

    return false;
}

void BoxCollider::DrawDebug()
{
    //Draw collider outline
        // Get world space bounds for this collider
        float left1 = ((transform != nullptr) ? transform->position.x + rectangle.x : rectangle.x) * PIXELS_PER_UNIT;
        float right1 = left1 + ((transform != nullptr ? rectangle.width * transform->scale.x : rectangle.width)) * PIXELS_PER_UNIT;
        float bottom1 = ((transform != nullptr) ? transform->position.y + rectangle.y : rectangle.y) * PIXELS_PER_UNIT;
        float top1 = bottom1 + ((transform != nullptr ? rectangle.height * transform->scale.y : rectangle.height)) * PIXELS_PER_UNIT;

        Vector2 topL = {left1, -top1}, topR = {right1, -top1}, botL = {left1, -bottom1}, botR = {right1, -bottom1};

        DrawLineV(topL, botL, BLUE);
        DrawLineV(topR, botR, BLUE);
        DrawLineV(topL, topR, BLUE);
        DrawLineV(botL, botR, BLUE);
        
        DrawText(TextFormat("{%.1f, %.1f}", topL.x, -topL.y), topL.x, topL.y, 12, ORANGE);
        DrawText(TextFormat("{%.1f, %.1f}", topR.x, -topR.y), topR.x, topR.y, 12, ORANGE);
        DrawText(TextFormat("{%.1f, %.1f}", botL.x, -botL.y), botL.x, botL.y, 12, ORANGE);
        DrawText(TextFormat("{%.1f, %.1f}", botR.x, -botR.y), botR.x, botR.y, 12, ORANGE);
}

GridCollider::GridCollider() : grid(nullptr) {}

GridCollider::GridCollider(GridCollider &other) : grid(other.grid) {}

GridCollider::GridCollider(Grid *grid) : grid(grid) {}

GridCollider::GridCollider(Grid *grid, Vector2 origin) : grid(grid) {}

bool GridCollider::Colliding(const Vector2 point)
{
    if (grid == nullptr)
        return false;

    // Convert world position to grid coordinates
    // Assuming grid tiles are 1 unit each in world space
    int gridX = (int)(point.x - grid->GetOffset().x - 0.5f);
    int gridY = -(int)(point.y + grid->GetOffset().y + 0.5f);

    // Check if the tile at this position is solid
    return grid->GetTile(gridX, gridY);
}

bool GridCollider::Colliding(Collider *other)
{
    if (other == nullptr)
        return false;

    // Try to cast to BoxCollider
    BoxCollider *boxOther = dynamic_cast<BoxCollider*>(other);
    if (boxOther != nullptr)
    {
        // Delegate to BoxCollider's collision check to avoid code duplication
        return boxOther->Colliding(this);
    }

    return false;
}
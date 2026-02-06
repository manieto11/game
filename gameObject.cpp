#include "gameObject.h"

bool GameObject::IsEnabled()
{
    return enabled;
}

void GameObject::SetActive(bool bActive)
{
    enabled = bActive;
}

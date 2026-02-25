#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

class GameObject
{
private:
    bool enabled;

public:
    // Return true if GameObject is enabled
    bool IsEnabled();
    // Enable (or disable) GameObject
    void SetActive(bool bActive);

    // Called at the begining of the execution
    virtual void Initialize() = 0;
    // Called every frame
    virtual void Update() = 0;
    // Called every fixed delta time step
    virtual void FixedUpdate() = 0;
    // Called every frame while drawing buffer active
    virtual void Draw() = 0;
    // Called at the end of the execution
    virtual void Finalize() = 0;

    virtual ~GameObject() {}
};

#endif
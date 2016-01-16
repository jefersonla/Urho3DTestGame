/* 
 * File:   System.h
 * Author: jeferson
 *
 * Created on January 16, 2016, 9:24 AM
 */

#ifndef SYSTEM_H
#define SYSTEM_H

#include <Urho3D/Engine/Application.h>

namespace Urho3D{
    class Scene;
    class Node;
}

using namespace Urho3D;

const float TOUCH_SENSITIVITY   = 2.0f;
const int   WINDOWED_WIDTH      = 800;
const int   WINDOWED_HEIGHT     = 600;

/// System class
///    - Initialization of the Urho3D engine (in Application class)
///    - Set custom window title and icon
///    - Create Console and Debug HUD, and use F1 and F2 key to toggle them
///    - Toggle rendering options from the keys 1-8
///    - Take screenshot with key 9
///    - Handle Esc key down to hide Console or exit application

class GameSystem : public Application
{
    // Create URHO Object
    URHO3D_OBJECT(GameSystem, Application);
    
public:
    GameSystem(Context* context);
    
    /// Setup before engine initialization; Modifies engine parameters
    virtual void Setup();
    /// Setup after engine initialization; Create the console & debug HUD
    virtual void Start();
    /// Stop the game; Cleanup all objects created by the game
    virtual void Stop();
    
    // Getters and Setters
    String getGameName();
    
protected:  
    /// Camera yaw angle
    float yaw_;
    /// Camera pitch angle
    float pitch_;
    
    /// Scene
    SharedPtr<Scene> scene_;
    /// Camera scene node
    SharedPtr<Node> cameraNode_;
       
private:
    /// Game name
    String game_name;
    /// Pause flag
    bool paused_;
    /// Sound flag
    bool sound_;
    /// FullScreen flag
    bool fullscreen_;
    /// Sizes
    int width_;
    int height_;
    
    /// Set custom window Icon
    void SetWindowSizeAndIcon();
    /// Create console and debug HUD.
    void CreateConsoleAndDebugHud();
    /// Handle key down event to process key controls common to all samples.
    void HandleKeyDown(StringHash eventType, VariantMap& eventData);
};

#include "GameSystem.inl"

#endif /* SYSTEM_H */


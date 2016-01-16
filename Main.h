/* 
 * File:   Main.h
 * Author: jeferson
 *
 * Created on January 16, 2016, 9:56 AM
 */

#ifndef MAIN_H
#define MAIN_H

#include "GameSystem.h"

class MyGame : public GameSystem
{
    URHO3D_OBJECT(MyGame, GameSystem);
    
public:
    MyGame(Context* context);
    
    /// Setup after engine initialization and before main loop
    virtual void Start();
    
private:
    /// Just spam text in the screen
    void CreateText();
    /// Subscribe to application-wide logic update events
    void SubscribeToEvents();
    /// Handle the logic update event
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
};

#endif	// MAIN_H
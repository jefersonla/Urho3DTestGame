/* 
 * File:   Main.cpp
 * Author: jeferson
 *
 * Created on January 16, 2016, 9:55 AM
 */

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Core/ProcessUtils.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>

#include "GameSystem.h"
#include "Main.h"

#include <Urho3D/DebugNew.h>

MyGame::MyGame(Context* context) :
    GameSystem(context)
{
}

void MyGame::Start(){
    // Execute System class startup
    GameSystem::Start();
    
    // Create the message
    CreateText();
    
    // Finally subscribe to the update event. Note that by subscribing events at this point we have already missed some events
    // like the ScreenMode event sent by the Graphics subsystem when opening the application window. To catch those as well we
    // could subscribe in the constructor instead.
    SubscribeToEvents();
}

void MyGame::CreateText(){
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    
    // Construct the text object
    SharedPtr<Text> myText(new Text(context_));
    
    // Set text to display
    myText->SetText("Hello Wolrd from " + getGameName());

    // Set font and text color
    myText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 30);
    myText->SetColor(Color(0.0f, 1.0f, 0.0f));

    // Align Text center-screen
    myText->SetHorizontalAlignment(HA_CENTER);
    myText->SetVerticalAlignment(VA_CENTER);

    // Add Text instance to the UI root element
    GetSubsystem<UI>()->GetRoot()->AddChild(myText);
}

void MyGame::SubscribeToEvents() {
    // Subscribe HandleUpdate() function for processing update events
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(MyGame, HandleUpdate));
}

void MyGame::HandleUpdate(StringHash eventType, VariantMap& eventData){
    // Nothing to do
}

// Macro that creates main "int main(){...}"
URHO3D_DEFINE_APPLICATION_MAIN(MyGame)
/* 
 * File:   System.inl
 * Author: jeferson
 *
 * Inline definitions of the header
 * 
 * Created on January 16, 2016, 9:49 AM
 */

#ifndef SYSTEM_INL
#define SYSTEM_INL

#include <Urho3D/Engine/Application.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Engine/Console.h>
#include <Urho3D/UI/Cursor.h>
#include <Urho3D/Engine/DebugHud.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Urho3D/Core/Timer.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/Resource/XMLFile.h>

#include "GameSystem.h"

GameSystem::GameSystem(Context* context) :
    Application(context),
    yaw_(0.0f),
    pitch_(0.0f),
    paused_(false),
    width_(WINDOWED_WIDTH),
    height_(WINDOWED_HEIGHT),
    game_name("WildCardsBrazil"),
    fullscreen_(true),
    sound_(false)
{
}

String GameSystem::getGameName(){
    return game_name;
}

void GameSystem::Setup(){
    // Modify engine startup parameters
    engineParameters_["WindowTitle"]    = game_name;
    engineParameters_["LogName"]        = GetSubsystem<FileSystem>()->GetAppPreferencesDir(game_name, "logs") + game_name + ".log";
    engineParameters_["FullScreen"]     = fullscreen_;
    engineParameters_["Sound"]          = sound_;
    engineParameters_["Headless"]       = false;
    
    // Construct a search path to find the resource prefix with two entries:
    // The first entry is an empty path which will be substituted with program/bin directory -- this entry is for binary when it is still in build tree
    // The second and third entries are possible relative paths from the installed program/bin directory to the asset directory -- these entries are for binary when it is in the Urho3D SDK installation location
    if (!engineParameters_.Contains("ResourcePrefixPaths"))
        engineParameters_["ResourcePrefixPaths"] = ";../share/Resources;../share/Urho3D/Resources";
}

void GameSystem::Start(){  
    // Set custom window Icon
    SetWindowSizeAndIcon();
    
    // Create console and debug HUD
    CreateConsoleAndDebugHud();

    // Subscribe key down event
    SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(GameSystem, HandleKeyDown));
}

void GameSystem::Stop(){
    engine_->DumpResources(true);
}

void GameSystem::SetWindowSizeAndIcon(){
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Graphics* graphics = GetSubsystem<Graphics>();
    Image* icon = cache->GetResource<Image>("Textures/UrhoIcon.png");
    graphics->SetWindowIcon(icon);
    //if(!fullscreen_)
    //    graphics->SetMode(width_, height_);
}

void GameSystem::CreateConsoleAndDebugHud(){
    // Get default style
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    XMLFile* xmlFile = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");

    // Create console
    Console* console = engine_->CreateConsole();
    console->SetDefaultStyle(xmlFile);
    console->GetBackground()->SetOpacity(0.8f);

    // Create debug HUD.
    DebugHud* debugHud = engine_->CreateDebugHud();
    debugHud->SetDefaultStyle(xmlFile);
}

void GameSystem::HandleKeyDown(StringHash eventType, VariantMap& eventData){
    using namespace KeyDown;

    int key = eventData[P_KEY].GetInt();

    // Close console (if open) or exit when ESC is pressed
    if (key == KEY_ESC){
        Console* console = GetSubsystem<Console>();
        if (console->IsVisible())
            console->SetVisible(false);
        else
            engine_->Exit();
    }

    // Toggle console with F1
    else if (key == KEY_F1)
        GetSubsystem<Console>()->Toggle();

    // Toggle debug HUD with F2
    else if (key == KEY_F2){
        DebugHud* debugHud = GetSubsystem<DebugHud>();
        if (debugHud->GetMode() == 0 || debugHud->GetMode() == DEBUGHUD_SHOW_ALL_MEMORY)
            debugHud->SetMode(DEBUGHUD_SHOW_ALL);
        else
            debugHud->SetMode(DEBUGHUD_SHOW_NONE);
    }
    else if (key == KEY_F3){
        DebugHud* debugHud = GetSubsystem<DebugHud>();
        if (debugHud->GetMode() == 0 || debugHud->GetMode() == DEBUGHUD_SHOW_ALL)
            debugHud->SetMode(DEBUGHUD_SHOW_ALL_MEMORY);
        else
            debugHud->SetMode(DEBUGHUD_SHOW_NONE);
    }
    
    // Common rendering quality controls, only when UI has no focused element
    else if (!GetSubsystem<UI>()->GetFocusElement())
    {
        Renderer* renderer = GetSubsystem<Renderer>();
        
        switch(key){
            // Texture quality
            case '1': {
                int quality = renderer->GetTextureQuality();
                ++quality;
                if (quality > QUALITY_HIGH)
                    quality = QUALITY_LOW;
                renderer->SetTextureQuality(quality);
            } break;
            // Material quality
            case '2': {
                int quality = renderer->GetMaterialQuality();
                ++quality;
                if (quality > QUALITY_HIGH)
                    quality = QUALITY_LOW;
                renderer->SetMaterialQuality(quality);
            }break;
            // Specular lighting
            case '3': {
                renderer->SetSpecularLighting(!renderer->GetSpecularLighting());
            }break;
            // Shadow rendering
            case '4': {
                renderer->SetDrawShadows(!renderer->GetDrawShadows());
            }break;
            // Shadow map resolution
            case '5': {
                int shadowMapSize = renderer->GetShadowMapSize();
                shadowMapSize *= 2;
                if (shadowMapSize > 2048)
                    shadowMapSize = 512;
                renderer->SetShadowMapSize(shadowMapSize);
            }break;
            // Shadow depth and filtering quality
            case '6': {
                ShadowQuality shadowQuality = renderer->GetShadowQuality();
                shadowQuality = (ShadowQuality)(shadowQuality + 1);
                if (shadowQuality > SHADOWQUALITY_BLUR_VSM)
                    shadowQuality = SHADOWQUALITY_SIMPLE_16BIT;
                renderer->SetShadowQuality(shadowQuality);
            }break;
            // Occlusion culling
            case '7': {
                bool occlusion = renderer->GetMaxOccluderTriangles() > 0;
                occlusion = !occlusion;
                renderer->SetMaxOccluderTriangles(occlusion ? 5000 : 0);
            }break;
            // Instancing
            case '8': {
                renderer->SetDynamicInstancing(!renderer->GetDynamicInstancing());
            }break;
            // Take screenshot
            case '9':{
                Graphics* graphics = GetSubsystem<Graphics>();
                Image screenshot(context_);
                graphics->TakeScreenShot(screenshot);
                // Here we save in the Data folder with date and time appended
                screenshot.SavePNG(GetSubsystem<FileSystem>()->GetProgramDir() + "Data/Screenshot_" +
                    Time::GetTimeStamp().Replaced(':', '_').Replaced('.', '_').Replaced(' ', '_') + ".png");
            }break;
        }
    }
}

#endif /* SYSTEM_INL */
#include <stdafx.h>
#include <string>
#include <game/Global.h>
#include <game/Input.h>
#include <mod/Mod.h>
#include <HookLoaderInternal.h>
#include <mod/ModFunctions.h>
#include <mod/MathFunctions.h>
#include <util/JsonConfig.h>
#include <assets/AssetFunctions.h>
#include <assets/FObjectSpawnParameters.h>
#include "../Detours/src/detours.h"
#include "../SatisfactorySDK/SDK.hpp"
#include <memory/MemoryObject.h>
#include <memory/MemoryFunctions.h>


using namespace SML::Mod;
using namespace SML::Objects;

#define SML_VERSION "1.0.0-pr6"
#define MOD_NAME "CustomInventorySize"
#define LOG(msg) SML::Utility::infoMod(MOD_NAME, msg)
#define INFO(msg) LOG(msg)
#define WARN(msg) SML::Utility::warningMod(MOD_NAME, msg)
#define ERR(msg) SML::Utility::errorMod(MOD_NAME, msg)


// Config
json config = SML::Utility::JsonConfig::load(MOD_NAME, {
	{"Version", "0.1.1"}
});

// Global variables required by the mod
AFGPlayerController* player;


// A custom event handler for when ExampleMod's post setup is complete.
// Other mods can also make a handler for ExampleMod_PostSetupComplete if they want to do something when the event is broadcast.
void postSetupComplete() {
	LOG("CustomInventorySize's post setup has been completed!");
}

// information about the mod
Mod::Info modInfo {
	// Target sml version
	SML_VERSION,

	// Name
	MOD_NAME,

	// Version
	"0.1.2",

	// Description
	"A basic mod to change inventory size.",

	// Authors
	"Stinosko",

	// Dependencies
	// Place mod names that you want to ensure is loaded in this vector. If you place an asterisk (*) before the mod name, it will be loaded as an optional dependency instead.
	{}
};

// The mod's class, put all functions inside here
class CustomInventorySize : public Mod {



public:
	// Constructor for SML usage. Do not put anything in here, use setup() instead.
	CustomInventorySize() : Mod(modInfo) {
	}

	// The setup function is the heart of the mod, where you hook your functions and register your commands and API functions. Do not rename!
	void setup() override {
		
		using namespace std::placeholders;

		SDK::InitSDK(); 

		::subscribe<&PlayerInput::InputKey>([this](Functions::ModReturns* modReturns, PlayerInput* playerInput, FKey key, InputEvent event, float amount, bool gamePad) {

			if (GetAsyncKeyState('K')) {
				SDK::AFGCharacterPlayer aCharachter = *Functions::getPlayerCharacter();
				SDK::UFGInventoryComponent* inventory = aCharachter.GetInventory();


				int size = inventory->GetSizeLinear();
				int newsize = size+1;

				std::string message = "Changing size from " + std::to_string(size) + " => " + std::to_string(newsize);
				LOG(message);

				inventory->Resize(newsize);
			}

			if (GetAsyncKeyState('L')) {
				SDK::AFGCharacterPlayer aCharachter = *Functions::getPlayerCharacter();
				SDK::UFGInventoryComponent* inventory = aCharachter.GetInventory();

				int size = inventory->GetSizeLinear();
				int newsize = size-1;
				//Preventing a error with a inventory of 0
				if (newsize == 0) {
					newsize = 1;
				};

				std::string message = "Changing size from " + std::to_string(size) + " => " + std::to_string(newsize);
				LOG(message);

				inventory->Resize(newsize);

			}
			return false;
		});


		
		
		Functions::registerEvent("CustomInventorySize_PostSetupComplete", postSetupComplete);

		LOG("Finished CustomInventorySize setup!");
	}

	//The postSetup function is where you do things based on other mods' setup functions
	void postSetup() override {
		//Broadcast the event for ExampleMod and other mods that do something for ExampleMod_PostSetupComplete.
		Functions::broadcastEvent("CustomInventorySize_PostSetupComplete");

	}

	~CustomInventorySize() {
		// Cleanup
		LOG("CustomInventorySize's finished cleanup!");
	}
};

// Required function to create the mod, do not rename!
MOD_API Mod* ModCreate() {
	return new CustomInventorySize();
}
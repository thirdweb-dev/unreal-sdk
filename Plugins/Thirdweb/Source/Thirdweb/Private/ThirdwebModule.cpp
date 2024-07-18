#include "ThirdwebModule.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

IMPLEMENT_MODULE(FThirdwebModule, Thirdweb)

#define LOCTEXT_NAMESPACE "FThirdwebModule"

void FThirdwebModule::StartupModule()
{
    UE_LOG(LogTemp, Warning, TEXT("Thirdweb plugin started!"));
}

void FThirdwebModule::ShutdownModule()
{
    UE_LOG(LogTemp, Warning, TEXT("Thirdweb plugin shut down."));
}

#undef LOCTEXT_NAMESPACE

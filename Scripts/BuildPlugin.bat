@echo off

ue4 package ^
    -NoHostPlatform ^
    -TargetPlatforms=Win64+Android+Linux+LinuxArm64 ^
    -Package=%UserProfile%\Downloads\ThirdwebSDK_Dist
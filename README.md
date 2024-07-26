# Thirdweb Unreak SDK Plugin

# Unreal Plugin

A plugin for Unreal Engine that enables developers to create thirdweb Private Key Wallets, In-App Wallets and Smart Wallets for their games and applications. The plugin provides a simple API integrating blueprints to interact with wallets, login with email or socials as well as create Smart Wallet session keys. 

With this plugin, you can keep the onboarding of your users in-client and grant a session key to your thirdweb Engine powered backend to interact with the blockchain, no signature in sight.

The plugin is still a work in progress and will be updated with more features and improvements.

## Installation

The Unreal SDK is distributed as an [Unreal Marketplace Plugin](https://www.unrealengine.com/marketplace/).

The plugin's core logic is built from Rust, making it lightning-fast and minimizing the clutter in your Unreal project.

It currently is built to support Win64 platforms and was tested in UE 5.4.


## Get Started

`BP_Thirdweb_OAuth` showcases social login with socials, defaulting to Google for the example. You may test it in `Level_Thirdweb_OAuth`.

`Level_Thirdweb` that will trigger Email Login, make sure `BP_Thirdweb` has the Email set.

`BP_Thirdweb` is the main blueprint that will handle wallet initialization, you may also set your thirdweb API key settings in its Class Defaults.

`BP_Thirdweb` showcases general wallet creation, interaction and email login as well as session resuming.

`BP_Thirdweb_VerificationWidget` has a graph that showcases the flow to verify an OTP and subsequently create a wallet. It also updates some basic UI.

![image](https://github.com/user-attachments/assets/a7ca6a17-b221-41a5-ad24-d2044e9b4e8c)

![image](https://github.com/user-attachments/assets/cc639ade-3398-427b-9c41-474645c1aa47)

![image](https://github.com/user-attachments/assets/514790a4-64f3-4b0b-8a83-d8a8ace5ab82)

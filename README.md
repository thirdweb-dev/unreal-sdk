![unreal-hero](https://github.com/user-attachments/assets/a3d3a83f-fbbe-4ad1-9c68-b115f8992087)

[![Static Badge](https://img.shields.io/badge/Unreal%20SDK-Documentation-red?style=for-the-badge)][documentation-url]
[![Static Badge](https://img.shields.io/badge/Unreal%20SDK-Marketplace-purple?style=for-the-badge)][marketplace-url]

# Thirdweb Unreal Engine SDK Plugin

A Code Plugin for Unreal Engine that enables developers to create thirdweb Private Key Wallets, In-App Wallets and
Smart Wallets for their games and applications. The plugin provides a simple API and blueprints to interact with
wallets, login with email or socials as well as create Smart Wallet session keys.

With this plugin, you can keep the onboarding of your users in-client and grant a session key to your
[thirdweb Engine](https://portal.thirdweb.com/engine) powered backend to interact with the blockchain,
no signature in sight.

## Supported Unreal Engine Versions

Thirdweb adheres to Unreal Engine's Marketplace Guidelines in supporting the 3 most recent versions[^1].
We aim to release an updated plugin version as expediently as possible once a minor version change has released.
If you or your company require compatibility with an older version, please reach out to [support][support-url].

## Installation

In addition to the source code provided, the plugin is available on the [Unreal Marketplace][marketplace-url].

The plugin's core logic is built from Rust, making it lightning-fast and minimizing the clutter in your Unreal project.

## Packaging

Windows, Linux, and Android all package natively and can be used in blueprint-only projects

### Mac

The plugin currently makes use of the [Web Browser][web-browser-doc] runtime module which bundles [CEF3][cef-forum].
Changes to the engine are currently required for modern xcode targets as Unreal Engine does not properly link
the framework during packaging. This requires either an engine patch or moving the framework post-build and
then codesigning your build again.

In the [CEF3.build.cs][cef3-build-cs] you need to bypass the `if` check for modern xcode. This can be accomplished by
adding `|| true` to the end of the if check on line 102.

Additionally, you need to add `bCompileCEF3 = true;` to your build target

## Supported Platforms & Architectures

| Platform | x64 | arm64 |
|----------|:---:|:-----:|
| Windows  |  ✅  |   ➖   |
| Linux    |  ✅  |   ❌   |
| Mac      |  ✅  |   ✅   |
| IOS      |  ➖  |   ✅   |
| TVOS     |  ➖  |   ✅   |
| VisionOS |  ➖  |   ✅   |
| Android  |  ➖  |   ✅   |

Legend: ✅ Supported | ❌ Unsupported | ➖ Not applicable

## Documentation

Documentation is available at https://portal.thirdweb.com/unreal

[^1]: Unreal Engine's Marketplace Guidelines - Section 3.1.b https://www.unrealengine.com/en-US/marketplace-guidelines#31b

[support-url]: https://thirdweb.com/support
[documentation-url]: https://portal.thirdweb.com/unreal
[marketplace-url]: https://www.unrealengine.com/marketplace/en-US/product/f21200c2610146f3888172994448e50d
[web-browser-doc]: https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/WebBrowser
[cef-forum]: https://www.magpcss.org/ceforum/index.php
[cef3-build-cs]: https://github.com/EpicGames/UnrealEngine/blob/release/Engine/Source/ThirdParty/CEF3/CEF3.build.cs#L102

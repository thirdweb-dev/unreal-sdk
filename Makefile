PYTHON_EXE ?= C:\Program Files\Python311\python.exe
BUILDS_DIR ?= E:\UnrealEngine\Thirdweb\PluginBuilds
BUILD_HOST_PLATFORM ?= false
UE_VERSION ?= 5.4
PLUGIN_VERSION ?= 1.3.0

# Determine the current OS in a platform-independent manner
ifeq ($(OS),Windows_NT)
  SHELL := cmd.exe
  TARGET_PLATFORMS ?= Win64+Android+Linux+LinuxArm64
  PATH_SEP := \\
  UE4CLI_CONFIG_DIR_BASE := %APPDATA%\ue4cli
else
  UNAME_S := $(shell uname -s)
  UE4CLI_CONFIG_DIR_BASE := ~/.config/ue4cli
  PATH_SEP := /
  ifeq ($(UNAME_S),Linux)
    TARGET_PLATFORMS ?= Android+Linux+LinuxArm64
  else ifeq ($(UNAME_S),darwin)
    TARGET_PLATFORMS ?= IOS+Mac+TVOS+VisionOS
  endif
endif

ifeq ($(BUILD_HOST_PLATFORM),false)
  HOST_PLATFORM_FLAG := -NoHostPlatform
 else
  HOST_PLATFORM_FLAG := 
endif

UE4_EXE := $(PYTHON_EXE) -m ue4cli
UE4CLI_CONFIG_DIR := $(UE4CLI_CONFIG_DIR_BASE)$(PATH_SEP)$(UE_VERSION)

.PHONY: build
build:
	$(UE4_EXE) setroot $(UE_ENGINE_DIR)
	$(UE4_EXE) package $(HOST_PLATFORM_FLAG) -TargetPlatforms=$(TARGET_PLATFORMS) -Package=$(BUILDS_DIR)\\$(UE_VERSION)

.PHONY: compress
compress:
	cd $(BUILDS_DIR)
	pwsh -Command Compress-Archive -LiteralPath "$(BUILDS_DIR)\\$(UE_VERSION)\\Source", "$(BUILDS_DIR)\\$(UE_VERSION)\\Content", "$(BUILDS_DIR)\\$(UE_VERSION)\\Thirdweb.uplugin" -DestinationPath "$(BUILDS_DIR)\\ThirdwebSDK-$(PLUGIN_VERSION)-$(UE_VERSION).zip" -CompressionLevel Optimal
// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "IOSRuntimeSettings.generated.h"


UENUM()
enum class EPowerUsageFrameRateLock : uint8
{
    /** Frame rate is not limited */
    PUFRL_None = 0 UMETA(DisplayName="None"),
        
    /** Frame rate is limited to a maximum of 20 frames per second */
    PUFRL_20 = 20 UMETA(DisplayName="20 FPS"),
    
    /** Frame rate is limited to a maximum of 30 frames per second */
    PUFRL_30 = 30 UMETA(DisplayName="30 FPS"),
    
    /** Frame rate is limited to a maximum of 60 frames per second */
    PUFRL_60 = 60 UMETA(DisplayName="60 FPS"),
};


/**
 * Implements the settings for the iOS target platform.
 */
UCLASS(config=Engine, defaultconfig)
class IOSRUNTIMESETTINGS_API UIOSRuntimeSettings : public UObject
{
public:
	GENERATED_UCLASS_BODY()

	// Should Game Center support (iOS Online Subsystem) be enabled?
	UPROPERTY(GlobalConfig, EditAnywhere, Category = Online)
	uint32 bEnableGameCenterSupport : 1;
	
	// Whether or not to add support for Metal API (requires IOS8 and A7 processors)
	UPROPERTY(GlobalConfig, EditAnywhere, Category = Rendering)
	bool bSupportsMetal;

	// Whether or not to add support for OpenGL ES2 (if this is false, then your game should specify minimum IOS8 version and use "metal" instead of "opengles-2" in UIRequiredDeviceCapabilities)
	UPROPERTY(GlobalConfig, EditAnywhere, Category = Rendering)
	bool bSupportsOpenGLES2;

	// Does the application support portrait orientation?
	UPROPERTY(GlobalConfig, EditAnywhere, Category = DeviceOrientations)
	uint32 bSupportsPortraitOrientation : 1;

	// Does the application support upside down orientation?
	UPROPERTY(GlobalConfig, EditAnywhere, Category = DeviceOrientations)
	uint32 bSupportsUpsideDownOrientation : 1;

	// Does the application support landscape left orientation?
	UPROPERTY(GlobalConfig, EditAnywhere, Category = DeviceOrientations)
	uint32 bSupportsLandscapeLeftOrientation : 1;

	// Does the application support landscape right orientation?
	UPROPERTY(GlobalConfig, EditAnywhere, Category = DeviceOrientations)
	uint32 bSupportsLandscapeRightOrientation : 1;

	// Bundle Display Name
	UPROPERTY(GlobalConfig, EditAnywhere, Category = BundleInfo)
	FString BundleDisplayName;

	// Bundle Name
	UPROPERTY(GlobalConfig, EditAnywhere, Category = BundleInfo)
	FString BundleName;

	// Bundle identifier
	UPROPERTY(GlobalConfig, EditAnywhere, Category = BundleInfo)
	FString BundleIdentifier;

	// version info
	UPROPERTY(GlobalConfig, EditAnywhere, Category = BundleInfo)
	FString VersionInfo;
    
    /** Set the maximum frame rate to save on power consumption */
    UPROPERTY(GlobalConfig, EditAnywhere, Category = PowerUsage)
    TEnumAsByte<EPowerUsageFrameRateLock> FrameRateLock;

#if WITH_EDITOR
	// UObject interface
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	// End of UObject interface
#endif
};

// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnholyCharacter.generated.h"


UENUM(BlueprintType)		
enum class EJumpType : uint8
{
	EJT_single 	UMETA(DisplayName = "jump"),
	EJT_double 	UMETA(DisplayName = "doubleJump"),
	EJT_wall	UMETA(DisplayName = "wallJump"),
	EJT_clean	UMETA(DisplayName = "clean")
};

UENUM(BlueprintType)		// offense skill (q)
enum class ESkillOneType : uint8
{
	ES1_none		UMETA(DisplayName = "none"),
	ES1_knockback 	UMETA(DisplayName = "Knockback"),
	ES2_nade		UMETA(DisplayName = "grenade"),
	ES2_freeze		UMETA(DisplayName = "freeze enemies"),
	ES2_bang		UMETA(DisplayName = "flash bang"),
	ES2_wave		UMETA(DisplayName = "wave attack"),
	ES2_rockets		UMETA(DisplayName = "lock-on rockets"),
	ES2_mine		UMETA(DisplayName = "mines")
	
};

UENUM(BlueprintType)		// defense skill	(e)
enum class ESkillTwoType : uint8
{
	ES2_none		UMETA(DisplayName = "none"),
	ES2_deflect		UMETA(DisplayName= "deflect damage"),
	ES2_absorb		UMETA(DisplayName = "absorb bomb"),
	ES2_invu		UMETA(DisplayName = "invulnarability shield")
};

UENUM(BlueprintType)		// support skill / gadget (c)
enum class ESkillThreeType : uint8
{
	ES3_none		UMETA(DisplayName = "none"),
	ES3_teleport	UMETA(DisplayName = "teleport"),
	ES3_gScale		UMETA(DisplayName = "gravity scale"),
	ES3_speed		UMETA(DisplayName = "run speed")
};

UENUM(BlueprintType)		// special shot (trigger, right click)
enum class ESpecialShotType : uint8
{
	ESS_none		UMETA(DisplayName = "none"),
	ESS_laser		UMETA(DisplayName = "laser"),
	ESS_explode		UMETA(DisplayName = "explode shot"),
	ESS_chain		UMETA(DisplayName = "chain shot"),
	ESS_auto		UMETA(DisplayName = "auto fire"),
	ESS_heavy		UMETA(DisplayName = "heavy shot")
};

UCLASS(config=Game)
class AUnholyCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

protected:

	UFUNCTION()
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetCharacterRotationToMouse();

	UFUNCTION()
	void GetAim();

	UFUNCTION()
	void GetMousePos();

	UFUNCTION()
	void AdjustMovementSpeed();

	UFUNCTION()
	void Dash();

	UFUNCTION()
	void SpaceDown();

	UFUNCTION()
	void SpaceUp();

	UFUNCTION()
	void FireWeapons();

	UFUNCTION()
	void FireCharged();

	UFUNCTION()
	void FireChargeActivated();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* characterCenter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* characterAimPoint;

	// e skills
	UFUNCTION(BlueprintCallable)
	void SkillOne();

	UFUNCTION(BlueprintCallable)
	void SkillKnockBack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skils")
	UChildActorComponent* knockBackSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skils")
	bool bKnockBackUsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skils")
	TArray<AActor*> knockBackHitActors;

	// q skills
	UFUNCTION(BlueprintCallable)
	void SkillTwo();

	// c skills
	UFUNCTION(BlueprintCallable)
	void SkillThree();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float chargeFireTimerMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float chargeFireTimerValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bCanChargeFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bIsChargeFiring;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float chargeActiveTimerMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float chargeActiveTimerValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bIsCharging;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	//USceneComponent* muzzleLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bSpecialShotReady;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	ESpecialShotType specialShotType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* characterMesh;

	/** Called for side to side input */
	void MoveRight(float Val);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	float screenSizeX;
	float screenSizeY;
	float mouseLocationX;
	float mouseLocationY;

	float relativeMouseLocX;
	float relativeMouseLocY;
	float weaponOffsetY;

	float pitchX;
	float pitchY;

public:
	AUnholyCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	EJumpType jumpType;

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BlueprintCallable)
	FVector GetCharacterCenter();

	// returns movement speed on axis 1:x, 2;y, 3:z
	UFUNCTION(BlueprintCallable, Category = Movement)
	float GetSpeed(int axis);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool bShowDebugWindow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool bShowPopUpInfos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool bShowPauseMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bAllowMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	bool bIsJumping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bCanClimb;

	// movement vars
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float inputValueX;
	
	float maxMovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float adjustedMovementSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float backwardsSpeedModifier;

	float runningSpeedModifier;
	float charVelocityX;
	float charVelocityY;
	float charVelocityZ;

	UFUNCTION(BlueprintCallable)
	void DealDamage(float dmg);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	int jumpCounterMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	int jumpCounter;

	// if player is in range of usable object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Usable")
	bool bUsableInRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Usable")
	bool bUsableActivated;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Usable")
	FString ActivateObjectName;

	// direction the character is facing
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aim")
	FString face;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aim")
	float pitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jump")
	bool bIsOnGround;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jump")
	bool bIsFalling;

	UFUNCTION()
	void Use();

	UFUNCTION()
	void DownPressed();

	UFUNCTION()
	void DownReleased();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bDownPressed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	float dashTimerActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	float dashTimerMax;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	float dashCDActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	float dashCDMax;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	float dashingTimeActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	float dashingTimeMax;

	// is actually performing a dash
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	bool isDashing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	bool dashed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	bool canDash;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	float dashSpeedMultiplicator;

	// speed for dash
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	float launchSpeed;

	// speedvector for dash
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash")
	FVector launchVector;

	// is holding to a wall atm
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallJump")
	bool bIsAtWallJump;

	// is touching a walljump panel during jump
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallJump")
	bool bCanWallJump;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WallJump")
	bool space;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WallJump")
	float gScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallJump")
	FVector jumpDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UserInterface")
	bool bShowOnCharUI;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	int facingMethod;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D screenSize;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WallJump")
	bool bWasWallHugging;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float doubleJumpLaunchPowerY;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float doubleJumpLaunchPowerZ;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jump")
	float jumpPowerValue;

	// multiplies time needed to reach max power
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Jump")
	float jumpPowerValueTimeMultiplicator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float jumpPowerMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	float jumpPowerBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vars")
	bool bIsAlive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vars")
	bool bIsDying;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vars")
	float healthMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "vars")
	float healthValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "vars")
	float healthDelayed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stomp")
	bool bDoStomp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stomp")
	bool stompRdy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stomp")
	bool bIsStomping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stomp")
	float stompTimerValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stomp")
	float stompTimerMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stomp")
	bool stompCoolDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stomp")
	float stompSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stomp")
	float stompCoolDownMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "stomp")
	float stompCoolDownValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "shield")
	bool bShieldActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "shield")
	int shieldChargesMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "shield")
	int shieldChargesValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "shield")
	float shieldRechargeTimerMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "shield")
	float shieldRechargeTimerValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "shield")
	float timeSinceLastShieldHitMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "shield")
	float timeSinceLastShieldHitValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "shield")
	bool bPlayHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "menues")
	bool bSkillMenuDisplayed;

	//skills

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skillpoints")
	int skillPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skillpoints")
	bool skills_CanWallJump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skillpoints")
	bool skills_CanDash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skillpoints")
	bool skills_CanStomp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skillpoints")
	bool skills_AdvancedJump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skillpoints")
	bool skills_CanChargeFire;

	// stats

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	ESkillOneType skillOneType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	ESkillTwoType skillTwoType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	ESkillThreeType skillThreeType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
		float baseKickPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skills")
	int skillLevel_health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skills")
	int skillLevel_dashCD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skills")
	int skillLevel_wallhugDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skills")
	int skillLevel_StompCD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skills")
	int skillLevel_chargeUpShotTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skills")
	int skillLevel_shieldRechargeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skills")
	int skillLevel_shieldRechargeDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skills")
	int skillLevel_gadgetCoolDown;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skills")
	int skillLevel_GadgetCapacity;

	// skill values

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skillValues")
	float skillValue_health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skillValues")
	float skillValue_dashCD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skillValues")
	float skill_ValuewallhugDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skillValues")
	float skillValue_StompCD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skillValues")
	float skillValue_chargeUpShotTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skillValues")
	float skillValue_shieldRechargeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skillValues")
	float skillValue_shieldRechargeDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skillValues")
	float skillValue_gadgetCoolDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "skillValues")
	float skillValue_GadgetCapacity;

	// skill values after level boni

	UFUNCTION()
	void IncreaseSkillsByLevel();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "actualSkillValues")
	float healthIncreased;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "actualSkillValues")
	float dashCDDecreased ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "actualSkillValues")
	float stompCoolDownDecreased;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "actualSkillValues")
	float chargeActiveTimerMaxDecreased;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "actualSkillValues")
	float shieldRechargeTimerMaxDecreased;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "actualSkillValues")
	float timeSinceLastShieldHitMaxDecreased;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "actualSkillValues")
	float chargeFireTimerMaxDecreased;

	// weapons stats

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "weaponStats")
	float bulletDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "weaponStats")
	float bulletRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "weaponStats")
	float bulletSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "weaponStats")
	float bulletPenetration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "weaponStats")
	float chargeSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "weaponStats")
	float chargeReadyTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "weaponStats")
	float critMultiplier;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bGameIsLoaded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "vars")
	UArrowComponent* aimPointRef;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "vars")
	TArray<AActor*> hitActors;
};

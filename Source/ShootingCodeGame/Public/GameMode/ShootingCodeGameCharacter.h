// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ShootingCodeGameCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AShootingCodeGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** TriggerAction Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TriggerAction;

	/** TakeWeaponAction Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TakeWeaponAction;

	/** ReloadAction Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

	/** DropWeaponAction Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DropWeaponAction;

public:
	AShootingCodeGameCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			
	/** Called for Trigger input */
	void Trigger(const FInputActionValue& Value);

	/** Called for TakeWeapon input */
	void TakeWeapon(const FInputActionValue& Value);

	/** Called for Reload input */
	void Reload(const FInputActionValue& Value);

	/** Called for DropWeapon input */
	void DropWeapon(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	// �̺�Ʈ ƽ �߰�
	virtual void Tick(float DeltaSeconds) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	// ��Ʈ��ũ �ڵ� ����
	UFUNCTION(Server, Reliable)
	void RequestTakeWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void ResponseTakeWeapon(AActor* PickActor);

	UFUNCTION(Client, Reliable)
	void ResponseTakeWeaponClient();

	UFUNCTION(NetMulticast, Reliable)
	void RequestDropWeapon();

	UFUNCTION(NetMulticast, Reliable)
	void ResponseDropWeapon(AActor* PickActor);

	UFUNCTION(Client, Reliable)
	void ResponseDropWeaponClient();

	UFUNCTION(Server, Reliable)
	void RequestTrigger();

	UFUNCTION(NetMulticast, Reliable)
	void ResponseTrigger();

	UFUNCTION(Server, Reliable)
	void RequestReload();

	UFUNCTION(NetMulticast, Reliable)
	void ResponseReload();

public:
	// Weapon �� ��ӹް� �ִ� Ŭ������ ���� �� �ֱ⿡ ������ Ŭ������ �մϴ�.
	UFUNCTION(BlueprintCallable)
	void EquipTestWeapon(TSubclassOf<class AWeapon> WeaponClass);

	UFUNCTION()
	void TestWeaponSetOwner();

	// ���� �ݱ⸦ ���� ���� ����
	AActor* FindNearestWeapon();

public:
	// �������� Ŭ���̾�Ʈ�� �� ���� �����ݴϴ�.
	UPROPERTY(Replicated, BlueprintReadWrite)
	FRotator ControlRot;

	// ĳ���Ϳ��� ������ ���� ���� ����
	UPROPERTY(BlueprintReadWrite)
	AActor* m_EquipWeapon;

	FTimerHandle th_BindSetOwner;
};


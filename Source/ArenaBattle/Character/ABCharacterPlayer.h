// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "InputActionValue.h"
#include "ABCharacterPlayer.generated.h"



class UInputMappingContext;
class UInputAction;
/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABCharacterPlayer : public AABCharacterBase
{
	GENERATED_BODY()


public:
	AABCharacterPlayer();

protected:
	virtual void BeginPlay() override;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	//이동함수.
	void Move(const FInputActionValue& Value);

	//회전함수.
	void Look(const FInputActionValue& Value);


protected:
	//컴포넌트 구성.
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class UCameraComponent> Camera;

	//입력관련.
protected:
	UPROPERTY(VisibleAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = Input, BlueprintReadWrite, meta= (AllowPrivateAccess="true"))
	TObjectPtr<UInputAction> MoveAction;


	UPROPERTY(VisibleAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<UInputAction> LookAction;


	UPROPERTY(VisibleAnywhere, Category = Input, BlueprintReadOnly)
	TObjectPtr<UInputAction> JumpAction;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterPlayer.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AABCharacterPlayer::AABCharacterPlayer()
{
	//기본 설정.
	bUseControllerRotationPitch = false; //y
	bUseControllerRotationYaw = false;  //z 
	bUseControllerRotationRoll = false; //x


	//무브먼트 설정
	//캐릭터가 이동하는 방향에 맞게 회전을 해주는 옵션.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 800.0f;


	//메시 컴포넌트 설정
	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, -88.0f),
		FRotator(0.0f, -90.0f, 0.0f)
	);

	//메시 에셋 지정(검색 필요함).
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(
		TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple")
	);


	if (CharacterMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
	}


	//애님 블루프린트 클래슺 정보 지정.
	static ConstructorHelpers::FClassFinder<UAnimInstance> CharacterAnim(
		TEXT("/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed.ABP_Unarmed_C")
	);

	if (CharacterAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(CharacterAnim.Class);
	}



	//컴포넌트 생성
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->bUsePawnControlRotation = true;


	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);



	//입력 관련 에셋 로드 설정.
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>
		DefaultMappingContextRef(TEXT("/Game/ArenaBattle/Input/IMC_Default.IMC_Default"));

	if (DefaultMappingContextRef.Succeeded())
	{
		DefaultMappingContext = DefaultMappingContextRef.Object;
	}



	static ConstructorHelpers::FObjectFinder<UInputAction>
		MoveActionRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_Move.IA_Move"));
	if (MoveActionRef.Succeeded())
	{
		MoveAction = MoveActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>
		LookActionRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_Look.IA_Look"));
	if (LookActionRef.Succeeded())
	{
		LookAction = LookActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>
		JumpActionRef(TEXT("/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump"));
	if (JumpActionRef.Succeeded())
	{
		JumpAction = JumpActionRef.Object;
	}


	
	
}

void AABCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();


	//사용할 입력 매핑 컨텍스트 지정.
	APlayerController* PlayerController =  Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* InputSystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			PlayerController->GetLocalPlayer()
		);

		if (InputSystem)
		{
			InputSystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}


}

void AABCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	//바인딩. - 향상된 입력 시스템 컴포넌트를 활용해서 설정.
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(
			MoveAction,
			ETriggerEvent::Triggered,
			this,
			&AABCharacterPlayer::Move
		);


		EnhancedInputComponent->BindAction(
			LookAction,
			ETriggerEvent::Triggered,
			this,
			&AABCharacterPlayer::Look
		);


		EnhancedInputComponent->BindAction(
			JumpAction,
			ETriggerEvent::Started,
			this,
			&ACharacter::Jump
		);

		EnhancedInputComponent->BindAction(
			JumpAction,
			ETriggerEvent::Completed,
			this,
			&ACharacter::StopJumping
		);
	}

}

void AABCharacterPlayer::Move(const FInputActionValue& Value)
{
	FVector2D Movement = Value.Get<FVector2D>();

	//이동할 방향 만들기.
	// 카메라가 바라보는 방향(컨트롤러의 방향)을 기준으로 방향 만들기.
	FRotator Rotation = GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);


	//앞방향.
	FVector ForwardVector
		= FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);



	//오른쪽 방향.
	FVector RightVector
		= FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);


	//무브먼트 컴포넌트 입력 전달하기
	AddMovementInput(ForwardVector, Movement.Y);
	AddMovementInput(RightVector, Movement.X);

}

void AABCharacterPlayer::Look(const FInputActionValue& Value)
{



}

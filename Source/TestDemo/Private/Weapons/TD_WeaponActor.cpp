#include "Weapons/TD_WeaponActor.h"

#include "Component/DidItHit/DidItHitActorComponent.h"
#include "Components/ArrowComponent.h"

ATD_WeaponActor::ATD_WeaponActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Root = CreateDefaultSubobject<UArrowComponent>(TEXT("Root"));
	RootComponent = Root;

	DidItHitActorComponent = CreateDefaultSubobject<UDidItHitActorComponent>(TEXT("DidItHitActorComponent"));

	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
	WeaponSkeletalMesh->SetupAttachment(RootComponent);

	WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponStaticMesh"));
	WeaponStaticMesh->SetupAttachment(RootComponent);
}

void ATD_WeaponActor::BeginPlay()
{
	Super::BeginPlay();

	SetupVariables(WeaponStaticMesh, GetOwner());
	DidItHitActorComponent->MyActorsToIgnore.Add(GetOwner());
	DidItHitActorComponent->MyActorsToIgnore.Add(this);
	DidItHitActorComponent->OnItemAdded.AddDynamic(this, &ThisClass::OnAddHitItem);
}

void ATD_WeaponActor::ToggleTraceCheck(bool bTrace)
{
	if (DidItHitActorComponent)
	{
		DidItHitActorComponent->ToggleTraceCheck(bTrace);
	}
}

void ATD_WeaponActor::SetupVariables(UPrimitiveComponent* MyPrimitiveTarget, AActor* MyActorTarget)
{
	if (DidItHitActorComponent)
	{
		DidItHitActorComponent->SetupVariables(MyPrimitiveTarget, MyActorTarget);
	}
}

UDidItHitActorComponent* ATD_WeaponActor::GetDidItHitActorComponent() const
{
	return DidItHitActorComponent;
}

void ATD_WeaponActor::OnAddHitItem_Implementation(FHitResult NewHitResult)
{
	if (IsValid(GetOwner()))
	{
		
	}
}



#pragma once

#include "CoreMinimal.h"
#include "EnemyController.h"
#include "ViolinZombieController.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLE_API AViolinZombieController : public AEnemyController
{
	GENERATED_BODY()
	
public:
	// Violin Zombie ÂüÁ¶
	void FindPlayer();

	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;

private:
	void SetOriginLocation();

	void SetSoundLocation(bool SoundCatch);

	class AViolinZombie* OwnerVZ;
};

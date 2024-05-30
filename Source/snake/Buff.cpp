#include "Buff.h"

void UBuff::HandleBuff(AActor* ActorToApply, BuffType Buff, float LifeDuration) {
	ASnakeBase* Snake = Cast<ASnakeBase>(ActorToApply);
	TimeDuration = LifeDuration;

	if (Snake) {
		switch (Buff) {
		case BuffType::Haste:
			SpeedUp(Snake);
			break;
		case BuffType::SlowDown:
			SlowingDown(Snake);
			break;
		case BuffType::Afk:
			Afk(Snake);
			break;
		}
	}
}

void UBuff::RemoveBuff(ASnakeBase* Snake, BuffType Buff) {
	switch (Buff) {
	case BuffType::Haste:
		SlowingDown(Snake);
		break;
	case BuffType::SlowDown:
		SpeedUp(Snake);
		break;
	case BuffType::Afk:
		UnAfk(Snake);
		break;
	}
}

float UBuff::GetTimerDuration() {
	return TimeDuration;
}

void UBuff::SpeedUp(ASnakeBase* Snake) {
	Snake->MovementSpeed *= 0.5;
	Snake->UpdateMovementSpeed(Snake->MovementSpeed);
}

void UBuff::SlowingDown(ASnakeBase* Snake) {
	Snake->MovementSpeed *= 2.0;
	Snake->UpdateMovementSpeed(Snake->MovementSpeed);
}

void UBuff::UnAfk(ASnakeBase* Snake) {
	Snake->MovementSpeed = LastMovementSpeed;
	Snake->UpdateMovementSpeed(Snake->MovementSpeed);
}

void UBuff::Afk(ASnakeBase* Snake) {
	LastMovementSpeed = Snake->MovementSpeed;
	Snake->MovementSpeed = TimeDuration;
	Snake->UpdateMovementSpeed(Snake->MovementSpeed);
}


#include "GP_Pawn_Base.h"


AGP_Pawn_Base::AGP_Pawn_Base() {
	PrimaryActorTick.bCanEverTick = true;
}

void AGP_Pawn_Base::HandleUpInput(const float Value) {}
void AGP_Pawn_Base::HandleRightInput(const float Value) {}
void AGP_Pawn_Base::HandleSpaceInput() {}
void AGP_Pawn_Base::HandleZoomInput(float Value) {}
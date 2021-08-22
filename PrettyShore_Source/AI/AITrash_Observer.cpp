// I like sand


#include "AITrash_Observer.h"

#include "Models/AI_Trash.h"

void UAITrash_Observer::AddTrash(AAI_Trash* Trash) {
	TrashObjects.Add(Trash);
	TotalDecoration += Trash->DecorationValue;
}

void UAITrash_Observer::RemoveTrash(AAI_Trash* Trash) {
	TrashObjects.Remove(Trash);
	TotalDecoration -= Trash->DecorationValue;
}

UBeachWork* UAITrash_Observer::GetWork() {
	if (TrashObjects.Num() > 0) {
		for (int i = 0; i< TrashObjects.Num(); i++) {
			if (TrashObjects[i]->BeachWork->WorkOwner == nullptr) {
				return TrashObjects[i]->BeachWork;
			}
		}
	}
	return nullptr;
}

bool UAITrash_Observer::HasWork() const {
	return TrashObjects.Num() > 0;
}

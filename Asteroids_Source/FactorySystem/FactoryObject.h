#pragma once
#include <FactorySystem/Predef.h>
#include <FactorySystem/ObjectDefinitions.h>

class FactoryObject {
public:
	void SetData(ObjectData* data) {
		predefData = data->def;
		OnSetData(data);
	}

protected:
	virtual void OnSetData(ObjectData* data) { }

	Predef predefData;
};
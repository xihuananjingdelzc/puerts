// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectRetainer.h"
#include "PropertyMacros.h"


USTRUCT(noexport)
struct FPropertyMetaRoot
{
};

namespace puerts
{

enum BuiltinType{
    TBool = 0,
    TByte = 1,
    TInt = 2,
    TFloat = 3,
    TInt64 = 4,
    TString = 5,
    TText = 6,
    TName = 7,
    MaxBuiltinType
};

class FContainerMeta
{
public:
    FContainerMeta();
	~FContainerMeta();

    PropertyMacro * GetBuiltinProperty(BuiltinType type);

    PropertyMacro * GetObjectProperty(UStruct *Struct);

    void NotifyUStructDeleted(const UStruct *Struct);

private:
    puerts::FObjectRetainer ObjectRetainer;

    UScriptStruct *PropertyMetaRoot;

    PropertyMacro *BuiltinProperty[MaxBuiltinType];

    TMap<UStruct*, PropertyMacro*> ObjectPropertyMap;
};

}
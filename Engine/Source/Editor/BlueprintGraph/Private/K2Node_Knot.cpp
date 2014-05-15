// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "BlueprintGraphPrivatePCH.h"
#include "K2ActionMenuBuilder.h"

#define LOCTEXT_NAMESPACE "K2Node_Knot"

/////////////////////////////////////////////////////
// UK2Node_Knot

UK2Node_Knot::UK2Node_Knot(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	bCanRenameNode = true;
}

void UK2Node_Knot::AllocateDefaultPins()
{
	const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();

	const FString InputPinName(TEXT("InputPin"));
	const FString OutputPinName(TEXT("OutputPin"));

	UEdGraphPin* MyInputPin = CreatePin(EGPD_Input, Schema->PC_Wildcard, FString(), nullptr, /*bIsArray=*/ false, /*bIsReference=*/ false, InputPinName);
	MyInputPin->bDefaultValueIsIgnored = true;

	UEdGraphPin* MyOutputPin = CreatePin(EGPD_Output, Schema->PC_Wildcard, FString(), nullptr, /*bIsArray=*/ false, /*bIsReference=*/ false, OutputPinName);
}

FString UK2Node_Knot::GetTooltip() const
{
	//@TODO: Should pull the tooltip from the source pin
	return LOCTEXT("KnotTooltip", "Reroute Node (reroutes wires)").ToString();
}

FText UK2Node_Knot::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (TitleType == ENodeTitleType::EditableTitle)
	{
		return FText::FromString(NodeComment);
	}
	else
	{
		return LOCTEXT("KnotTitle", "Reroute Node");
	}
}

void UK2Node_Knot::GetMenuEntries(struct FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	if (ContextMenuBuilder.FromPin != nullptr)
	{
		UK2Node_Knot* TemplateNode = NewObject<UK2Node_Knot>(GetTransientPackage(), GetClass());

		FString EmptyCategory;
		FText MenuDesc = LOCTEXT("KnotMenuDescription", "Add Reroute Node...");
		FString Tooltip = TemplateNode->GetTooltip();//@TODO: Make this work +LOCTEXT("KnotMenuExtraTooltip", "\nYou can also create one by Shift+Dragging off a pin").ToString();
		FString Keywords = TemplateNode->GetKeywords();

		TSharedPtr<FEdGraphSchemaAction_K2NewNode> NodeAction = FK2ActionMenuBuilder::AddNewNodeAction(ContextMenuBuilder, EmptyCategory, MenuDesc, Tooltip, 0, Keywords);
		NodeAction->NodeTemplate = TemplateNode;
		NodeAction->SearchTitle = TemplateNode->GetNodeSearchTitle();
	}
}

void UK2Node_Knot::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	UEdGraphPin* MyInputPin = GetInputPin();
	UEdGraphPin* MyOutputPin = GetOutputPin();

	K2Schema->CombineTwoPinNetsAndRemoveOldPins(MyInputPin, MyOutputPin);
}

bool UK2Node_Knot::IsNodeSafeToIgnore() const
{
	return true;
}

void UK2Node_Knot::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
	UEdGraphPin* MyInputPin = GetInputPin();
	UEdGraphPin* MyOutputPin = GetOutputPin();

	const int32 NumLinks = MyInputPin->LinkedTo.Num() + MyOutputPin->LinkedTo.Num();

	if (Pin->LinkedTo.Num() > 0)
	{
		// Just made a connection, was it the first?
		if (NumLinks == 1)
		{
			UEdGraphPin* TypeSource = Pin->LinkedTo[0];

			MyInputPin->PinType = TypeSource->PinType;
			MyOutputPin->PinType = TypeSource->PinType;
		}
	}
	else
	{
		// Just broke a connection, was it the last?
		if (NumLinks == 0)
		{
			// Revert to wildcard
			MyInputPin->BreakAllPinLinks();
			MyInputPin->PinType.ResetToDefaults();
			MyInputPin->PinType.PinCategory = K2Schema->PC_Wildcard;

			MyOutputPin->BreakAllPinLinks();
			MyOutputPin->PinType.ResetToDefaults();
			MyOutputPin->PinType.PinCategory = K2Schema->PC_Wildcard;
		}
	}
}

void UK2Node_Knot::PostReconstructNode()
{
	UEdGraphPin* MyInputPin = GetInputPin();
	UEdGraphPin* MyOutputPin = GetOutputPin();

	// Find a pin that has connections to use to jumpstart the wildcard process
	for (int32 PinIndex = 0; PinIndex < Pins.Num(); ++PinIndex)
	{
		if (Pins[PinIndex]->LinkedTo.Num() > 0)
		{
			// The pin is linked, continue to use its type as the type for all pins.
			UEdGraphPin* TypeSource = Pins[PinIndex]->LinkedTo[0];

			MyInputPin->PinType = TypeSource->PinType;
			MyOutputPin->PinType = TypeSource->PinType;
			break;
		}
	}
}

bool UK2Node_Knot::ShouldOverridePinNames() const
{
	return true;
}

FString UK2Node_Knot::GetPinNameOverride(const UEdGraphPin& Pin) const
{
	// Keep the pin size tiny
	return FString();
}

void UK2Node_Knot::OnRenameNode(const FString& NewName)
{
	NodeComment = NewName;
}

TSharedPtr<class INameValidatorInterface> UK2Node_Knot::MakeNameValidator() const
{
	// Comments can be duplicated, etc...
	return MakeShareable(new FDummyNameValidator(EValidatorResult::Ok));
}

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE

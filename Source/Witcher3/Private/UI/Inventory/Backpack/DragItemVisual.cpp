// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/Backpack/DragItemVisual.h"
#include "Components/Image.h"
void UDragItemVisual::SetVisualImage()
{
	if (VisualImage)
	{
		VisualImage->SetBrushFromTexture(ShowImage);
	}
}

// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#pragma once
#include "Widgets/SCompoundWidget.h"
#include "Brushes/SlateImageBrush.h"

class UTexture2D;

class SCorniferZoomPan : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCorniferZoomPan) : _Texture(nullptr), _InitialZoom(1.f) {}
		SLATE_ARGUMENT(UTexture2D*, Texture)
		SLATE_ARGUMENT(float, InitialZoom)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetTexture(UTexture2D* InTexture);
	void ResetView();

private:
	// SWidget overrides
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
						  const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
						  int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	void UpdateBrushFromTexture() const;
	float GetMinimumZoom(const FVector2D& ViewportSize) const;
	FVector2D ClampTranslation(const FVector2D& InTranslation, const FVector2D& ViewportSize) const;

	TWeakObjectPtr<UTexture2D> Texture;
	mutable FSlateBrush Brush;

	float Zoom = 1.f;
	FVector2D Translation = FVector2D::ZeroVector;
	bool bDragging = false;
	FVector2D DragOrigin = FVector2D::ZeroVector;
	FVector2D TranslationAtDragStart = FVector2D::ZeroVector;
};

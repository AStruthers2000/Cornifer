// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#pragma once
#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Brushes/SlateImageBrush.h"

class UTexture2D;

class SCorniferZoomPan : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCorniferZoomPan) : _Texture(nullptr), _InitialZoom(1.f), _MaxZoom(10.f), _ZoomSpeed(1.1f) {}
		SLATE_ARGUMENT(UTexture2D*, Texture)
		SLATE_ARGUMENT(float, InitialZoom)
		SLATE_ARGUMENT(float, MaxZoom)
		SLATE_ARGUMENT(float, ZoomSpeed)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	void SetTexture(UTexture2D* InTexture);
	void ResetView();

	void SetMaxZoom(float InMaxZoom) { MaxZoom = FMath::Max(1.0f, InMaxZoom); }
	void SetZoomSpeed(float InZoomSpeed) { ZoomSpeed = FMath::Max(1.01f, InZoomSpeed); }
	void SetZoom(float InZoom) { Zoom = FMath::Clamp(InZoom, 0.1f, MaxZoom); bDidFirstPaint = false; Invalidate(EInvalidateWidget::Paint); }
	void ResetTranslation() { Translation = FVector2D::ZeroVector; Invalidate(EInvalidateWidget::Paint); }

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

	mutable float Zoom = 1.f; // mutable so we can first-paint clamp inside const OnPaint
	mutable bool bDidFirstPaint = false;
	float MaxZoom = 10.f;
	float ZoomSpeed = 1.1f;
	FVector2D Translation = FVector2D::ZeroVector;
	bool bDragging = false;
	FVector2D DragOrigin = FVector2D::ZeroVector;
	FVector2D TranslationAtDragStart = FVector2D::ZeroVector;
};

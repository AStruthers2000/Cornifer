// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#pragma once
#include "CoreMinimal.h"
#include "Widgets/SPanel.h"
#include "Brushes/SlateImageBrush.h"
#include "Cornifer/CorniferDefaultValues.h"

class UTexture2D;

/**
 * A panel widget that displays a pannable/zoomable map image and positions child widgets in map space.
 * Replaces SCorniferZoomPan with support for multiple children positioned in map coordinates.
 */
class SCorniferMapView : public SPanel
{
public:
	/** Slot for child widgets positioned in map coordinates */
	class FMapSlot : public TSlotBase<FMapSlot>
	{
	public:
		FMapSlot()
			: TSlotBase<FMapSlot>()
			, MapPosition(FVector2D::ZeroVector)
			, Alignment(FVector2D(0.5f, 0.5f))
			, PixelOffset(FVector2D::ZeroVector)
		{
		}

		SLATE_SLOT_BEGIN_ARGS(FMapSlot, TSlotBase<FMapSlot>)
		SLATE_SLOT_END_ARGS()

		void Construct(const FChildren& SlotOwner, FSlotArguments&& InArgs)
		{
			TSlotBase<FMapSlot>::Construct(SlotOwner, MoveTemp(InArgs));
			if (InArgs.AttachedWidget)
			{
				this->AttachWidget(InArgs.AttachedWidget->AsShared());
			}
		}

		FMapSlot& MapPosition(FVector2D InPosition)
		{
			this->MapPosition = InPosition;
			return *this;
		}

		FMapSlot& Alignment(FVector2D InAlignment)
		{
			this->Alignment = InAlignment;
			return *this;
		}

		FMapSlot& PixelOffset(FVector2D InOffset)
		{
			this->PixelOffset = InOffset;
			return *this;
		}

		/** Position in map pixel coordinates (0,0 = top-left of image) */
		FVector2D MapPosition = FVector2D::ZeroVector;
		
		/** Alignment anchor point (0,0 = top-left, 0.5,0.5 = center, 1,1 = bottom-right) */
		FVector2D Alignment = FVector2D(0.5f, 0.5f);
		
		/** Additional offset in screen pixels after alignment */
		FVector2D PixelOffset = FVector2D::ZeroVector;
	};

	SLATE_BEGIN_ARGS(SCorniferMapView)
		: _Texture(nullptr)
		, _InitialZoom(1.f)
		, _MaxZoom(10.f)
		, _ZoomSpeed(1.1f)
	{
		_Visibility = EVisibility::Visible;
	}
		SLATE_ARGUMENT(UTexture2D*, Texture)
		SLATE_ARGUMENT(float, InitialZoom)
		SLATE_ARGUMENT(float, MaxZoom)
		SLATE_ARGUMENT(float, ZoomSpeed)
		SLATE_SLOT_ARGUMENT(FMapSlot, Slots)
	SLATE_END_ARGS()

	SCorniferMapView();

	void Construct(const FArguments& InArgs);

	/** Add a child widget at a specific map position */
	FMapSlot& AddSlot()
	{
		FMapSlot& NewSlot = *(new FMapSlot());
		Children.AddSlot(MoveTemp(NewSlot));
		return Children[Children.Num() - 1];
	}

	/** Remove a specific child widget */
	int32 RemoveSlot(const TSharedRef<SWidget>& SlotWidget)
	{
		for (int32 i = 0; i < Children.Num(); ++i)
		{
			if (Children[i].GetWidget() == SlotWidget)
			{
				Children.RemoveAt(i);
				return i;
			}
		}
		return -1;
	}

	/** Remove all child widgets */
	void ClearChildren()
	{
		Children.Empty();
	}

	void SetTexture(UTexture2D* InTexture);
	void ResetView();

	void SetMaxZoom(const float InMaxZoom)
	{
		MaxZoom = FMath::Max(GMinimum_Max_Zoom_Level, InMaxZoom);
	}

	void SetZoomSpeed(const float InZoomSpeed)
	{
		ZoomSpeed = FMath::Max(GMinimum_Zoom_Speed, InZoomSpeed);
	}

	void SetZoom(const float InZoom)
	{
		Zoom = FMath::Clamp(InZoom, GMinimum_Initial_Zoom_Level, MaxZoom);
		bDidFirstPaint = false;
		Invalidate(EInvalidateWidget::LayoutAndVolatility);
	}
	
	void ResetTranslation()
	{
		Translation = FVector2D::ZeroVector;
		Invalidate(EInvalidateWidget::LayoutAndVolatility);
	}

	/** Convert map-space coordinates to screen-space coordinates */
	FVector2D MapToScreen(const FVector2D& MapPosition, const FGeometry& Geometry) const;

	/** Convert screen-space coordinates to map-space coordinates */
	FVector2D ScreenToMap(const FVector2D& ScreenPosition, const FGeometry& Geometry) const;

	// SWidget interface
	virtual void OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;
	virtual FVector2D ComputeDesiredSize(float) const override;
	virtual FChildren* GetChildren() override { return &Children; }
	virtual bool SupportsKeyboardFocus() const override { return true; }
	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override { return FReply::Handled(); }
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
						  const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
						  int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

private:
	void UpdateBrushFromTexture() const;
	float GetMinimumZoom(const FVector2D& ViewportSize) const;
	FVector2D ClampTranslation(const FVector2D& InTranslation, const FVector2D& ViewportSize) const;

	/** Slots for child widgets */
	TPanelChildren<FMapSlot> Children;

	TWeakObjectPtr<UTexture2D> Texture;
	mutable FSlateBrush Brush;

	mutable float Zoom = 1.f;
	mutable bool bDidFirstPaint = false;
	float MaxZoom = 10.f;
	float ZoomSpeed = 1.1f;
	FVector2D Translation = FVector2D::ZeroVector;
	bool bDragging = false;
	FVector2D DragOrigin = FVector2D::ZeroVector;
	FVector2D TranslationAtDragStart = FVector2D::ZeroVector;
};

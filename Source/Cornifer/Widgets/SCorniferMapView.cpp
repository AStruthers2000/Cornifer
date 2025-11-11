// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#include "SCorniferMapView.h"
#include "Engine/Texture2D.h"
#include "Rendering/DrawElements.h"
#include "Styling/SlateBrush.h"

SCorniferMapView::SCorniferMapView()
	: Children(this)
{
}

void SCorniferMapView::Construct(const FArguments& InArgs)
{
	Texture = InArgs._Texture;
	MaxZoom = FMath::Max(1.0f, InArgs._MaxZoom);
	ZoomSpeed = FMath::Max(1.01f, InArgs._ZoomSpeed);
	UpdateBrushFromTexture();
	// Note: We can't properly clamp zoom here since we don't have viewport size yet.
	// Will be clamped on first paint/interaction.
	Zoom = FMath::Clamp(InArgs._InitialZoom, 0.1f, MaxZoom);
	bDidFirstPaint = false;

	// Process any child slots from arguments
	Children.AddSlots(MoveTemp(const_cast<TArray<FMapSlot::FSlotArguments>&>(InArgs._Slots)));
}

void SCorniferMapView::SetTexture(UTexture2D* InTexture)
{
	Texture = InTexture;
	UpdateBrushFromTexture();
	Invalidate(EInvalidateWidget::LayoutAndVolatility);
}

void SCorniferMapView::ResetView()
{
	Translation = FVector2D::ZeroVector;
	// Zoom will be clamped to minimum on next paint/interaction
	Zoom = 1.f;
	bDidFirstPaint = false; // ensure clamp occurs again
	Invalidate(EInvalidateWidget::LayoutAndVolatility);
}

void SCorniferMapView::UpdateBrushFromTexture() const
{
	Brush = FSlateBrush();
	if (UTexture2D* T = Texture.Get())
	{
		Brush.SetResourceObject(T);
		Brush.ImageSize = FVector2D(T->GetSizeX(), T->GetSizeY());
		Brush.Tiling = ESlateBrushTileType::NoTile;
		Brush.DrawAs = ESlateBrushDrawType::Image;
	}
}

FVector2D SCorniferMapView::MapToScreen(const FVector2D& MapPosition, const FGeometry& Geometry) const
{
	const FVector2D ViewSize = Geometry.GetLocalSize();
	const FVector2D ImgSize = Brush.ImageSize * Zoom;
	const FVector2D ImageTopLeft = (ViewSize - ImgSize) * 0.5f + Translation;
	
	// Convert map position to screen position
	return ImageTopLeft + MapPosition * Zoom;
}

FVector2D SCorniferMapView::ScreenToMap(const FVector2D& ScreenPosition, const FGeometry& Geometry) const
{
	const FVector2D ViewSize = Geometry.GetLocalSize();
	const FVector2D ImgSize = Brush.ImageSize * Zoom;
	const FVector2D ImageTopLeft = (ViewSize - ImgSize) * 0.5f + Translation;
	
	// Convert screen position to map position
	return (ScreenPosition - ImageTopLeft) / Zoom;
}

void SCorniferMapView::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	// Arrange each child widget based on its map position
	for (int32 i = 0; i < Children.Num(); ++i)
	{
		const FMapSlot& CurSlot = Children[i];
		const TSharedRef<SWidget>& Widget = CurSlot.GetWidget();
		
		if (Widget->GetVisibility() != EVisibility::Collapsed)
		{
			// Convert map position to screen position
			const FVector2D ScreenPos = MapToScreen(CurSlot.MapPosition, AllottedGeometry);
			
			// Get the desired size of the child widget
			const FVector2D ChildDesiredSize = Widget->GetDesiredSize();
			
			// Apply alignment offset
			const FVector2D AlignmentOffset = -ChildDesiredSize * CurSlot.Alignment;
			
			// Final position with pixel offset
			const FVector2D FinalPos = ScreenPos + AlignmentOffset + CurSlot.PixelOffset;
			
			// Arrange the child
			ArrangedChildren.AddWidget(
				AllottedGeometry.MakeChild(Widget, FinalPos, ChildDesiredSize)
			);
		}
	}
}

FVector2D SCorniferMapView::ComputeDesiredSize(float) const
{
	// The panel itself doesn't have an intrinsic size - it fills its allocated space
	return FVector2D(100.f, 100.f);
}

int32 SCorniferMapView::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
                                const int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 MaxLayerId = LayerId;

	if (Texture.IsValid())
	{
		// First paint: clamp initial zoom so the image at least covers the viewport
		if (!bDidFirstPaint)
		{
			const FVector2D ViewSizeFirst = AllottedGeometry.GetLocalSize();
			const float MinZoom = GetMinimumZoom(ViewSizeFirst);
			Zoom = FMath::Clamp(Zoom, MinZoom, MaxZoom);
			bDidFirstPaint = true;
		}

		const FVector2D ViewSize = AllottedGeometry.GetLocalSize();
		const FVector2D ImgSize = Brush.ImageSize * Zoom;

		// Centered plus user translation
		const FVector2D Pos = (ViewSize - ImgSize) * 0.5f + Translation;
		const FPaintGeometry Geo = AllottedGeometry.ToPaintGeometry(ImgSize, FSlateLayoutTransform(Pos));

		FSlateDrawElement::MakeBox(OutDrawElements, LayerId, Geo, &Brush);
		MaxLayerId = LayerId + 1;
	}

	// Paint children on top of the background image
	MaxLayerId = SPanel::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, MaxLayerId, InWidgetStyle, bParentEnabled);
	
	return MaxLayerId;
}

FReply SCorniferMapView::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bDragging = true;
		DragOrigin = MouseEvent.GetScreenSpacePosition();
		TranslationAtDragStart = Translation;
		return FReply::Handled().CaptureMouse(AsShared());
	}
	return FReply::Unhandled();
}

FReply SCorniferMapView::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (bDragging && MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bDragging = false;
		return FReply::Handled().ReleaseMouseCapture();
	}
	return FReply::Unhandled();
}

FReply SCorniferMapView::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (bDragging)
	{
		const FVector2D Delta = MouseEvent.GetScreenSpacePosition() - DragOrigin;
		const FVector2D ViewSize = MyGeometry.GetLocalSize();
		Translation = ClampTranslation(TranslationAtDragStart + Delta, ViewSize);
		Invalidate(EInvalidateWidget::LayoutAndVolatility);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply SCorniferMapView::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	const FVector2D ViewSize = MyGeometry.GetLocalSize();
	const float MinZoom = GetMinimumZoom(ViewSize);
	
	float OldZoom = Zoom;
	const float Factor = (MouseEvent.GetWheelDelta() > 0 ? ZoomSpeed : 1.f / ZoomSpeed);
	Zoom = FMath::Clamp(Zoom * Factor, MinZoom, MaxZoom);

	// Zoom about cursor point: adjust translation so the cursor stays on the same image point.
	const FVector2D ImgSizeOld = Brush.ImageSize * OldZoom;
	const FVector2D ImgSizeNew = Brush.ImageSize * Zoom;

	const FVector2D CenterOld = (ViewSize - ImgSizeOld) * 0.5f + Translation;
	const FVector2D CursorLocal = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	const FVector2D Alpha = (CursorLocal - CenterOld) / ImgSizeOld; // 0..1
	const FVector2D CenterNew = CursorLocal - ImgSizeNew * Alpha;

	Translation = ClampTranslation(CenterNew - (ViewSize - ImgSizeNew) * 0.5f, ViewSize);

	Invalidate(EInvalidateWidget::LayoutAndVolatility);
	return FReply::Handled();
}

float SCorniferMapView::GetMinimumZoom(const FVector2D& ViewportSize) const
{
	if (Brush.ImageSize.X <= 0.f || Brush.ImageSize.Y <= 0.f || ViewportSize.X <= 0.f || ViewportSize.Y <= 0.f)
	{
		return 0.1f; // fallback
	}
	
	// Calculate zoom needed to fit viewport width and height
	const float ZoomToFitWidth = ViewportSize.X / Brush.ImageSize.X;
	const float ZoomToFitHeight = ViewportSize.Y / Brush.ImageSize.Y;
	
	// Use the larger of the two to ensure the image always covers the viewport
	return FMath::Max(ZoomToFitWidth, ZoomToFitHeight);
}

FVector2D SCorniferMapView::ClampTranslation(const FVector2D& In, const FVector2D& ViewportSize) const
{
	const FVector2D ImgSize = Brush.ImageSize * Zoom;
	// Always clamp translation so image borders never leave viewport borders
	const FVector2D MaxOffset = (ImgSize - ViewportSize) * 0.5f;

	FVector2D Out = In;
	// Clamp each axis independently
	Out.X = FMath::Clamp(Out.X, -MaxOffset.X, MaxOffset.X);
	Out.Y = FMath::Clamp(Out.Y, -MaxOffset.Y, MaxOffset.Y);
	return Out;
}

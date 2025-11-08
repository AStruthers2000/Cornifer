// Copyright (C) 2025 AStruthers2000 - All Rights Reserved

#include "SCorniferZoomPan.h"
#include "Engine/Texture2D.h"
#include "Rendering/DrawElements.h"
#include "Styling/SlateBrush.h"

void SCorniferZoomPan::Construct(const FArguments& InArgs)
{
    Texture = InArgs._Texture;
    Zoom = FMath::Clamp(InArgs._InitialZoom, 0.1f, 10.f);
    UpdateBrushFromTexture();
    ChildSlot[SNullWidget::NullWidget]; // we draw directly
}

void SCorniferZoomPan::SetTexture(UTexture2D* InTexture)
{
    Texture = InTexture;
    UpdateBrushFromTexture();
    Invalidate(EInvalidateWidget::Paint);
}

void SCorniferZoomPan::ResetView()
{
    Zoom = 1.f; Translation = FVector2D::ZeroVector;
    Invalidate(EInvalidateWidget::Paint);
}

void SCorniferZoomPan::UpdateBrushFromTexture() const
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

int32 SCorniferZoomPan::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
                                const int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    if (!Texture.IsValid()) return LayerId;

    const FVector2D ViewSize = AllottedGeometry.GetLocalSize();
    const FVector2D ImgSize = Brush.ImageSize * Zoom;

    // Centered plus user translation
    FVector2D Pos = (ViewSize - ImgSize) * 0.5f + Translation;
    FPaintGeometry Geo = AllottedGeometry.ToPaintGeometry(ImgSize, FSlateLayoutTransform(Pos));

    FSlateDrawElement::MakeBox(OutDrawElements, LayerId, Geo, &Brush);
    return LayerId + 1;
}

FReply SCorniferZoomPan::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
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

FReply SCorniferZoomPan::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (bDragging && MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        bDragging = false;
        return FReply::Handled().ReleaseMouseCapture();
    }
    return FReply::Unhandled();
}

FReply SCorniferZoomPan::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (bDragging)
    {
        const FVector2D Delta = MouseEvent.GetScreenSpacePosition() - DragOrigin;
        const FVector2D ViewSize = MyGeometry.GetLocalSize();
        Translation = ClampTranslation(TranslationAtDragStart + Delta, ViewSize);
        Invalidate(EInvalidateWidget::Paint);
        return FReply::Handled();
    }
    return FReply::Unhandled();
}

FReply SCorniferZoomPan::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    float OldZoom = Zoom;
    Zoom = FMath::Clamp(Zoom * (MouseEvent.GetWheelDelta() > 0 ? 1.1f : 1.f/1.1f), 0.1f, 10.f);

    // Zoom about cursor point: adjust translation so the cursor stays on the same image point.
    const FVector2D ViewSize = MyGeometry.GetLocalSize();
    const FVector2D ImgSizeOld = Brush.ImageSize * OldZoom;
    const FVector2D ImgSizeNew = Brush.ImageSize * Zoom;

    const FVector2D CenterOld = (ViewSize - ImgSizeOld) * 0.5f + Translation;
    const FVector2D CursorLocal = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
    const FVector2D Alpha = (CursorLocal - CenterOld) / ImgSizeOld; // 0..1
    const FVector2D CenterNew = CursorLocal - ImgSizeNew * Alpha;

    Translation = ClampTranslation(CenterNew - (ViewSize - ImgSizeNew) * 0.5f, ViewSize);

    Invalidate(EInvalidateWidget::Paint);
    return FReply::Handled();
}

FVector2D SCorniferZoomPan::ClampTranslation(const FVector2D& In, const FVector2D& ViewportSize) const
{
    const FVector2D ImgSize = Brush.ImageSize * Zoom;
    // Allow some overscroll if image smaller than view; clamp when larger.
    const FVector2D MaxOffset = (ImgSize - ViewportSize) * 0.5f;
    if (ImgSize.X <= ViewportSize.X && ImgSize.Y <= ViewportSize.Y)
        return In; // no clamp needed when fully inside

    FVector2D Out = In;
    if (ImgSize.X > ViewportSize.X) { Out.X = FMath::Clamp(Out.X, -MaxOffset.X, MaxOffset.X); }
    if (ImgSize.Y > ViewportSize.Y) { Out.Y = FMath::Clamp(Out.Y, -MaxOffset.Y, MaxOffset.Y); }
    return Out;
}
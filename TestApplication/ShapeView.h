// Copyright (C) The Company

#pragma once

#define DEFAULT_SIZE 200

// ShapeViewLayer
enum class ShapeViewLayer
{
	// Shape views themselves are rendered in this layer.
	Default,

	// Helper stuff is rendered in this layer (on top of all shape views).
	Helper
};

// Allowed marker cursors.
enum class MarkerCursor
{
	None,
	SizeNESW,
	SizeNWSE
};

// Base class for renderable shapes.
class IShapeView
{
public:
	// Must be called during creation to correctly align it to
	// the center of the working area.
	virtual void InitInstance(class IDocumentView* docView) = 0;

	// Should return true if the point is found inside the shape.
	virtual bool HitTest(const CPoint& p) const = 0;

	// Rectangle that contains the shape.
	virtual CRect GetRect() const = 0;
	virtual void SetRect(const CRect& rect) = 0;

	// Changes the position of the shape.
	virtual void SetPosition(const CPoint& p) = 0;
	virtual CPoint GetPosition() const = 0;

	virtual bool IsSelected() = 0;
	virtual void SetSelected(bool value) = 0;

	// Markers to resize objects are generalized. For example, beziers' markers can be used
	// to edit control points.
	virtual int GetMarkerCount() const = 0;

	// Gets the rectangle of the marker at the given index.
	virtual CRect GetMarkerRect(int index) const = 0;

	// Describes the cursor of the marker (when hovering over it).
	virtual MarkerCursor GetMarkerCursor(int index) const = 0;

	// Modifies the marker's position. For convex shapes, this means modifying rects.
	// For beziers, it can mean modifying control points, for example.
	virtual void SetMarkerPosition(int index, const CPoint& p) = 0;
	virtual CPoint GetMarkerPosition(int index) const = 0;

	// Poor man's reflection.
	// TODO Does Visual C++/MFC have something better? Is their RTTI reliable?
	virtual const std::string GetTypeName() const = 0;

	// Renders the shape.
	virtual void Render(CDC* pDC, ShapeViewLayer layer) = 0;
};

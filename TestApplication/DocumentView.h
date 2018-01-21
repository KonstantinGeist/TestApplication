// Copyright (C) The Company

#pragma once

#include "ShapeView.h"

class IDocumentViewEventHandler;

// Abstracted document view logic that doesn't depend on Windows-specific
// functionality.
class CDocumentView final
{
public:
	CDocumentView();

	// See IDocumentViewEventHandler for more details.
	void SetEventHandler(IDocumentViewEventHandler* handler);

	void AddShapeView(std::shared_ptr<IShapeView> shapeView);
	void InsertShapeView(std::shared_ptr<IShapeView> shapeView, int index);
	void RemoveShapeView(std::shared_ptr<IShapeView> shapeView);
	void RemoveShapeViewAt(int index);
	void Clear();

	// Returns the full list of shape views. Useful for the presenter.
	const std::vector<std::shared_ptr<IShapeView>>& GetShapeViews() const;

	std::shared_ptr<IShapeView> GetSelected() const;

	// Attempts a click and tries to find an object which is found
	// under the point, and marks it as the selected object (if any).
	// The selected object can be retrieved with GetSelected().
	// Returns true if selection changed.
	bool Select(const CPoint& p);

	// Selects an existing object.
	// If the value is null, removes any selection.
	void Select(std::shared_ptr<IShapeView> shapeView);

	// Attempts a drag action which will happen if there's a selected object
	// and we're currently in the dragging mode.
	// Returns true if dragging was successful.
	// Fires the IDocumentViewEventHandler::OnBeginShapeViewDrag(..) event
	// if it's the first call.
	bool Drag(const CPoint& p);

	// Finalizes dragging. Returns false if no dragging was performed.
	// Fires the IDocumentViewEventHandler::OnEndShapeViewDrag(..) event.
	bool EndDrag(const CPoint& p);

	// Gets the current or previous dragging's start position.
	CPoint GetDragStartPosition() const;

	// Gets the last position updated by Drag(..). Can be the position
	// of a shape or the position of a marker.
	CPoint GetLastDragPosition() const;

	// Attempts a hover action. Returns false if no hovering was performed.
	bool Hover(const CPoint& p);

	// Deletes the currently selected shape.
	// Returns the deleted shape view (if successful), which is required
	// for proper do/undo.
	// `out_index` returns the layer index the shape was in.
	std::shared_ptr<IShapeView> RemoveSelected(int* out_index);

	// Renders all shapes.
	void Render(CDC* pDC);

	// Returns the working area.
	CRect GetRect() const;

	// Sets the working area.
	void SetRect(const CRect& rect);

	// Returns the current marker cursor if a marker is hovered.
	MarkerCursor GetCurrentMarkerCursor() const;

	// Returns the current marker index if a marker is hovered.
	int GetCurrentMarkerIndex() const;

	// Use this method when you update data retrieved from the document view
	// (for example, shapeView->SetPosition(..)) and you want to notify the view
	// of changes (they are not automatically marked as changed for performance
	// and architectural reasons). Methods of CDocumentView itself should
	// automatically mark it as changed.
	void MarkChanged();

private:
	// Cannot move shapes outside of the working area.
	static CPoint restrictToRect(const CPoint& targetPos,
								 const CRect& shapeViewRect,
								 const CRect& documentRect);
	static CPoint restrictToRect(const CPoint& targetPos, const CRect& documentRect);

private:
	IDocumentViewEventHandler* m_handler;

	// Shape views.

	std::vector<std::shared_ptr<IShapeView>> m_shapeViews;
	std::shared_ptr<IShapeView> m_selShapeView;

	// Basic.

	CRect m_rect;				// working area

	// Dragging-related.

	bool m_isDragging;			// if we're currently dragging
	CPoint m_dragStartPosition; // the start position of the shape at the start of the dragging
	CPoint m_lastDragPosition;  // the last drag position, can be read after calling EndDrag(..)
	CPoint m_dragOffset;		// the offset into the dragged shape for smooth dragging

	// Hovering- and marker- related.

	int m_curMarkerIndex;
	MarkerCursor m_curMarkerCursor;
};

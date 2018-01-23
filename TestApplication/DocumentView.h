// Copyright (C) The Company

#pragma once

#include "ShapeView.h"

class IDocumentViewEventHandler;

// Abstracted document view logic that doesn't depend on Windows-specific
// functionality.
class IDocumentView
{
public:
	// See IDocumentViewEventHandler for more details.
	virtual void SetEventHandler(IDocumentViewEventHandler* handler) = 0;

	virtual void AddShapeView(std::shared_ptr<IShapeView> shapeView) = 0;
	virtual void InsertShapeView(std::shared_ptr<IShapeView> shapeView, int index) = 0;
	virtual void RemoveShapeView(std::shared_ptr<IShapeView> shapeView) = 0;
	virtual void RemoveShapeViewAt(int index) = 0;
	virtual void Clear() = 0;

	// Returns the full list of shape views. Useful for the presenter.
	virtual const std::vector<std::shared_ptr<IShapeView>>& GetShapeViews() const = 0;

	virtual std::shared_ptr<IShapeView> GetSelected() const = 0;

	// Attempts a click and tries to find an object which is found
	// under the point, and marks it as the selected object (if any).
	// The selected object can be retrieved with GetSelected().
	// Returns true if selection changed.
	virtual bool Select(const CPoint& p) = 0;

	// Selects an existing object.
	// If the value is null, removes any selection.
	virtual void Select(std::shared_ptr<IShapeView> shapeView) = 0;

	// Attempts a drag action which will happen if there's a selected object
	// and we're currently in the dragging mode.
	// Returns true if dragging was successful.
	// Fires the IDocumentViewEventHandler::OnBeginShapeViewDrag(..) event
	// if it's the first call.
	virtual bool Drag(const CPoint& p) = 0;

	// Finalizes dragging. Returns false if no dragging was performed.
	// Fires the IDocumentViewEventHandler::OnEndShapeViewDrag(..) event.
	virtual bool EndDrag(const CPoint& p) = 0;

	// Gets the current or previous dragging's start position.
	virtual CPoint GetDragStartPosition() const = 0;

	// Gets the last position updated by Drag(..). Can be the position
	// of a shape or the position of a marker.
	virtual CPoint GetLastDragPosition() const = 0;

	// Attempts a hover action. Returns false if no hovering was performed.
	virtual bool Hover(const CPoint& p) = 0;

	// Deletes the currently selected shape.
	// Returns the deleted shape view (if successful), which is required
	// for proper do/undo.
	// `out_index` returns the layer index the shape was in.
	virtual std::shared_ptr<IShapeView> RemoveSelected(int* out_index) = 0;

	// Renders all shapes.
	virtual void Render(CDC* pDC) = 0;

	// Returns the working area.
	virtual CRect GetRect() const = 0;

	// Sets the working area.
	virtual void SetRect(const CRect& rect) = 0;

	// Returns the current marker cursor if a marker is hovered.
	virtual MarkerCursor GetCurrentMarkerCursor() const = 0;

	// Returns the current marker index if a marker is hovered.
	virtual int GetCurrentMarkerIndex() const = 0;

	// Use this method when you update data retrieved from the document view
	// (for example, shapeView->SetPosition(..)) and you want to notify the view
	// of changes (they are not automatically marked as changed for performance
	// and architectural reasons). Methods of IDocumentView itself should
	// automatically mark it as changed.
	virtual void MarkChanged() = 0;
};

class CDocumentView final: public IDocumentView
{
public:
	CDocumentView();

	virtual void SetEventHandler(IDocumentViewEventHandler* handler) override;
	virtual void AddShapeView(std::shared_ptr<IShapeView> shapeView) override;
	virtual void InsertShapeView(std::shared_ptr<IShapeView> shapeView, int index) override;
	virtual void RemoveShapeView(std::shared_ptr<IShapeView> shapeView) override;
	virtual void RemoveShapeViewAt(int index) override;
	virtual void Clear() override;
	virtual const std::vector<std::shared_ptr<IShapeView>>& GetShapeViews() const override;
	virtual std::shared_ptr<IShapeView> GetSelected() const override;
	virtual bool Select(const CPoint& p) override;
	virtual void Select(std::shared_ptr<IShapeView> shapeView) override;
	virtual bool Drag(const CPoint& p) override;
	virtual bool EndDrag(const CPoint& p) override;
	virtual CPoint GetDragStartPosition() const override;
	virtual CPoint GetLastDragPosition() const override;
	virtual bool Hover(const CPoint& p) override;
	virtual std::shared_ptr<IShapeView> RemoveSelected(int* out_index) override;
	virtual void Render(CDC* pDC) override;
	virtual CRect GetRect() const override;
	virtual void SetRect(const CRect& rect) override;
	virtual MarkerCursor GetCurrentMarkerCursor() const override;
	virtual int GetCurrentMarkerIndex() const override;
	virtual void MarkChanged() override;

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

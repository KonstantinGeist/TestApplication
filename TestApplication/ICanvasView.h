// Copyright (C) The Company

#pragma once

#include "IShapeView.h"
#include "IMarkerView.h"

namespace view
{
	// Cursors supported by ICanvasView.
	enum class ECanvasCursor
	{
		Arrow,
		Hand,
		SizeNESW,
		SizeNWSE
	};

	class ICanvasViewListener
	{
	public:
		virtual void OnDeleteKey() = 0;

		// Global canvas mouse events.

		virtual void OnMouseDown(const CPoint& p) = 0;
		virtual void OnMouseMove(const CPoint& p) = 0;
		virtual void OnMouseUp(const CPoint& p) = 0;

		virtual ~ICanvasViewListener() = default;
	};

	// Views are now absolutely dumb and contain no logic other than rendering shape views,
	// rendering marker views, changing render states (for example, changing cursors), and reporting
	// user events. Everything else is moved to the presenter (for logic) and application model
	// (for state).
	class ICanvasView
	{
	public:
		virtual std::shared_ptr<IShapeView> InsertShapeView(const std::string& typeName, int index) = 0;
		virtual void RemoveShapeView(std::shared_ptr<IShapeView> view) = 0;
		virtual void SelectShapeView(std::shared_ptr<IShapeView> view) = 0;

		virtual std::shared_ptr<IMarkerView> InsertMarkerView() = 0;
		virtual void RemoveMarkerView(std::shared_ptr<IMarkerView> view) = 0;

		virtual void SetCursor(ECanvasCursor cursor) = 0;
		virtual ECanvasCursor GetCursor() const = 0;

		virtual void SetMouseCapture(bool b) = 0;

		virtual void SetRect(const CRect& rect) = 0;

		// TODO should Views invalidate themselves on change or should the presenter do it?
		// Currently it does not invalidate itself, the presenter does it, to avoid excessive
		// invalidations (think of adding many shape views in a row).
		virtual void Invalidate() = 0;

		virtual void SetListener(ICanvasViewListener* listener) = 0;

		virtual ~ICanvasView() = default;
	};

}
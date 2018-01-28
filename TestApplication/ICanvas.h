// Copyright (C) The Company

#pragma once

#include "IShape.h"
#include "IMarker.h"
#include "History.h"

namespace pres_model
{
	class ICanvasListener
	{
	public:
		virtual void OnShapeAdded(std::shared_ptr<IShape> shape, int index) = 0;
		virtual void OnShapeRemoved(std::shared_ptr<IShape> shape, int index) = 0;
		virtual void OnSelectionChanged(std::shared_ptr<IShape> shape) = 0;
		virtual void OnRectChanged(const CRect& rect) = 0;
		virtual void OnMarkerAdded(std::shared_ptr<IMarker> marker) = 0;
		virtual void OnMarkerRemoved(std::shared_ptr<IMarker> marker) = 0;

		virtual ~ICanvasListener() = default;
	};

	class ICanvas
	{
	public:
		// TODO move to pres_model::IShapeCollection?
		virtual void AddShape(const std::string& typeName) = 0;
		virtual void RemoveShape(std::shared_ptr<IShape> shape) = 0;
		virtual std::shared_ptr<IShape> GetShape(int index) const = 0;
		virtual int GetShapeIndex(IShape* shape) const = 0;
		virtual int GetShapeCount() const = 0;

		virtual std::shared_ptr<IShape> GetSelection() const = 0;
		virtual void SetSelection(std::shared_ptr<IShape> shape) = 0;

		// TODO move to pres_model::IMarkerCollection?
		virtual void AddMarker(EMarkerKind kind, const CRect& rect) = 0;
		virtual void RemoveMarker(std::shared_ptr<IMarker> marker) = 0;
		virtual int GetMarkerCount() const = 0;
		virtual std::shared_ptr<IMarker> GetMarker(int index) const = 0;

		virtual CRect GetRect() const = 0;

		virtual IHistory* GetHistory() const = 0;
		virtual void SetListener(ICanvasListener* listener) = 0;
	};

}

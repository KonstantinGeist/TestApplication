// Copyright (C) The Company

#pragma once

#include "ICanvas.h"
#include "ICanvasView.h"
#include "IResizeMarkerUpdater.h"
#include "MarkerPresenter.h"
#include "ShapePresenter.h"

namespace presenter
{
	enum class ECanvasEditMode
	{
		None,
		Dragging,
		Resizing
	};

	class CCanvasPresenter : public pres_model::ICanvasListener,
							 public view::ICanvasViewListener,
							 public IResizeMarkerUpdater
	{
	public:
		CCanvasPresenter(pres_model::ICanvas* presModel, view::ICanvasView* view);

	private:

		// NOTE Precise shape information such as ContainsPoint is now queried from the
		// model as views are now "dumb" and should not be used for querying state;
		// the presenter must be careful about properly syncing the view with the model,
		// or there can be a mismatch.
		// TODO should be adjusted for global coords<=>local coord conversions when
		// scrollbars/zooming are implemented
		std::shared_ptr<pres_model::IShape> findShapeUnderPoint(const CPoint& p) const;

		std::shared_ptr<pres_model::IMarker> findResizeMarkerUnderPoint(const CPoint& p) const;

		// Resize marker management.

		// deletes all known (!) resize markers from the model
		void clearResizeMarkers();

		// implements view::IResizeMarkerUpdater
		// adds new resize markers or updates existing ones around a selection rect
		virtual void UpdateResizeMarkers(const CRect& rect) override;

		// Resizing and dragging.

		void beginResize(std::shared_ptr<pres_model::IMarker> marker, const CPoint& p);
		void resize(const CPoint& p);
		void endResize(const CPoint& p);

		void unselect();

		// TODO is this OK that the dragging logic is contained in Presenter?
		// The view is passive and only renders stuff.
		void beginDrag(std::shared_ptr<pres_model::IShape> shape, const CPoint& p);
		void drag(const CPoint& p);
		void endDrag(const CPoint& p);

		void tryHover(const CPoint& p);

		// Implements view::ICanvasViewListener

		virtual void OnDeleteKey() override;

		virtual void OnMouseDown(const CPoint& p) override;
		virtual void OnMouseMove(const CPoint& p) override;
		virtual void OnMouseUp(const CPoint& p) override;

		// Implements ApplicationModel.ICanvasListener

		virtual void OnShapeAdded(std::shared_ptr<pres_model::IShape> shape, int index) override;
		virtual void OnShapeRemoved(std::shared_ptr<pres_model::IShape> shape, int index) override;
		void setSelectionPresenter(std::shared_ptr<CShapePresenter> selection);
		virtual void OnSelectionChanged(std::shared_ptr<pres_model::IShape> shape) override;
		virtual void OnMarkerAdded(std::shared_ptr<pres_model::IMarker> marker) override;
		virtual void OnMarkerRemoved(std::shared_ptr<pres_model::IMarker> marker) override;
		virtual void OnRectChanged(const CRect& newRect) override;

		// Edit mode helper functions.

		bool isDraggingMode() const;
		void setDraggingMode(bool b);
		bool isResizingMode() const;
		void setResizingMode(bool b);

		// Main.

		pres_model::ICanvas* m_presModel;
		view::ICanvasView* m_view;

		// Presenter maps.

		// TODO std::shared_ptr<CShapePresenter> etc. saves time
		std::unordered_map<std::shared_ptr<pres_model::IShape>, std::shared_ptr<presenter::CShapePresenter>> m_shapeToPresenter;
		std::unordered_map<std::shared_ptr<pres_model::IMarker>, std::shared_ptr<presenter::CMarkerPresenter>> m_markerToPresenter;

		// Selection-related.

		// WARNING do not update it directly, always use setSelectionPresenter(..)!
		// That way we can hook into the event.
		std::shared_ptr<CShapePresenter> m_selection;
		// Known (!) resize markers. The presenter should not touch markers it has no idea about.
		std::vector<std::shared_ptr<pres_model::IMarker>> m_resizeMarkers;

		// Dragging- and resizing- related.

		ECanvasEditMode m_editMode;
		CPoint m_offset;
		CRect m_prevRect;
		std::shared_ptr<pres_model::IShape> m_modifiedShape;
		pres_model::EMarkerKind m_resizeKind;
	};

}

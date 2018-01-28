// Copyright (C) The Company

#include "stdafx.h"
#include "CanvasPresenter.h"

namespace presenter
{

	CCanvasPresenter::CCanvasPresenter(pres_model::ICanvas* presModel, view::ICanvasView* view)
		: m_presModel (presModel), m_view(view),
		  m_editMode(ECanvasEditMode::None), m_resizeKind(pres_model::EMarkerKind::TopLeft)
	{
		m_presModel->SetListener(this);
		m_view->SetListener(this);

		m_view->SetRect(presModel->GetRect());
	}

	std::shared_ptr<pres_model::IShape> CCanvasPresenter::findShapeUnderPoint(const CPoint& p) const
	{
		int shapeCount = m_presModel->GetShapeCount();
		for (int i = shapeCount - 1; i >= 0; i--)
		{
			auto shape = m_presModel->GetShape(i);
			if (shape->ContainsPoint(p))
			{
				return shape;
			}
		}

		return nullptr;
	}

	std::shared_ptr<pres_model::IMarker> CCanvasPresenter::findResizeMarkerUnderPoint(const CPoint& p) const
	{
		for (auto& resizeMarker: m_resizeMarkers)
		{
			if (resizeMarker->GetRect().PtInRect(p))
				return resizeMarker;
		}

		return nullptr;
	}

	// Resize marker management.

	void CCanvasPresenter::clearResizeMarkers()
	{
		for(auto marker: m_resizeMarkers)
		{
			m_presModel->RemoveMarker(marker);
		}

		m_resizeMarkers.clear();
	}

	// implements view::IResizeMarkerUpdater

	#define MARKER_SIZE 10
	static CRect getRectAroundPoint(const CPoint& p)
	{
		return CRect(p.x - MARKER_SIZE / 2,
			p.y - MARKER_SIZE / 2,
			p.x + MARKER_SIZE / 2,
			p.y + MARKER_SIZE / 2);
	}

	void CCanvasPresenter::UpdateResizeMarkers(const CRect& rect)
	{
		CRect rects[4];
		rects[0] = getRectAroundPoint(CPoint(rect.left, rect.top));
		rects[1] = getRectAroundPoint(CPoint(rect.right, rect.top));
		rects[2] = getRectAroundPoint(CPoint(rect.right, rect.bottom));
		rects[3] = getRectAroundPoint(CPoint(rect.left, rect.bottom));

		if (m_resizeMarkers.size() != 4)
		{
			// resize markers not created or already deleted => recreate them

			assert(m_resizeMarkers.size() == 0);

			pres_model::EMarkerKind markerKinds[4];
			markerKinds[0] = pres_model::EMarkerKind::TopLeft;
			markerKinds[1] = pres_model::EMarkerKind::TopRight;
			markerKinds[2] = pres_model::EMarkerKind::BottomRight;
			markerKinds[3] = pres_model::EMarkerKind::BottomLeft;

			for (int i = 0; i < 4; i++)
			{
				m_presModel->AddMarker(markerKinds[i], rects[i]);
				m_resizeMarkers.push_back(m_presModel->GetMarker(m_presModel->GetMarkerCount() - 1));
			}
		}
		else
		{
			// resize markers exist, we need to update their positions

			for (int i = 0; i < 4; i++)
			{
				m_resizeMarkers[i]->SetRect(rects[i]);
			}
		}
	}

	// Resizing and dragging.

	void CCanvasPresenter::beginResize(std::shared_ptr<pres_model::IMarker> marker, const CPoint& p)
	{
		if (!m_selection)
			return;

		if (!isResizingMode())
		{
			// The cursor is still valid from previous hovering, so we do not update it here.
			m_view->SetMouseCapture(true);

			m_resizeKind = marker->GetKind();
			m_offset = p;
			m_modifiedShape = m_selection->GetModel();
			m_prevRect = m_modifiedShape->GetRect();

			setResizingMode(true);
		}
	}

	static CPoint restrictToRect(const CPoint& targetPos, const CRect& documentRect)
	{
		CPoint r = targetPos;

		if (r.x < documentRect.left)
			r.x = documentRect.left;
		else if (r.x > documentRect.right)
			r.x = documentRect.right;
		if (r.y < documentRect.top)
			r.y = documentRect.top;
		else if (r.y > documentRect.bottom)
			r.y = documentRect.bottom;

		return r;
	}

	static CPoint restrictToRect(const CPoint& targetPos, const CRect& shapeViewRect, const CRect& documentRect)
	{
		const CPoint dif = targetPos - shapeViewRect.CenterPoint();
		CRect shapeViewRectWithOffset = shapeViewRect; // OffsetRect(..)'s mutability...
		shapeViewRectWithOffset.OffsetRect(dif);

		CPoint r = targetPos;
		if (shapeViewRectWithOffset.left < documentRect.left)
			r.x += documentRect.left - shapeViewRectWithOffset.left;
		else if (shapeViewRectWithOffset.right > documentRect.right)
			r.x -= shapeViewRectWithOffset.right - documentRect.right;
		if (shapeViewRectWithOffset.top < documentRect.top)
			r.y += documentRect.top - shapeViewRectWithOffset.top;
		else if (shapeViewRectWithOffset.bottom > documentRect.bottom)
			r.y -= shapeViewRectWithOffset.bottom - documentRect.bottom;
		return r;
	}

	#define MIN_SHAPE_SIZE 20
	void CCanvasPresenter::resize(const CPoint& _p)
	{
		if (!m_modifiedShape) // just in case
			return;

		const CPoint p (restrictToRect(_p, m_presModel->GetRect()));
		CRect newRect (m_modifiedShape->GetRect());

		switch (m_resizeKind)
		{
		case pres_model::EMarkerKind::TopLeft:
			if (newRect.right - p.x > MIN_SHAPE_SIZE)
				newRect.left = p.x;
			if (newRect.bottom - p.y > MIN_SHAPE_SIZE)
				newRect.top = p.y;
			break;

		case pres_model::EMarkerKind::TopRight:
			if (p.x - newRect.left > MIN_SHAPE_SIZE)
				newRect.right = p.x;
			if (newRect.bottom - p.y > MIN_SHAPE_SIZE)
				newRect.top = p.y;
			break;

		case pres_model::EMarkerKind::BottomRight:
			if (p.x - newRect.left > MIN_SHAPE_SIZE)
				newRect.right = p.x;
			if (p.y - newRect.top > MIN_SHAPE_SIZE)
				newRect.bottom = p.y;
			break;

		case pres_model::EMarkerKind::BottomLeft:
			if (newRect.right - p.x > MIN_SHAPE_SIZE)
				newRect.left = p.x;
			if (p.y - newRect.top > MIN_SHAPE_SIZE)
				newRect.bottom = p.y;
			break;
		}

		m_modifiedShape->SetRect(newRect);
	}

	void CCanvasPresenter::endResize(const CPoint& p)
	{
		m_modifiedShape->CommitRect(m_prevRect);

		m_view->SetCursor(view::ECanvasCursor::Arrow);
		m_view->SetMouseCapture(false);

		setResizingMode(false);
	}

	void CCanvasPresenter::unselect()
	{
		m_presModel->SetSelection(nullptr);
		m_view->SetCursor(view::ECanvasCursor::Arrow);
	}

	void CCanvasPresenter::beginDrag(std::shared_ptr<pres_model::IShape> shape, const CPoint& p)
	{
		if (!isDraggingMode())
		{
			m_presModel->SetSelection(shape);
			m_view->SetCursor(view::ECanvasCursor::Hand);
			m_view->SetMouseCapture(true);

			auto shapePos = shape->GetRect().CenterPoint();
			m_offset = CPoint(shapePos.x - p.x, shapePos.y - p.y);
			m_modifiedShape = shape;
			m_prevRect = m_modifiedShape->GetRect();

			setDraggingMode(true);
		}
	}

	static CRect moveRectangle(const CRect& rect, const CPoint& _p)
	{
		// TODO doesn't look like good code

		int center_x = (rect.right - rect.left) / 2;
		int center_y = (rect.bottom - rect.top) / 2;

		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		CPoint p = _p;
		p.x -= center_x;
		p.y -= center_y;

		return CRect(p.x, p.y, p.x + width, p.y + height);
	}

	void CCanvasPresenter::drag(const CPoint& p)
	{
		// Dragging.

		assert(m_modifiedShape);

		auto rect = m_modifiedShape->GetRect();
		auto pWithOffset = CPoint(p.x + m_offset.x, p.y + m_offset.y);
		pWithOffset = restrictToRect(pWithOffset, rect, m_presModel->GetRect());
		rect = moveRectangle(rect, pWithOffset);
		m_modifiedShape->SetRect(rect);

		m_view->Invalidate();
	}

	void CCanvasPresenter::endDrag(const CPoint& p)
	{
		m_modifiedShape->CommitRect(m_prevRect);

		m_view->SetCursor(view::ECanvasCursor::Arrow);
		m_view->SetMouseCapture(false);

		setDraggingMode(false);
	}

	void CCanvasPresenter::tryHover(const CPoint& p)
	{
		// Potentially hovering over known markers.

		auto marker = findResizeMarkerUnderPoint(p);

		view::ECanvasCursor canvasCursor;

		if (marker)
		{
			switch (marker->GetKind())
			{
			case pres_model::EMarkerKind::TopLeft:
				canvasCursor = view::ECanvasCursor::SizeNWSE;
				break;
			case pres_model::EMarkerKind::TopRight:
				canvasCursor = view::ECanvasCursor::SizeNESW;
				break;
			case pres_model::EMarkerKind::BottomRight:
				canvasCursor = view::ECanvasCursor::SizeNWSE;
				break;
			case pres_model::EMarkerKind::BottomLeft:
				canvasCursor = view::ECanvasCursor::SizeNESW;
				break;
			default:
				canvasCursor = view::ECanvasCursor::Arrow;
				break;
			}
		}
		else
		{
			canvasCursor = view::ECanvasCursor::Arrow;
		}

		m_view->SetCursor(canvasCursor);
	}

	// Implements view::ICanvasViewListener

	void CCanvasPresenter::OnDeleteKey()
	{
		// NOTE do not allow to delete if there's no selection or during dragging
		if (!m_selection || isDraggingMode())
			return;

		m_presModel->RemoveShape(m_selection->GetModel());
	}

	void CCanvasPresenter::OnMouseDown(const CPoint& p)
	{
		auto marker = findResizeMarkerUnderPoint(p);
		if (marker)
		{
			beginResize(marker, p);
		}
		else
		{
			auto shape = findShapeUnderPoint(p);
			if (shape)
				beginDrag(shape, p);
			else
				unselect();
		}
	}

	void CCanvasPresenter::OnMouseMove(const CPoint& p)
	{
		if (isDraggingMode())
			drag(p);
		else if (isResizingMode())
			resize(p);
		else
			tryHover(p);
	}

	void CCanvasPresenter::OnMouseUp(const CPoint& p)
	{
		if (isDraggingMode())
			endDrag(p);
		else if (isResizingMode())
			endResize(p);
	}

	// Implements ApplicationModel.ICanvasListener

	void CCanvasPresenter::OnShapeAdded(std::shared_ptr<pres_model::IShape> shape, int index)
	{
		auto shapeView = m_view->InsertShapeView(shape->GetTypeName(), index);
		m_view->Invalidate();
		auto shapePresenter = std::make_shared<CShapePresenter>(shape, shapeView, m_view);
		m_shapeToPresenter[shape] = shapePresenter;
	}

	void CCanvasPresenter::OnShapeRemoved(std::shared_ptr<pres_model::IShape> shape, int index)
	{
		auto shapePresenter = m_shapeToPresenter[shape];
		// TODO should the shape presenter delete its view? In OnShapeAdded, it was
		// the canvas presenter which created the view, so I remove it in the canvas
		// presenter as well for symmetry.
		m_view->RemoveShapeView(shapePresenter->GetView());
		m_view->Invalidate();
		if (shapePresenter == m_selection)
			setSelectionPresenter(nullptr);
		m_shapeToPresenter.erase(shape);
	}

	void CCanvasPresenter::setSelectionPresenter(std::shared_ptr<CShapePresenter> selection)
	{
		if (m_selection)
			m_selection->SetResizeMarkerUpdater(nullptr);

		m_selection = selection;

		if (selection)
		{
			m_selection->SetResizeMarkerUpdater(this);
			m_view->SelectShapeView(m_selection->GetView());
		}
		else
		{
			clearResizeMarkers();
			m_view->SelectShapeView(nullptr);
		}
	}

	void CCanvasPresenter::OnSelectionChanged(std::shared_ptr<pres_model::IShape> shape)
	{
		// This event comes from the app model, we need to sync the view with it.

		if (shape)
			setSelectionPresenter(m_shapeToPresenter[shape]);
		else
			setSelectionPresenter(nullptr);

		m_view->Invalidate();
	}

	void CCanvasPresenter::OnMarkerAdded(std::shared_ptr<pres_model::IMarker> marker)
	{
		auto markerView = m_view->InsertMarkerView();
		auto markerPresenter = std::make_shared<CMarkerPresenter>(marker, markerView, m_view);
		m_markerToPresenter[marker] = markerPresenter;
		m_view->Invalidate();
	}

	void CCanvasPresenter::OnMarkerRemoved(std::shared_ptr<pres_model::IMarker> marker)
	{
		// see also the comments in OnShapeRemoved(..)
		auto markerPresenter = m_markerToPresenter[marker];
		m_view->RemoveMarkerView(markerPresenter->GetView());
		m_markerToPresenter.erase(marker);
		m_view->Invalidate();
	}

	void CCanvasPresenter::OnRectChanged(const CRect& newRect)
	{
		m_view->SetRect(newRect);
		m_view->Invalidate();
	}

	// Edit mode helper functions.

	bool CCanvasPresenter::isDraggingMode() const
	{
		return m_editMode == ECanvasEditMode::Dragging;
	}

	void CCanvasPresenter::setDraggingMode(bool b)
	{
		if (b)
			m_editMode = ECanvasEditMode::Dragging;
		else
			m_editMode = ECanvasEditMode::None;
	}

	bool CCanvasPresenter::isResizingMode() const
	{
		return m_editMode == ECanvasEditMode::Resizing;
	}

	void CCanvasPresenter::setResizingMode(bool b)
	{
		if (b)
			m_editMode = ECanvasEditMode::Resizing;
		else
			m_editMode = ECanvasEditMode::None;
	}

}

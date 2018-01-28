// Copyright (C) The Company

#include "stdafx.h"
#include "MarkerPresenter.h"

namespace presenter
{

	CMarkerPresenter::CMarkerPresenter(
		std::shared_ptr<pres_model::IMarker> presModel,
		std::shared_ptr<view::IMarkerView> view,
		view::ICanvasView* canvasView)
		: m_presModel (presModel), m_view(view),
		  m_canvasView(canvasView)
	{
		m_presModel->SetListener(this);

		m_view->SetRect(m_presModel->GetRect());
	}

	std::shared_ptr<view::IMarkerView> CMarkerPresenter::GetView() const
	{
		return m_view;
	}

	// Implements ApplicationModel.IMarkerListener

	void CMarkerPresenter::OnRectChanged(const CRect& newRect)
	{
		m_view->SetRect(newRect);
		m_canvasView->Invalidate();
	}

}

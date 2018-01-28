// Copyright (C) The Company

#include "stdafx.h"
#include "ShapePresenter.h"

namespace presenter
{

	CShapePresenter::CShapePresenter(
		std::shared_ptr<pres_model::IShape> presModel,
		std::shared_ptr<view::IShapeView> view,
		view::ICanvasView* canvasView)
		: m_presModel(presModel), m_view(view),
		  m_canvasView(canvasView), m_rmu(nullptr)
	{
		m_presModel->SetListener(this);

		// *************************

		m_view->SetRect(m_presModel->GetRect());
		m_canvasView->Invalidate();
	}

	std::shared_ptr<pres_model::IShape> CShapePresenter::GetModel() const
	{
		return m_presModel;
	}

	std::shared_ptr<view::IShapeView> CShapePresenter::GetView() const
	{
		return m_view;
	}

	void CShapePresenter::SetResizeMarkerUpdater(IResizeMarkerUpdater* rmu)
	{
		m_rmu = rmu;

		if (m_rmu)
			m_rmu->UpdateResizeMarkers(m_presModel->GetRect());
	}

	// implements pres_model::IShapeListener

	void CShapePresenter::OnRectChanged(const CRect& newRect)
	{
		m_view->SetRect(newRect);
		m_canvasView->Invalidate();

		if (m_rmu)
			m_rmu->UpdateResizeMarkers(newRect);
	}

}

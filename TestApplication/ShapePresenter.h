// Copyright (C) The Company

#pragma once

#include "IShape.h"
#include "IShapeView.h"
#include "ICanvasView.h"
#include "IResizeMarkerUpdater.h"

namespace presenter
{

	class CShapePresenter : public pres_model::IShapeListener
	{
	public:
		CShapePresenter(std::shared_ptr<pres_model::IShape> presModel,
			std::shared_ptr<view::IShapeView> view,
			view::ICanvasView* canvasView);

		std::shared_ptr<pres_model::IShape> GetModel() const;
		std::shared_ptr<view::IShapeView> GetView() const;
		void SetResizeMarkerUpdater(IResizeMarkerUpdater* rmu);

	private:

		// implements pres_model::IShapeListener

		virtual void OnRectChanged(const CRect& newRect) override;

	private:
		std::shared_ptr<pres_model::IShape> m_presModel;
		std::shared_ptr<view::IShapeView> m_view;
		view::ICanvasView* m_canvasView; // to invalidate on change
		IResizeMarkerUpdater* m_rmu;
	};

}
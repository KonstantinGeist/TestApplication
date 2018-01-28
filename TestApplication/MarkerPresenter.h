// Copyright (C) The Company

#pragma once

#include "IMarker.h"
#include "IMarkerView.h"
#include "ICanvasView.h"

namespace presenter
{

	class CMarkerPresenter : public pres_model::IMarkerListener
	{
	public:
		CMarkerPresenter(
			std::shared_ptr<pres_model::IMarker> presModel,
			std::shared_ptr<view::IMarkerView> view,
			view::ICanvasView* canvasView);

		std::shared_ptr<view::IMarkerView> GetView() const;

	private:

		// Implements ApplicationModel.IMarkerListener

		virtual void OnRectChanged(const CRect& newRect) override;

	private:
		std::shared_ptr<pres_model::IMarker> m_presModel;
		std::shared_ptr<view::IMarkerView> m_view;
		view::ICanvasView* m_canvasView; // to refresh on change TODO can be replaced with IInvalidatable?
	};
}

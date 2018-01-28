// Copyright (C) The Company

#pragma once

namespace presenter
{
	// Belongs to CCanvasPresenter, referenced by CShapePresenter's as a point of
	// communication.
	// TODO feels dirty
	class IResizeMarkerUpdater
	{
	public:
		virtual void UpdateResizeMarkers(const CRect& rect) = 0;

		virtual ~IResizeMarkerUpdater() = default;
	};

}

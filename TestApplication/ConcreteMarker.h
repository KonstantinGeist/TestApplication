// Copyright (C) The Company

#pragma once

#include "IMarker.h"

namespace pres_model
{

	class CConcreteMarker final : public IMarker
	{
	public:
		CConcreteMarker(EMarkerKind kind, const CRect& rect);

		virtual EMarkerKind GetKind() const override;
		virtual CRect GetRect() const override;
		virtual void SetRect(const CRect& rect) override;
		virtual void SetListener(IMarkerListener* listener) override;

	private:
		EMarkerKind m_kind;
		CRect m_rect;
		IMarkerListener* m_listener;
	};

}

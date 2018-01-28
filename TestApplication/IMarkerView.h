// Copyright (C) The Company

#pragma once

namespace view
{

	class IMarkerView
	{
	public:
		// TODO marker attributes like color etc.

		virtual CRect GetRect() const = 0;
		virtual void SetRect(const CRect& rect) = 0;

		virtual ~IMarkerView() = default;
	};

}

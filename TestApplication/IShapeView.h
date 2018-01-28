// Copyright (C) The Company

#pragma once

namespace view
{

	class IShapeView
	{
	public:
		virtual std::string GetTypeName() const = 0;
		virtual void SetRect(const CRect& rect) = 0;
		virtual CRect GetRect() const = 0;

		virtual ~IShapeView() = default;
	};

}

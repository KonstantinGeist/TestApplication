// Copyright (C) The Company

#pragma once

#include "Shape.h"

namespace domain_model
{

	class CTriangle : public CShape
	{
	public:
		virtual std::string GetTypeName() const override;
		virtual bool ContainsPoint(const CPoint& p) const override;
	};

}

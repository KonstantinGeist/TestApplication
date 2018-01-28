// Copyright (C) The Company

#pragma once

#include "Shape.h"

namespace domain_model
{

	class CRectangle : public CShape
	{
	public:
		virtual std::string GetTypeName() const override
		{
			return "Rectangle";
		}
	};

}

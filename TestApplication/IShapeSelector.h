// Copyright (C) The Company

#pragma once

#include "Shape.h"

namespace pres_model
{

	class IShapeSelector
	{
	public:
		virtual void SelectShape(std::shared_ptr<domain_model::CShape> shape) = 0;

		virtual ~IShapeSelector() = default;
	};

}

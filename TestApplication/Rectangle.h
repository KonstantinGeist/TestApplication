// Copyright (C) The Company

#pragma once

#include "Shape.h"

class CRectangle final : public CConvexShape
{
public:
	virtual const std::string GetTypeName() const override
	{
		return "Rectangle";
	}
};

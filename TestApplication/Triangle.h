// Copyright (C) The Company

#pragma once

#include "Shape.h"

class CTriangle : public CConvexShape
{
public:
	virtual const std::string GetTypeName() const override
	{
		return "Triangle";
	}
};

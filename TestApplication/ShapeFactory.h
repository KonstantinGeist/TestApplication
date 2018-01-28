// Copyright (C) The Company

#pragma once

namespace domain_model
{
	class CShape;

	class CShapeFactory
	{
	public:
		static CShape* CreateInstance(const std::string& typeName);
	};

}

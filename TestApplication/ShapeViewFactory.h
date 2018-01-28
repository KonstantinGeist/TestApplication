// Copyright (C) The Company

#pragma once

namespace view
{
	class IShapeView;

	class CShapeViewFactory
	{
	public:
		static IShapeView* CreateInstance(const std::string& typeName);
	};

}

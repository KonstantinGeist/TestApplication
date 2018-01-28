// Copyright (C) The Company

#pragma once

namespace view
{
	class CConcreteShapeViewBase;

	// NOTE Logically belongs to CTestApplicationView
	class CShapeViewFactory
	{
	public:
		static CConcreteShapeViewBase* CreateInstance(const std::string& typeName);
	};

}

// Copyright (C) The Company

#pragma once

#include "IShapeView.h"

namespace view
{

	// Used to hide the ::Render(..) method from public interfaces, only
	// CTestApplicationView knows about it.
	class CConcreteShapeViewBase : public IShapeView
	{
	public:
		virtual void Render(CDC* pDC) = 0;
	};

}

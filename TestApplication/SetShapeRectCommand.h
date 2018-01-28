// Copyright (C) The Company

#pragma once

#include "AbstractCommand.h"
#include "IShape.h"
#include "Shape.h"
#include "IShapeSelector.h"

namespace pres_model
{

	class CSetShapeRectCommand : public CAbstractCommand
	{
	public:
		CSetShapeRectCommand(
			std::shared_ptr<domain_model::CShape> shape,
			const CRect& prevRect,
			const CRect& nextRect,
			IShapeSelector* selector);

	protected:
		virtual void DoExecute() override;
		virtual void DoUnexecute() override;

	private:
		std::shared_ptr<domain_model::CShape> m_shape;
		CRect m_prevRect, m_nextRect;
		IShapeSelector* m_selector;
	};

}

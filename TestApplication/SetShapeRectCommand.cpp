// Copyright (C) The Company

#include "stdafx.h"
#include "SetShapeRectCommand.h"

namespace pres_model
{

	CSetShapeRectCommand::CSetShapeRectCommand(
		std::shared_ptr<domain_model::CShape> shape,
		const CRect& prevRect,
		const CRect& nextRect,
		IShapeSelector* selector)
		: m_shape(shape),
		  m_prevRect(prevRect),
		  m_nextRect(nextRect),
		  m_selector(selector)
	{
	}

	void CSetShapeRectCommand::DoExecute()
	{
		m_selector->SelectShape(m_shape);
		m_shape->SetRect(m_nextRect);
	}

	void CSetShapeRectCommand::DoUnexecute()
	{
		m_selector->SelectShape(m_shape);
		m_shape->SetRect(m_prevRect);
	}

}

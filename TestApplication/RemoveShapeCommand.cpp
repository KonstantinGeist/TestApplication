// Copyright (C) The Company

#include "stdafx.h"
#include "RemoveShapeCommand.h"

namespace pres_model
{

	CRemoveShapeCommand::CRemoveShapeCommand(
		std::shared_ptr<domain_model::CShape> shape,
		int index,
		IShapeSelector* selector,
		domain_model::IShapeCollection* shapes)
		: m_shape(shape),
		m_index(index),
		m_selector(selector),
		m_shapes(shapes)
	{
	}

	void CRemoveShapeCommand::DoExecute()
	{
		m_shapes->RemoveShape(m_shape);
	}

	void CRemoveShapeCommand::DoUnexecute()
	{
		m_shapes->InsertShape(m_shape, m_index);
		m_selector->SelectShape(m_shape);
	}

}

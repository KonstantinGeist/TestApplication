// Copyright (C) The Company

#include "stdafx.h"
#include "AddShapeCommand.h"

namespace pres_model
{

	CAddShapeCommand::CAddShapeCommand(
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

	void CAddShapeCommand::DoExecute()
	{
		m_shapes->InsertShape(m_shape, m_index);
		m_selector->SelectShape(m_shape);
	}

	void CAddShapeCommand::DoUnexecute()
	{
		m_shapes->RemoveShape(m_shape);
	}

}

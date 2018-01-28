// Copyright (C) The Company

#pragma once

#include "AbstractCommand.h"
#include "IShape.h"
#include "Shape.h"
#include "IShapeSelector.h"

namespace pres_model
{

	class CAddShapeCommand : public CAbstractCommand
	{
	public:
		CAddShapeCommand(std::shared_ptr<domain_model::CShape> shape,
			int index,
			IShapeSelector* selector,
			domain_model::IShapeCollection* shapes);

	protected:
		virtual void DoExecute() override;
		virtual void DoUnexecute() override;

	private:
		std::shared_ptr<domain_model::CShape> m_shape;
		int m_index;
		IShapeSelector* m_selector;
		domain_model::IShapeCollection* m_shapes;
	};

}

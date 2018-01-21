// Copyright (C) The Company

#include "stdafx.h"

#include "DocumentModel.h"

void CDocumentModel::Clear()
{
	m_shapes.clear();
}

void CDocumentModel::AddShape(std::shared_ptr<IShape> shape)
{
	assert(shape);

	m_shapes.push_back(shape);
}

void CDocumentModel::RemoveShape(std::shared_ptr<IShape> shape)
{
	auto it = std::find(m_shapes.begin(), m_shapes.end(), shape);
	assert(it != m_shapes.end());
	m_shapes.erase(it);
}

const std::vector<std::shared_ptr<IShape>>& CDocumentModel::GetShapes() const
{
	return m_shapes;
}

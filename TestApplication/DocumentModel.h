// Copyright (C) The Company

#pragma once

#include "Shape.h"

// Separates document model data (list of shapes).
class CDocumentModel final
{
public:
	void Clear();

	void AddShape(std::shared_ptr<IShape> shape);
	void RemoveShape(std::shared_ptr<IShape> shape);

	// Returns the full list of shapes. Useful for the presenter.
	const std::vector<std::shared_ptr<IShape>>& GetShapes() const;

private:
	std::vector<std::shared_ptr<IShape>> m_shapes;
};

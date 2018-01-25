// Copyright (C) The Company

#pragma once

#include "Shape.h"

class IDocumentModel
{
public:
	virtual ~IDocumentModel() = default;

	virtual void Clear() = 0;
	virtual void AddShape(std::shared_ptr<IShape> shape) = 0;
	virtual void RemoveShape(std::shared_ptr<IShape> shape) = 0;
	virtual const std::vector<std::shared_ptr<IShape>>& GetShapes() const = 0;
};

// Separates document model data (list of shapes).
class CDocumentModel final: public IDocumentModel
{
public:
	virtual void Clear() override;
	virtual void AddShape(std::shared_ptr<IShape> shape) override;
	virtual void RemoveShape(std::shared_ptr<IShape> shape) override;
	virtual const std::vector<std::shared_ptr<IShape>>& GetShapes() const override;

private:
	std::vector<std::shared_ptr<IShape>> m_shapes;
};

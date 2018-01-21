// Copyright (C) The Company

#pragma once

class IShape;
class IShapeView;

namespace tinyxml2
{
	class XMLDocument;
	class XMLElement;
}

// Maps shapes to shape views and vice versa.
class IShapeMapper
{
public:
	// The type name we're working with.
	virtual const std::string GetTypeName() const = 0;
	virtual IShape* ConvertViewToModel(IShapeView* shapeView) const = 0;
	virtual IShapeView* ConvertModelToView(IShape* shape) const = 0;
	virtual tinyxml2::XMLElement* ConvertModelToXMLElement(IShape* shape, tinyxml2::XMLDocument& doc) const = 0;
	virtual IShape* ConvertXMLElementToModel(tinyxml2::XMLElement* node) const = 0;
};

// Built-in shape mappers.

class CRectangleMapper : public IShapeMapper
{
public:
	virtual const std::string GetTypeName() const override;
	virtual IShape* ConvertViewToModel(IShapeView* shapeView) const override;
	virtual IShapeView* ConvertModelToView(IShape* shape) const override;
	virtual tinyxml2::XMLElement* ConvertModelToXMLElement(IShape* shape, tinyxml2::XMLDocument& doc) const override;
	virtual IShape* ConvertXMLElementToModel(tinyxml2::XMLElement* node) const override;
};

class CTriangleMapper : public IShapeMapper
{
public:
	virtual const std::string GetTypeName() const override;
	virtual IShape* ConvertViewToModel(IShapeView* shapeView) const override;
	virtual IShapeView* ConvertModelToView(IShape* shape) const override;
	virtual tinyxml2::XMLElement* ConvertModelToXMLElement(IShape* shape, tinyxml2::XMLDocument& doc) const override;
	virtual IShape* ConvertXMLElementToModel(tinyxml2::XMLElement* node) const override;
};

class CEllipseMapper : public IShapeMapper
{
public:
	virtual const std::string GetTypeName() const override;
	virtual IShape* ConvertViewToModel(IShapeView* shapeView) const override;
	virtual IShapeView* ConvertModelToView(IShape* shape) const override;
	virtual tinyxml2::XMLElement* ConvertModelToXMLElement(IShape* shape, tinyxml2::XMLDocument& doc) const override;
	virtual IShape* ConvertXMLElementToModel(tinyxml2::XMLElement* node) const override;
};

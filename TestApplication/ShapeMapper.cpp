// Copyright (C) The Company

#include "stdafx.h"

#include "ShapeMapper.h"
#include "Rectangle.h"
#include "RectangleView.h"
#include "Triangle.h"
#include "TriangleView.h"
#include "Ellipse.h"
#include "EllipseView.h"

#include "tinyxml2.h"

static void setCommonAttributes(tinyxml2::XMLElement* r, const CRect& rect)
{
	r->SetAttribute("X", rect.left);
	r->SetAttribute("Y", rect.top);
	r->SetAttribute("Width", rect.right - rect.left);
	r->SetAttribute("Height", rect.bottom - rect.top);
}

static void getCommonAttributes(tinyxml2::XMLElement* r, CRect* out_rect)
{
	int x = 0, y = 0, width = 0, height = 0;

	r->QueryIntAttribute("X", &x);
	r->QueryIntAttribute("Y", &y);
	r->QueryIntAttribute("Width", &width);
	r->QueryIntAttribute("Height", &height);

	out_rect->left = x;
	out_rect->top = y;
	out_rect->right = x + width;
	out_rect->bottom = y + height;
}

// ********************
//   CRectangleMapper
// ********************

const std::string CRectangleMapper::GetTypeName() const
{
	return "Rectangle";
}

IShape* CRectangleMapper::ConvertViewToModel(IShapeView* shapeView) const
{
	auto rectangleView = dynamic_cast<CRectangleView*>(shapeView);
	assert(rectangleView);

	auto r = new CRectangle();
	r->Rect = rectangleView->GetRect();
	return r;
}

IShapeView* CRectangleMapper::ConvertModelToView(IShape* shape) const
{
	auto rectangle = dynamic_cast<CRectangle*>(shape);
	assert(rectangle);

	auto r = new CRectangleView();
	r->SetRect(rectangle->Rect);
	return r;
}

tinyxml2::XMLElement* CRectangleMapper::ConvertModelToXMLElement(IShape* shape, tinyxml2::XMLDocument& doc) const
{
	// TODO copypaste
	auto rectangle = dynamic_cast<CRectangle*>(shape);
	assert(rectangle);

	auto r = doc.NewElement("Rectangle");
	setCommonAttributes(r, rectangle->Rect);
	return r;
}

IShape* CRectangleMapper::ConvertXMLElementToModel(tinyxml2::XMLElement* node) const
{
	auto r = new CRectangle();
	getCommonAttributes(node, &r->Rect);
	return r;
}

// *******************
//   CTriangleMapper
// *******************

const std::string CTriangleMapper::GetTypeName() const
{
	return "Triangle";
}

IShape* CTriangleMapper::ConvertViewToModel(IShapeView* shapeView) const
{
	auto triangleView = dynamic_cast<CTriangleView*>(shapeView);
	assert(triangleView);

	auto r = new CTriangle();
	r->Rect = triangleView->GetRect();
	return r;
}

IShapeView* CTriangleMapper::ConvertModelToView(IShape* shape) const
{
	auto triangle = dynamic_cast<CTriangle*>(shape);
	assert(triangle);

	auto r = new CTriangleView();
	r->SetRect(triangle->Rect);
	return r;
}

tinyxml2::XMLElement* CTriangleMapper::ConvertModelToXMLElement(IShape* shape, tinyxml2::XMLDocument& doc) const
{
	// TODO copypaste
	auto triangle = dynamic_cast<CTriangle*>(shape);
	assert(triangle);

	auto r = doc.NewElement("Triangle");
	setCommonAttributes(r, triangle->Rect);
	return r;
}

IShape* CTriangleMapper::ConvertXMLElementToModel(tinyxml2::XMLElement* node) const
{
	auto r = new CTriangle();
	getCommonAttributes(node, &r->Rect);
	return r;
}

// ******************
//   CEllipseMapper
// ******************

const std::string CEllipseMapper::GetTypeName() const
{
	return "Ellipse";
}

IShape* CEllipseMapper::ConvertViewToModel(IShapeView* shapeView) const
{
	auto ellipseView = dynamic_cast<CEllipseView*>(shapeView);
	assert(ellipseView);

	auto r = new CEllipse();
	r->Rect = ellipseView->GetRect();
	return r;
}

IShapeView* CEllipseMapper::ConvertModelToView(IShape* shape) const
{
	auto ellipse = dynamic_cast<CEllipse*>(shape);
	assert(ellipse);

	auto r = new CEllipseView();
	r->SetRect(ellipse->Rect);
	return r;
}

tinyxml2::XMLElement* CEllipseMapper::ConvertModelToXMLElement(IShape* shape, tinyxml2::XMLDocument& doc) const
{
	// TODO copypaste
	auto ellipse = dynamic_cast<CEllipse*>(shape);
	assert(ellipse);

	auto r = doc.NewElement("Ellipse");
	setCommonAttributes(r, ellipse->Rect);
	return r;
}

IShape* CEllipseMapper::ConvertXMLElementToModel(tinyxml2::XMLElement* node) const
{
	auto r = new CEllipse();
	getCommonAttributes(node, &r->Rect);
	return r;
}

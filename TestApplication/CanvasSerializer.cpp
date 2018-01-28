// Copyright (C) The Company

#include "stdafx.h"
#include "CanvasSerializer.h"
#include "Rectangle.h"
#include "Triangle.h"
#include "Ellipse.h"

namespace domain_model
{
	// WARNING this class contains copypaste

	// *********************************
	//              Helpers
	// *********************************

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

	class CRectangleSerializer final : public IShapeSerializer
	{
	public:
		virtual const std::string GetTypeName() const override
		{
			return "Rectangle";
		}

		virtual tinyxml2::XMLElement* ConvertModelToXMLElement(domain_model::CShape* shape, tinyxml2::XMLDocument& doc) const override
		{
			auto rectangle = dynamic_cast<domain_model::CRectangle*>(shape);
			assert(rectangle);

			auto r = doc.NewElement("Rectangle");
			setCommonAttributes(r, rectangle->GetRect());
			return r;
		}

		virtual domain_model::CShape* ConvertXMLElementToModel(tinyxml2::XMLElement* node) const override
		{
			auto r = new CRectangle();
			CRect rect;
			getCommonAttributes(node, &rect);
			r->SetRect(rect);
			return r;
		}
	};

	class CTriangleSerializer final : public IShapeSerializer
	{
	public:
		virtual const std::string GetTypeName() const override
		{
			return "Triangle";
		}

		virtual tinyxml2::XMLElement* ConvertModelToXMLElement(domain_model::CShape* shape, tinyxml2::XMLDocument& doc) const override
		{
			auto triangle = dynamic_cast<domain_model::CTriangle*>(shape);
			assert(triangle);

			auto r = doc.NewElement("Triangle");
			setCommonAttributes(r, triangle->GetRect());
			return r;
		}

		virtual domain_model::CShape* ConvertXMLElementToModel(tinyxml2::XMLElement* node) const override
		{
			auto r = new CTriangle();
			CRect rect;
			getCommonAttributes(node, &rect);
			r->SetRect(rect);
			return r;
		}
	};

	class CEllipseSerializer final : public IShapeSerializer
	{
	public:
		virtual const std::string GetTypeName() const override
		{
			return "Ellipse";
		}

		virtual tinyxml2::XMLElement* ConvertModelToXMLElement(domain_model::CShape* shape, tinyxml2::XMLDocument& doc) const override
		{
			auto ellipse = dynamic_cast<domain_model::CEllipse*>(shape);
			assert(ellipse);

			auto r = doc.NewElement("Ellipse");
			setCommonAttributes(r, ellipse->GetRect());
			return r;
		}

		virtual domain_model::CShape* ConvertXMLElementToModel(tinyxml2::XMLElement* node) const override
		{
			auto r = new CEllipse();
			CRect rect;
			getCommonAttributes(node, &rect);
			r->SetRect(rect);
			return r;
		}
	};

	// *********************************
	//    CShapeSerializerCollection
	// *********************************

	CShapeSerializerCollection::CShapeSerializerCollection()
	{
		// Registers built-in mappers.
		Register(std::make_unique<CRectangleSerializer>());
		Register(std::make_unique<CTriangleSerializer>());
		Register(std::make_unique<CEllipseSerializer>());
	}

	void CShapeSerializerCollection::Register(std::unique_ptr<IShapeSerializer> mapper)
	{
		m_serializers[mapper->GetTypeName()] = std::move(mapper);
	}

	IShapeSerializer* CShapeSerializerCollection::GetSerializer(const std::string& name) const
	{
		return m_serializers.at(name).get();
	}

	CShapeSerializerCollection& CShapeSerializerCollection::GetInstance()
	{
		static CShapeSerializerCollection g_instance;

		return g_instance;
	}

	// *********************************
	//         SerializeCanvas
	// *********************************

	bool SerializeCanvas(CCanvas* canvas, const std::string& path)
	{
		tinyxml2::XMLDocument xmlDoc;
		tinyxml2::XMLNode* shapesNode = xmlDoc.NewElement("Shapes");
		xmlDoc.InsertFirstChild(shapesNode);

		auto& serializerCollection = CShapeSerializerCollection::GetInstance();

		int count = canvas->GetShapeCount();
		for(int i = 0; i < count; i++)
		{
			auto shape = canvas->GetShape(i);

			std::string typeName = shape->GetTypeName();
			auto serializer = serializerCollection.GetSerializer(typeName);

			tinyxml2::XMLElement* element = serializer->ConvertModelToXMLElement(shape.get(), xmlDoc);
			assert(element);

			shapesNode->InsertEndChild(element);
		}

		tinyxml2::XMLError eResult = xmlDoc.SaveFile(path.c_str());
		if (eResult != tinyxml2::XML_SUCCESS)
			return false;

		return true;
	}

	// *********************************
	//        DeserializeCanvas
	// *********************************

	bool DeserializeCanvas(CCanvas* canvas, const std::string& path)
	{
		tinyxml2::XMLDocument xmlDoc;
		tinyxml2::XMLError eResult = xmlDoc.LoadFile(path.c_str());
		if (eResult != tinyxml2::XML_SUCCESS)
			return false;

		tinyxml2::XMLNode* shapesNode = xmlDoc.FirstChild();
		if (!shapesNode)
			return false;

		canvas->Clear();

		auto& serializerCollection = CShapeSerializerCollection::GetInstance();

		for (auto e = shapesNode->FirstChildElement();
			e != nullptr;
			e = e->NextSiblingElement())
		{
			std::string typeName = e->Name();
			auto serializer = serializerCollection.GetSerializer(typeName);

			auto shape = std::shared_ptr<CShape>(serializer->ConvertXMLElementToModel(e));
			assert(shape);

			int index = canvas->GetShapeCount();
			canvas->InsertShape(shape, index);
		}

		return true;
	}

}

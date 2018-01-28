// Copyright (C) The Company

#pragma once

#include "tinyxml2.h"
#include "Shape.h"
#include "Canvas.h"

namespace domain_model
{

	class IShapeSerializer
	{
	public:
		// The type name we're working with.
		virtual const std::string GetTypeName() const = 0;

		virtual tinyxml2::XMLElement* ConvertModelToXMLElement(domain_model::CShape* shape, tinyxml2::XMLDocument& doc) const = 0;
		virtual domain_model::CShape* ConvertXMLElementToModel(tinyxml2::XMLElement* node) const = 0;

		virtual ~IShapeSerializer() = default;
	};

	// CShapeSerializerCollection
	class CShapeSerializerCollection final
	{
	private:
		CShapeSerializerCollection();

	public:
		static CShapeSerializerCollection& GetInstance();

		void Register(std::unique_ptr<IShapeSerializer> serializer);
		IShapeSerializer* GetSerializer(const std::string& name) const;

	private:
		std::unordered_map<std::string, std::unique_ptr<IShapeSerializer>> m_serializers;
	};

	bool SerializeCanvas(CCanvas* canvas, const std::string& path);
	bool DeserializeCanvas(CCanvas* canvas, const std::string& path);

}

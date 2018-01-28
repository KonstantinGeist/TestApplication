// Copyright (C) The Company

#pragma once

#include "IShape.h"
#include "Shape.h"
#include "IShapeSelector.h"
#include "History.h"

namespace pres_model
{

	class CShapePresentation : public IShape, domain_model::IShapeListener
	{
	public:
		CShapePresentation(
			std::shared_ptr<domain_model::CShape> domainModel,
			CHistory* history,
			IShapeSelector* selector);

		std::shared_ptr<domain_model::CShape> GetDomainModel() const;

		// Implements pres_model::IShape

		virtual std::string GetTypeName() const override;
		virtual CRect GetRect() const override;
		virtual void SetRect(const CRect& rect) override;
		virtual void CommitRect(const CRect& prevRect) override;
		virtual bool ContainsPoint(const CPoint& p) const override;
		virtual void SetListener(pres_model::IShapeListener* listener) override;

	private:
		// Implements domain_model::IShapeListener

		virtual void OnRectChanged(const CRect& newRect) override;

	private:
		std::shared_ptr<domain_model::CShape> m_domainModel;
		CHistory* m_history; // for commit=true TODO is this OK?
		IShapeSelector* m_selector;
		pres_model::IShapeListener* m_listener;
	};

}

// Copyright (C) The Company

#include "stdafx.h"
#include "ShapePresentation.h"
#include "SetShapeRectCommand.h"

namespace pres_model
{

	CShapePresentation::CShapePresentation(
		std::shared_ptr<domain_model::CShape> domainModel,
		CHistory* history,
		IShapeSelector* selector)
		: m_domainModel(domainModel),
		  m_history(history),
		  m_selector(selector),
		  m_listener(nullptr)
	{
		m_domainModel->SetListener(this);
	}

	std::shared_ptr<domain_model::CShape> CShapePresentation::GetDomainModel() const
	{
		return m_domainModel;
	}

	// Implements IShape

	std::string CShapePresentation::GetTypeName() const
	{
		return m_domainModel->GetTypeName();
	}

	CRect CShapePresentation::GetRect() const
	{
		return m_domainModel->GetRect();
	}

	void CShapePresentation::SetRect(const CRect& rect)
	{
		m_domainModel->SetRect(rect);
	}

	void CShapePresentation::CommitRect(const CRect& prevRect)
	{
		auto nextRect = m_domainModel->GetRect();

		// Do not commit if rects are same.
		if (nextRect == prevRect)
			return;

		auto moveCommand = std::make_unique<CSetShapeRectCommand>(
			m_domainModel,
			prevRect,
			nextRect,
			m_selector);

		m_history->AddAndExecute(std::move(moveCommand));
	}

	bool CShapePresentation::ContainsPoint(const CPoint& p) const
	{
		return m_domainModel->ContainsPoint(p);
	}

	void CShapePresentation::SetListener(pres_model::IShapeListener* listener)
	{
		m_listener = listener;
	}

	// Implements domain_model::IShapeListener

	void CShapePresentation::OnRectChanged(const CRect& newRect)
	{
		if (m_listener)
			m_listener->OnRectChanged(newRect);
	}

}

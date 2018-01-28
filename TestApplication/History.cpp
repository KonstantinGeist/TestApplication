// Copyright (C) The Company

#include "stdafx.h"
#include "History.h"

namespace pres_model
{

	CHistory::CHistory()
		: m_listener(nullptr)
	{
	}

	bool CHistory::CanUndo() const
	{
		return !m_toUndo.empty();
	}

	bool CHistory::CanRedo() const
	{
		return !m_toRedo.empty();
	}

	void CHistory::AddAndExecute(std::unique_ptr<ICommand> command)
	{
		command->Execute(); // TODO may throw

		if (m_listener)
			m_listener->OnExecute(command.get());

		m_toUndo.push_back(std::move(command));
		m_toRedo.clear(); // NOTE _doing_ something destroys our Redo queue
	}

	void CHistory::Undo()
	{
		if (!CanUndo())
			return;

		auto command = std::move(m_toUndo.back());
		m_toUndo.pop_back();

		command->Unexecute(); // TODO may throw

		if (m_listener)
			m_listener->OnUnexecute(command.get());

		m_toRedo.push_back(std::move(command));
	}

	void CHistory::Redo()
	{
		if (!CanRedo())
			return;

		auto command = std::move(m_toRedo.back());
		m_toRedo.pop_back();

		command->Execute(); // TODO may throw

		if (m_listener)
			m_listener->OnExecute(command.get());

		m_toUndo.push_back(std::move(command));
	}

	void CHistory::Clear()
	{
		m_toUndo.clear();
		m_toRedo.clear();
	}

	void CHistory::SetListener(IHistoryListener* listener)
	{
		m_listener = listener;
	}

}

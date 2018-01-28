// Copyright (C) The Company

#pragma once

#include "Command.h"

namespace pres_model
{
	class IHistoryListener
	{
	public:
		virtual void OnExecute(ICommand* newCommand) = 0;
		virtual void OnUnexecute(ICommand* newCommand) = 0;

		virtual ~IHistoryListener() = default;
	};

	class IHistory
	{
	public:
		virtual bool CanUndo() const = 0;
		virtual bool CanRedo() const = 0;
		virtual void Undo() = 0;
		virtual void Redo() = 0;
		virtual void Clear() = 0;
		virtual void SetListener(IHistoryListener* listener) = 0;

		virtual ~IHistory() = default;
	};

	class CHistory final: public IHistory
	{
	public:
		CHistory();

		virtual bool CanUndo() const override;
		virtual bool CanRedo() const override;

		void AddAndExecute(std::unique_ptr<ICommand> command);

		virtual void Undo() override;
		virtual void Redo() override;

		virtual void Clear() override;

		virtual void SetListener(IHistoryListener* listener) override;

	private:
		std::list<std::unique_ptr<ICommand>> m_toUndo, m_toRedo;
		IHistoryListener* m_listener;
	};

}

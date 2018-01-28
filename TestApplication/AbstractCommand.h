// Copyright (C) The Company

#pragma once

#include "Command.h"

namespace pres_model
{

	class CAbstractCommand : public ICommand
	{
	public:
		CAbstractCommand();

		virtual void Execute() override;
		virtual void Unexecute() override;

	protected:
		virtual void DoExecute() = 0;
		virtual void DoUnexecute() = 0;

	private:
		bool m_executed;
	};

}

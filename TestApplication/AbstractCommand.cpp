// Copyright (C) The Company

#include "stdafx.h"
#include "AbstractCommand.h"

namespace pres_model
{

	CAbstractCommand::CAbstractCommand()
		: m_executed(false)
	{
	}

	void CAbstractCommand::Execute()
	{
		if (!m_executed)
		{
			DoExecute();
			m_executed = true;
		}
	}

	void CAbstractCommand::Unexecute()
	{
		if (m_executed)
		{
			DoUnexecute();
			m_executed = false;
		}
	}

}

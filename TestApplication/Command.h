// Copyright (C) The Company

#pragma once

namespace pres_model
{

	class ICommand
	{
	public:
		virtual void Execute() = 0;
		virtual void Unexecute() = 0;

		virtual ~ICommand() = default;
	};

}

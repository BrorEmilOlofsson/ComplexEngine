#pragma once
#include "NodeScript/Fly.hpp"

namespace Simple
{
	class FlyReflectionMemoryWindow
	{

	public:

		void Update();

	private:

		Fly::GenericDataTypeProxy myCurrentDataType;

	};
}
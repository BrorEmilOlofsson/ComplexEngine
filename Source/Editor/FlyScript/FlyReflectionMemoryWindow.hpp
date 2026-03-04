#pragma once
#include "NodeScript/Fly.hpp"

namespace CLX
{
	class FlyReflectionMemoryWindow
	{

	public:

		void Update();

	private:

		Fly::GenericDataTypeProxy myCurrentDataType;

	};
}
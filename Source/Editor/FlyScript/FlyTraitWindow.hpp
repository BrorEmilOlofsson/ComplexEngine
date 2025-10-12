#pragma once
#include "NodeScript/Fly.hpp"
#include "FlyScriptEditorUtilities.hpp"

namespace Simple
{
	class FlyTraitWindow final
	{
	public:

		FlyTraitWindow();

		void Update();

	private:

		Fly::GenericDataTypeProxy mSelectedDataType;
		Fly::TraitID mCurrentTraitID;

		NodeGraphContext mNodeGraphContext;
	};
}
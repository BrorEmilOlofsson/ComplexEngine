#pragma once
#include "NodeScript/Fly.hpp"
#include "FlyScriptEditorUtilities.hpp"

namespace CLX
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
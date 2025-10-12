#include "Editor/Precompiled/EditorPch.hpp"
#include "FlyTraitWindow.hpp"
#include "FlyScriptEditorUtilities.hpp"
#include "NodeScript/Internal/FlyInternal.hpp"
#include "NodeScript/DataType/FlyDataTypeManager.hpp"
#include "NodeScript/Trait/FlyTraitManager.hpp"

namespace Simple
{

	FlyTraitWindow::FlyTraitWindow()
	{
	}

	void FlyTraitWindow::Update()
	{
		if (ImGui::Begin("Trait Window"))
		{
			std::string traitName = "None";

			if (mCurrentTraitID != Fly::InvalidID<Fly::TraitID>())
			{
				traitName = Fly::Internal::GetTraitByID(mCurrentTraitID).mName;
			}

			if (ImGui::BeginCombo("Selected Trait", traitName.c_str()))
			{
				const auto& traits = Fly::Internal::GetTraitManager().GetTraits();

				for (Fly::TraitID traitID{ 0 }; traitID < traits.size(); traitID++)
				{
					if (ImGui::Selectable(Fly::Internal::GetTraitManager().GetTrait(traitID).mName.c_str()))
					{
						mCurrentTraitID = traitID;
					}
				}

				ImGui::EndCombo();
			}



			if (DataTypeComboNoFilter("Data Type", mSelectedDataType))
			{

			}

			const bool validTrait = mCurrentTraitID != Fly::InvalidID<Fly::TraitID>() && mSelectedDataType;
			bool traitExists = false;
			if (validTrait)
			{
				traitExists = Fly::Internal::GetTraitManager().HasDataTypeTrait(std::get<Fly::DataTypeID>(mSelectedDataType.GetID().mID), mCurrentTraitID);
			}
			ImGui::BeginDisabled(!validTrait || traitExists);

			if (ImGui::Button("Implement Trait"))
			{
				Fly::Internal::CreateTraitImplementation(std::get<Fly::DataTypeID>(mSelectedDataType.GetID().mID), mCurrentTraitID);
			}

			ImGui::EndDisabled();

			if (validTrait)
			{

				if (Fly::Internal::GetTraitManager().HasDataTypeTrait(std::get<Fly::DataTypeID>(mSelectedDataType.GetID().mID), mCurrentTraitID))
				{

					Fly::TraitImplementation* t = Fly::Internal::GetTraitManager().GetTraitImplementation(std::get<Fly::DataTypeID>(mSelectedDataType.GetID().mID), mCurrentTraitID);

					if (Fly::FlyTraitImplementation* t2 = std::get_if<Fly::FlyTraitImplementation>(t))
					{
						mNodeGraphContext.mNodeGraph = Fly::NodeGraphProxy(t2->mEventGraph);
						ShowNodeGraph(mNodeGraphContext);
					}


				}
			}
		}
		ImGui::End();
	}
}
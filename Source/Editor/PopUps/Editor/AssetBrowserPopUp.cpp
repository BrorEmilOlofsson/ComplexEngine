#include "Editor/Precompiled/EditorPch.hpp"
#include "Editor/PopUps/Editor/AssetBrowserPopUp.hpp"
#include "Editor/Core/Tabs/MenuTabWindow.hpp"
#include "Utility/Win/WinConsole.hpp"
#include "Editor/FlyScript/NodeScriptingWindow.hpp"
#include "Utility/File/FileUtility.hpp"
#include "Utility/AssetPath.hpp"
#include "Engine/Scene/SceneManager.hpp"
#include "Utility/Asset/AssetManager.hpp"
#include "Engine/Utility/BlackboardKeys.hpp"
#include "Graphics/Texture/Texture.hpp"
#include "Utility/Algorithm.hpp"
#include "Editor/PopUps/Editor/EntityCompositionPopUp.hpp"
#include "Engine/ECS/EntityComposition.hpp"
#include "Engine/ECS/ECSSerializer.hpp"
#include "Graphics/GraphicsDefines.hpp"

namespace Simple
{

	static void* GetSRVFromIconType(const eIconType iconType, AssetManager& assetManager)
	{
		TextureAssetHandle handle = assetManager.GetTexture(GetPath(iconType));
		assert(handle);
		return handle->GetShaderResourceView();
	}

	static void* GetSRVFromFileExtension(const std::filesystem::path& extension, const std::filesystem::path& path, AssetManager& assetManager)
	{
		const std::optional<eIconType> iconType = ToIconType(extension.string());

		if (iconType)
		{
			return GetSRVFromIconType(*iconType, assetManager);
		}

		if (extension == ".dds")
		{
			TextureAssetHandle texture = assetManager.GetTexture(path);

			if (texture->GetSlot() != TextureSlots::CubeMap)
			{
				return texture->GetShaderResourceView();
			}
			else
			{
				return GetSRVFromIconType(eIconType::CubeMap, assetManager);
			}
		}

		return GetSRVFromIconType(eIconType::Unknown, assetManager);
	}

	static void OnAssetClicked(const std::filesystem::path& path, std::filesystem::path& currentDirectory, 
		NodeScriptingWindow& nodeScriptingWindow, MenuTabWindow& nodeScriptParentTab, MenuItemPopUp& nodeScriptButton,
		SceneManager& sceneManager, AssetManager& assetManager, EntityCompositionPopUp& entityCompositionPopUp, const ImTextureID textureID)
	{
		const std::string& extension = path.extension().string();
		if (std::filesystem::is_directory(path))
		{
			currentDirectory = path;
			return;
		}
		else if (textureID == GetSRVFromIconType(eIconType::Scene, assetManager))
		{
			const std::filesystem::path scenePath = std::filesystem::path("Assets/Scenes/") / path;
			sceneManager.ChangeScene(scenePath);
		}
		else if (extension == ".fly")
		{
			if (nodeScriptingWindow.OpenClassByName(path.stem().string()))
			{
				nodeScriptParentTab.ActivateWindow(&nodeScriptButton, true);
			}
		}
		else if (extension == ".ecomp")
		{
			EntityCompositionAssetHandle entityCompositionAsset = assetManager.GetEntityComposition(path);
			if (entityCompositionAsset.IsValid())
			{
				entityCompositionPopUp.SetCompositionAsset(entityCompositionAsset);
			}
		}
	}

	static void DeleteAsset(const std::filesystem::path& path, AssetManager& assetManager)
	{
		assetManager.Remove(path);
		
		if (std::filesystem::remove(path))
		{
			Console::Print("Removed ", ConsoleTextColor::White, false);
			Console::Print(path.string(), ConsoleTextColor::Green, true);
		}
	}

	static void ShowAssetCreationPopUp(const std::filesystem::path& directoryPath, bool& canOpenPopUp, AssetManager& assetManager, const DataTypeRegistry& dataTypeRegistry)
	{
		static constexpr const char* CreateAssetMenuPopupName = "CreateAssetMenu";
		static constexpr const char* CreateEntityCompositionAssetName = "CreateEntityCompositionAsset";

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered() && canOpenPopUp)
		{
			ImGui::OpenPopup(CreateAssetMenuPopupName);
		}

		bool hasOpened = false;

		if (ImGui::BeginPopup(CreateAssetMenuPopupName))
		{

			if (ImGui::BeginMainMenuBar())
			{
				ImGui::Text("Create Asset");
				ImGui::EndMainMenuBar();
			}
			if (ImGui::BeginMenu("Fly##Create"))
			{
				if (ImGui::MenuItem("Struct##CreateFlyStruct"))
				{
					Fly::CreateStruct("TestStruct", directoryPath);
				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Entity Composition"))
			{
				hasOpened = true;
			}

			canOpenPopUp = false;

			ImGui::EndPopup();
		}
		else
		{
			canOpenPopUp = true;
		}

		if (hasOpened)
		{
			ImGui::OpenPopup(CreateEntityCompositionAssetName);
		}

		if (ImGui::BeginPopup(CreateEntityCompositionAssetName))
		{
			if (ImGui::BeginMainMenuBar())
			{
				ImGui::Text("Create Entity Composition");
				ImGui::EndMainMenuBar();
			}

			static char name[32]{};
			ImGui::InputTextWithHint("##", "Name", name, 32);

			if (ImGui::Button("Create"))
			{
				EntityCompositionAsset asset(std::make_shared<EntityComposition>(ECSRegistry::Get()));
				const std::filesystem::path path = directoryPath / (std::string(name) + ".ecomp");
				asset->SetPath(path);
				auto assetHandle = assetManager.AddEntityCompositionAsset(path, asset);
				SaveEntityCompositionAsset(assetHandle, dataTypeRegistry);

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	struct DrawFilesInFolderData
	{
		SceneManager& sceneManager;
		AssetManager& assetManager;
		DataTypeRegistry& dataTypeRegistry;
		NodeScriptingWindow& nodeScriptingWindow;
		MenuTabWindow& nodeScriptParentTab;
		MenuItemPopUp& nodeScriptButton;
		EntityCompositionPopUp& entityCompositionPopUp;
		bool& canOpenPopup;
		std::string& filePopUpID;
		std::filesystem::path& fileToRemove;
		std::filesystem::path& currentDirectory;
	};

	static void DrawFilesInFolder(const std::filesystem::path& directory, const DrawFilesInFolderData& data)
	{
		PROFILER_FUNCTION(profiler::colors::Blue300);
		const std::vector<std::filesystem::path> paths = FileUtility::GetPathsFromDirectory(directory, true);

		static constexpr float padding = 16.0f;
		static constexpr float thumbnailSize = 64.0f;
		static constexpr float cellSize = thumbnailSize + padding;
		const float panelWidth = ImGui::GetContentRegionAvail().x;

		int columnCount = (int)(panelWidth / cellSize);

		if (columnCount < 1)
		{
			columnCount = 1;
		}

		ImGui::Columns(columnCount, 0, false);

		ImGui::PushStyleColor(ImGuiCol_Button, ImColor(0.12f, 0.12f, 0.12f, 0.0f).Value);

		for (const std::filesystem::path& path : paths)
		{
			const std::filesystem::path extension = path.extension();

			const ImTextureID textureID = GetSRVFromFileExtension(extension, path, data.assetManager);

			const std::string stem = path.stem().string();
			ImGui::ImageButton(stem.c_str(), textureID, { thumbnailSize, thumbnailSize });
			if (extension == ".obj")
			{
				static AssetPath_OBJ obj;
				std::string pathStr = path.string();
				obj = AssetPath_OBJ(FixedString<256>(pathStr.begin(), pathStr.end()));
				ObjectSource(obj, "OBJ");
			}
			else if (ImGui::BeginDragDropSource())
			{
				if (!std::filesystem::is_directory(path))
				{

					char buffer[256]{};
					strcpy_s(buffer, path.string().c_str());

					ImGui::SetDragDropPayload("Asset", buffer, sizeof(buffer));

					ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

					ImGui::BeginTooltip();
					ImGui::ImageButton(textureID, ImVec2(64.0f, 64.0f));
					ImGui::EndTooltip();

					ImGui::PopStyleVar();
				}

				ImGui::EndDragDropSource();
			}

			if (ImGui::IsItemHovered())
			{
				if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					OnAssetClicked(
						path,
						data.currentDirectory,
						data.nodeScriptingWindow,
						data.nodeScriptParentTab,
						data.nodeScriptButton,
						data.sceneManager,
						data.assetManager,
						data.entityCompositionPopUp,
						textureID
					);
				}
				else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && !std::filesystem::is_directory(path))
				{
					data.filePopUpID = "Delete##" + path.string();
					data.fileToRemove = directory / path;
					ImGui::OpenPopup(data.filePopUpID.c_str());
				}
			}

			ImGui::TextWrapped(path.stem().string().c_str());
			ImGui::NextColumn();
		}

		ImGui::Columns();

		ImGui::PopStyleColor();


		if (ImGui::BeginPopup(data.filePopUpID.c_str()))
		{
			if (ImGui::MenuItem("Delete##FileUtilityPopUp"))
			{
				DeleteAsset(data.fileToRemove, data.assetManager);
			}

			data.canOpenPopup = false;

			ImGui::EndPopup();
		}
		else
		{
			data.canOpenPopup = true;
		}

		ShowAssetCreationPopUp(directory, data.canOpenPopup, data.assetManager, data.dataTypeRegistry);
	}


	void ViewFolders(const std::filesystem::path& directory, const std::string& treeNodeName, std::filesystem::path& currentDirectory)
	{
		PROFILER_FUNCTION(profiler::colors::Yellow300);

		if (ImGui::TreeNode(treeNodeName.c_str()))
		{
			const std::vector<std::filesystem::path> fileNames = FileUtility::GetPathsFromDirectory(directory, true);

			for (const auto& name : fileNames)
			{
				if (std::filesystem::is_directory(name))
				{
					ViewFolders(directory / name, name.stem().string(), currentDirectory);
				}
				else
				{
					if (ImGui::Selectable(name.filename().string().c_str()))
					{
					}
				}
			}

			ImGui::TreePop();
		}
		else
		{
			if (ImGui::IsItemClicked())
			{
				currentDirectory = directory;
			}
		}
	}

	static void RenderAssetBrowserPopup(const DrawFilesInFolderData& data, const std::string& imGuiName)
	{
		if (ImGui::Begin(imGuiName.c_str()))
		{
			ImVec2 parentSize = ImGui::GetContentRegionAvail();
			parentSize.x *= 0.2f;

			if (ImGui::BeginChild("AssetPaths#", parentSize, ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeX))
			{
				ImGui::SetNextItemOpen(true);
				ViewFolders(std::filesystem::absolute(SIMPLE_DIR_ASSETS), SIMPLE_DIR_ASSETS, data.currentDirectory);
				ImGui::EndChild();
			}

			ImGui::SameLine();

			parentSize = ImGui::GetContentRegionAvail();

			if (ImGui::BeginChild("Test", parentSize, ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY))
			{
				ImGui::AlignTextToFramePadding();

				if (ImGui::ArrowButton("##Arrow_back", ImGuiDir_Left))
				{
					const std::string currentDirectoryStr = data.currentDirectory.string();
					const size_t lastBackSlashPos = currentDirectoryStr.find_last_of('\\');

					if (lastBackSlashPos != std::string::npos)
					{
						const std::string previousDirectory = currentDirectoryStr.substr(0, lastBackSlashPos);

						if (previousDirectory.find("Assets") != std::string::npos)
						{
							data.currentDirectory = previousDirectory;
						}
					}
				}

				ImGui::SameLine();
				ImGui::Text(data.currentDirectory.string().c_str());
				ImGui::Separator();


				DrawFilesInFolder(data.currentDirectory, data);

				ImGui::EndChild();
			}
		}

		ImGui::End();
	}

	AssetBrowserPopUp::AssetBrowserPopUp(const std::string& name, NodeScriptingWindow* nodeScriptWindow, MenuTabWindow* nodeScriptParentTab, MenuItemPopUp* nodeScriptButton)
		: PopUp(name)
		, mNodeScriptingWindow(nodeScriptWindow)
		, mNodeScriptParentTab(nodeScriptParentTab)
		, mNodeScriptButton(nodeScriptButton)
		, mCanOpenPopup(false)
	{
	}

	void AssetBrowserPopUp::Render(const Blackboard& blackboard)
	{
		SceneManager& sceneManager = blackboard.Get<Key_SceneManager>();
		AssetManager& assetManager = blackboard.Get<Key_AssetManager>();
		DataTypeRegistry& dataTypeRegistry = blackboard.Get<Key_DataTypeRegistry>();
		EntityCompositionPopUp& entityCompositionPopUp = blackboard.Get<Key_EntityCompositionPopUp>();
		const DrawFilesInFolderData data
		{
			sceneManager,
			assetManager,
			dataTypeRegistry,
			*mNodeScriptingWindow,
			*mNodeScriptParentTab,
			*mNodeScriptButton,
			entityCompositionPopUp,
			mCanOpenPopup,
			mFilePopUpID,
			mFileToRemove,
			mCurrentDirectory
		};
		RenderAssetBrowserPopup(data, GetImGuiName());
	}

}

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

namespace Simple
{

	static void* GetSRVFromIconType(const eIconType iconType, AssetManager& assetManager)
	{
		TextureAssetHandle handle = assetManager.GetTexture(GetPath(iconType));
		assert(handle);
		return handle->GetShaderResourceView();
	}

	static void* GetSRVFromFileExtension(const std::filesystem::path& extension, const std::filesystem::path& directory, const std::filesystem::path& fileName, AssetManager& assetManager)
	{
		const std::optional<eIconType> iconType = ToIconType(extension.string());

		if (iconType)
		{
			return GetSRVFromIconType(*iconType, assetManager);
		}

		if (extension == ".dds")
		{
			const std::filesystem::path texturePath = ConvertAbsolutePathToRelativePath(directory) / fileName;
			TextureAssetHandle texture = assetManager.GetTexture(texturePath);

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

	struct DrawFilesInFolderData
	{
		SceneManager& mSceneManager;
		AssetManager& mAssetManager;
		NodeScriptingWindow& mNodeScriptingWindow;
		MenuTabWindow& mNodeScriptParentTab;
		MenuItemPopUp& mNodeScriptButton;
		bool& mCanOpenPopup;
		std::string& mFilePopUpID;
		std::filesystem::path& mFileToRemove;
		std::filesystem::path& mCurrentDirectory;
	};

	static void DrawFilesInFolder(const std::filesystem::path& directory, const DrawFilesInFolderData& data)
	{
		PROFILER_FUNCTION(profiler::colors::Blue300);
		const std::vector<std::filesystem::path> fileNames = FileUtility::GetFilesFromDirectory(directory, true);

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

		for (size_t i = 0; i < fileNames.size(); ++i)
		{
			const std::filesystem::path extension = fileNames[i].extension();

			const ImTextureID textureID = GetSRVFromFileExtension(extension, directory, fileNames[i], data.mAssetManager);

			const std::string n = fileNames[i].stem().string();
			ImGui::ImageButton(n.c_str(), textureID, { thumbnailSize, thumbnailSize });
			if (extension == ".obj")
			{
				static AssetPath_OBJ obj;
				std::string pathStr = fileNames[i].string();
				std::string a(pathStr.begin(), pathStr.end());
				obj = AssetPath_OBJ(FixedString<256>(pathStr.begin(), pathStr.end()));
				ObjectSource(obj, "OBJ");
			}
			else if (ImGui::BeginDragDropSource())
			{
				if (extension.string()[0] == '.')
				{
					std::filesystem::path filePath = directory / fileNames[i];

					char buffer[256]{};
					strcpy_s(buffer, filePath.string().c_str());

					ImGui::SetDragDropPayload("Assets_Browser", buffer, sizeof(buffer));

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
					if (extension.string()[0] != '.')
					{
						data.mCurrentDirectory = directory / fileNames[i];
						break;
					}
					else if (textureID == GetSRVFromIconType(eIconType::Scene, data.mAssetManager))
					{
						const std::filesystem::path scenePath = std::filesystem::path("Assets/Scenes/") / fileNames[i]; //TO-DO(v11.2.3): Fix so it doesnt become hardcoded
						data.mSceneManager.ChangeScene(scenePath);
					}
					else if (extension == ".fly")
					{
						if (data.mNodeScriptingWindow.OpenClassByName(fileNames[i].stem().string()))
						{
							data.mNodeScriptParentTab.ActivateWindow(&data.mNodeScriptButton, true);
						}
					}
				}
				else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && extension.string()[0] == '.')
				{
					data.mFilePopUpID = "Delete##" + fileNames[i].string();
					data.mFileToRemove = directory / fileNames[i];
					ImGui::OpenPopup(data.mFilePopUpID.c_str());
				}
			}

			ImGui::TextWrapped(fileNames[i].stem().string().c_str());
			ImGui::NextColumn();
		}

		ImGui::Columns();

		ImGui::PopStyleColor();


		if (ImGui::BeginPopup(data.mFilePopUpID.c_str()))
		{
			if (ImGui::MenuItem("Delete##FileUtilityPopUp"))
			{
				if (std::filesystem::remove(data.mFileToRemove))
				{
					Console::Print("Removed ", ConsoleTextColor::White, false);
					Console::Print(data.mFileToRemove.string().c_str(), ConsoleTextColor::Green, true);
				}
			}

			data.mCanOpenPopup = false;

			ImGui::EndPopup();
		}
		else
		{
			data.mCanOpenPopup = true;
		}

		static constexpr const char* CreateAssetMenuPopupName = "CreateAssetMenu";

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered() && data.mCanOpenPopup)
		{
			ImGui::OpenPopup(CreateAssetMenuPopupName);
		}

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
					Fly::CreateStruct("TestStruct", directory.string());
				}

				ImGui::EndMenu();
			}

			data.mCanOpenPopup = false;

			ImGui::EndPopup();
		}
		else
		{
			data.mCanOpenPopup = true;
		}
	}


	void ViewFolders(const std::filesystem::path& directory, const std::string& treeNodeName, std::filesystem::path& currentDirectory)
	{
		PROFILER_FUNCTION(profiler::colors::Yellow300);

		if (ImGui::TreeNode(treeNodeName.c_str()))
		{
			const std::vector<std::filesystem::path> fileNames = FileUtility::GetFilesFromDirectory(directory, true);

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
				ViewFolders(std::filesystem::absolute(SIMPLE_DIR_ASSETS), SIMPLE_DIR_ASSETS, data.mCurrentDirectory);
				ImGui::EndChild();
			}

			ImGui::SameLine();

			parentSize = ImGui::GetContentRegionAvail();

			if (ImGui::BeginChild("Test", parentSize, ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY))
			{
				ImGui::AlignTextToFramePadding();

				if (ImGui::ArrowButton("##Arrow_back", ImGuiDir_Left))
				{
					const std::string currentDirectoryStr = data.mCurrentDirectory.string();
					const size_t lastBackSlashPos = currentDirectoryStr.find_last_of('\\');

					if (lastBackSlashPos != std::string::npos)
					{
						const std::string previousDirectory = currentDirectoryStr.substr(0, lastBackSlashPos);

						if (previousDirectory.find("Assets") != std::string::npos)
						{
							data.mCurrentDirectory = previousDirectory;
						}
					}
				}

				ImGui::SameLine();
				ImGui::Text(data.mCurrentDirectory.string().c_str());
				ImGui::Separator();

				
				DrawFilesInFolder(data.mCurrentDirectory, data);

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
		const DrawFilesInFolderData data
		{
			sceneManager,
			assetManager,
			*mNodeScriptingWindow,
			*mNodeScriptParentTab,
			*mNodeScriptButton,
			mCanOpenPopup,
			mFilePopUpID,
			mFileToRemove,
			mCurrentDirectory
		};
		RenderAssetBrowserPopup(data, GetImGuiName());
	}

}

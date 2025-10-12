#include "Editor/Precompiled/EditorPch.hpp"
#include "FlyScriptEditorUtilities.hpp"
#include "NodeScript/Fly.hpp"
#include "NodeScript/Command/FlyCommandTracker.hpp"
#include <External/imgui/imnodes/imnodes.h>
#include <External/imgui/imnodes/imnodes_internal.h>

namespace Simple
{

	static bool DataTypeCombo(const char* comboLabel, Fly::GenericDataTypeProxy& dataTypeProxy, auto filterPredicate)
	{
		bool wasSelected = false;
		const std::string defaultName = dataTypeProxy ? dataTypeProxy.GetName() : "None";
		if (ImGui::BeginCombo(comboLabel, defaultName.c_str()))
		{
			const std::vector<Fly::GenericDataTypeProxy> dataTypes = Fly::GetGenericDataTypesFiltered(filterPredicate);

			for (const Fly::GenericDataTypeProxy& dataType : dataTypes)
			{
				if (ImGui::Selectable(dataType.GetName().c_str(), dataTypeProxy == dataType))
				{
					dataTypeProxy = dataType;
					wasSelected = true;
				}
			}

			ImGui::EndCombo();
		}
		return wasSelected;
	}

	bool DataTypeComboEditableFilter(const char* comboLabel, Fly::GenericDataTypeProxy& dataTypeProxy)
	{
		return DataTypeCombo(comboLabel, dataTypeProxy, [](const Fly::GenericDataTypeProxy& dataType) -> bool { return dataType.IsViewAndEditable(); });
	}

	bool DataTypeComboTargetableFilter(const char* comboLabel, Fly::GenericDataTypeProxy& dataTypeProxy)
	{
		return DataTypeCombo(comboLabel, dataTypeProxy, [](const Fly::GenericDataTypeProxy& dataType) -> bool { return dataType.IsTargetable(); });
	}

	bool DataTypeComboNoFilter(const char* comboLabel, Fly::GenericDataTypeProxy& dataTypeProxy)
	{
		return DataTypeCombo(comboLabel, dataTypeProxy, [](const Fly::GenericDataTypeProxy&) -> bool { return true; });
	}

	void UpdateClickPos(NodeGraphContext& nodeGraphContext)
	{
		nodeGraphContext.mNodeCreationClickPos = GetMousePos(nodeGraphContext);
	}

	ImVec2 GetMousePos(const NodeGraphContext& nodeGraphContext)
	{
		return ImGui::GetMousePosOnOpeningCurrentPopup() - nodeGraphContext.mImNodesContext->CanvasOriginScreenSpace - ImNodes::EditorContextGetPanning();
	}

	void ShowNodeGraph(NodeGraphContext& nodeGraphContext)
	{
		ImNodes::SetCurrentContext(nodeGraphContext.mImNodesContext);
		VisualizeNodeGraph(nodeGraphContext);
		UpdateNodeGraph(nodeGraphContext);
		NodeCreation(nodeGraphContext);
	}

	void VisualizeNodeGraph(NodeGraphContext& nodeGraphContext)
	{

		const Fly::NodeGraphProxy& nodeGraphProxy = nodeGraphContext.mNodeGraph;
		ImNodes::BeginNodeEditor();

		for (const auto& node : nodeGraphProxy.IterateNodes())
		{

			ImNodesStyle& style = ImNodes::GetStyle();

			if (node.GetEventID() == Fly::InvalidID<Fly::EventID>())
			{
				style.Colors[ImNodesCol_TitleBar] = ToImGuiColor(Fly::Color{ 0.1f, 0.3f, 0.6f, 1.f });
				style.Colors[ImNodesCol_TitleBarHovered] = ToImGuiColor(Fly::Color{ 0.1f, 0.3f, 0.7f, 1.f });
				style.Colors[ImNodesCol_TitleBarSelected] = ToImGuiColor(Fly::Color{ 0.1f, 0.3f, 0.8f, 1.f });
			}
			else
			{
				style.Colors[ImNodesCol_TitleBar] = ToImGuiColor(Fly::Color{ 0.8f, 0.1f, 0.1f, 1.f });
				style.Colors[ImNodesCol_TitleBarHovered] = ToImGuiColor(Fly::Color{ 0.9f, 0.1f, 0.1f, 1.f });
				style.Colors[ImNodesCol_TitleBarSelected] = ToImGuiColor(Fly::Color{ 1.f, 0.1f, 0.1f, 1.f });
			}

			style.Colors[ImNodesCol_NodeOutline] = ImNodes::IsNodeSelected(node.GetID()) ? ToImGuiColor(Fly::Color(0.8f, 0.8f, 0.8f, 1.f)) : ToImGuiColor(Fly::Color(0.f, 0.f, 0.f, 1.f));

			ImNodes::SetNodeGridSpacePos(node.GetID(), ImVec2{ node.GetPosition().x, node.GetPosition().y });
			ImNodes::BeginNode(node.GetID());

			float nodeNameWidth = 0;

			{ // Node title bar

				ImNodes::BeginNodeTitleBar();

				std::string nodeLabel;

				if (node.IsAccessor())
				{
					/*const Fly::VariableProxy variable = nodeProxy.GetVariableProxy();
					const bool isGetter = Fly::HasFlag(nodeProxy.GetTraits(), Fly::eNodeTrait::Getter);
					const char* const prefixLabel = isGetter ? "Get " : "Set ";
					nodeLabel = prefixLabel + variable.GetName();*/
				}
				else
				{
					nodeLabel = node.GetNodeType().GetShortName();
				}

				if (Fly::IsDebugging())
				{
					nodeLabel += ", ID: " + std::to_string(node.GetID());
				}
				ImGui::TextUnformatted(nodeLabel.c_str());

				nodeNameWidth = ImGui::CalcTextSize(nodeLabel.c_str()).x;

				ImNodes::EndNodeTitleBar();
			}

			ImVec2 cursorPos = ImGui::GetCursorPos();

			std::vector<Fly::PinProxy> inputPinProxys = node.GetSplitInputPins();
			float nodeWidthLeft = inputPinProxys.empty() ? 0.f : 100.f;

			for (const Fly::PinProxy& inputPinProxy : inputPinProxys)
			{
				const std::string_view pinLabel = inputPinProxy.GetPinTypeName();
				const float labelWidth = ImGui::CalcTextSize(std::string(pinLabel).c_str()).x;

				nodeWidthLeft = std::max(nodeWidthLeft, labelWidth);
			}

			float nodeWidthRight = 0.f;

			std::vector<Fly::PinProxy> outputPins = node.GetSplitOutputPins();

			for (const Fly::PinProxy& outputPinProxy : outputPins)
			{
				const std::string_view pinLabel = outputPinProxy.GetPinTypeName();
				const float labelWidth = ImGui::CalcTextSize(std::string(pinLabel).c_str()).x;

				nodeWidthRight = std::max(nodeWidthRight, labelWidth);
			}

			float extraWidth = 0.f;

			if (nodeWidthLeft > 0.f && nodeWidthRight > 0.f)
			{
				extraWidth += 30.f;
			}

			// Render input pins
			for (Fly::PinProxy& inputPinProxy : inputPinProxys)
			{
				const Fly::GenericDataTypeProxy pinDataType(inputPinProxy.GetDataTypeID());
				ImNodes::PushColorStyle(ImNodesCol_Pin, ToImGuiColor(pinDataType.GetColor()));
				ImNodes::PushColorStyle(ImNodesCol_PinHovered, ToImGuiColor(pinDataType.GetColor()/* - myHoverTint*/));

				const bool shouldBeHighlighted = false;// std::find(currentNodeContext.myPinProxysToHighlight.begin(), currentNodeContext.myPinProxysToHighlight.end(), inputPinProxy) != currentNodeContext.myPinProxysToHighlight.end();


				const ImNodesPinShape shape = [shouldBeHighlighted, inputPinProxy]()
					{
						if (shouldBeHighlighted)
						{
							return ImNodesPinShape_Triangle;
						}

						const bool isPointer = Fly::GenericDataTypeProxy(inputPinProxy.GetDataTypeID()).IsPointer();
						return isPointer ? ImNodesPinShape_QuadFilled : ImNodesPinShape_CircleFilled;
					}();
				ImNodes::BeginInputAttribute(inputPinProxy.GetID(), shape);

				std::string pinLabel(inputPinProxy.GetPinTypeName());
				if (!pinLabel.empty())
				{
					if (Fly::IsDebugging())
					{
						pinLabel += ", " + std::to_string(inputPinProxy.GetID());
					}
					ImGui::TextUnformatted(pinLabel.c_str());
				}

				if (inputPinProxy.GetConnectedPinIDs().empty())
				{
					const float itemWidth = std::max(20.f, nodeWidthLeft);
					ImGui::PushItemWidth(itemWidth);

					inputPinProxy.ViewAndEdit(nodeGraphContext.mCommandTracker.get());

					ImGui::PopItemWidth();
				}

				ImNodes::EndInputAttribute();
				ImNodes::PopColorStyle();
				ImNodes::PopColorStyle();
			}

			ImGui::SetCursorPos(cursorPos);

			// Render output pins
			for (const Fly::PinProxy& outputPinProxy : outputPins)
			{
				const Fly::GenericDataTypeProxy pinDataType(outputPinProxy.GetDataTypeID());
				ImNodes::PushColorStyle(ImNodesCol_Pin, ToImGuiColor(pinDataType.GetColor()));
				ImNodes::PushColorStyle(ImNodesCol_PinHovered, ToImGuiColor(pinDataType.GetColor() - NodeGraphContext::HOVER_TINT));

				const bool shouldBeHighlighted = std::ranges::find(nodeGraphContext.mPinsToHighlight, outputPinProxy) != nodeGraphContext.mPinsToHighlight.end();

				const ImNodesPinShape shape = [shouldBeHighlighted, outputPinProxy]()
					{
						if (shouldBeHighlighted)
						{
							return ImNodesPinShape_Triangle;
						}

						const bool isPointer = Fly::GenericDataTypeProxy(outputPinProxy.GetDataTypeID()).IsPointer();
						return isPointer ? ImNodesPinShape_QuadFilled : ImNodesPinShape_CircleFilled;
					}();

				ImNodes::BeginOutputAttribute(outputPinProxy.GetID(), shape);

				std::string pinLabel(outputPinProxy.GetPinTypeName());

				if (!pinLabel.empty())
				{
					const float plusWidth = std::max(nodeWidthLeft + extraWidth, nodeNameWidth - nodeWidthRight);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + plusWidth + extraWidth + nodeWidthRight - ImGui::CalcTextSize(pinLabel.c_str()).x);

					if (Fly::IsDebugging())
					{
						pinLabel += ", " + std::to_string(outputPinProxy.GetID());
					}
					ImGui::TextUnformatted(pinLabel.c_str());
				}

				ImNodes::EndInputAttribute();
				ImNodes::PopColorStyle();
				ImNodes::PopColorStyle();
			}

			ImNodes::EndNode();
		}

		for (const Fly::LinkProxy& link : nodeGraphProxy.IterateLinks())
		{
			if (link.IsDestroyed())
			{
				continue;
			}
			const Fly::PinProxy pinProxy = link.GetInputPin();
			const Fly::GenericDataTypeProxy pinDataType(pinProxy.GetDataTypeID());

			const Fly::Color dataTypeColor = pinDataType.GetColor();
			Fly::Color linkColor = dataTypeColor;
			if (std::ranges::find(nodeGraphContext.mTraversedLinks, link) != nodeGraphContext.mTraversedLinks.end())
			{
				linkColor = NodeGraphContext::TRAVERSED_LINK_COLOR;
			}
			ImNodes::PushColorStyle(ImNodesCol_Link, ToImGuiColor(linkColor));
			ImNodes::PushColorStyle(ImNodesCol_LinkSelected, ToImGuiColor(linkColor - NodeGraphContext::SELECTION_TINT));
			ImNodes::PushColorStyle(ImNodesCol_LinkHovered, ToImGuiColor(linkColor - NodeGraphContext::HOVER_TINT));

			ImNodes::Link(link.GetID(), link.GetInputPin().GetID(), link.GetOutputPin().GetID());

			ImNodes::PopColorStyle();
			ImNodes::PopColorStyle();
			ImNodes::PopColorStyle();
		}


		nodeGraphContext.mIsNodeEditorHovered = ImNodes::IsEditorHovered();

		ImNodes::MiniMap(.2f, ImNodesMiniMapLocation_BottomRight);
		ImNodes::EndNodeEditor();
	}


	void UpdateNodeGraph(NodeGraphContext& nodeGraphContext)
	{
		Fly::NodeGraphProxy& nodeGraphProxy = nodeGraphContext.mNodeGraph;
		bool dragStarted = ImGui::IsKeyDown(ImGuiKey_MouseLeft) && !nodeGraphContext.mIsDraggingNode;
		if (dragStarted)
		{
			if (ImNodes::NumSelectedNodes() > 0) // Drag node started
			{
				nodeGraphContext.mIsDraggingNode = true;
			}
		}

		bool dragEnded = ImGui::IsKeyReleased(ImGuiKey_MouseLeft) && nodeGraphContext.mIsDraggingNode;
		if (dragEnded)
		{
			nodeGraphContext.mIsDraggingNode = false;
		}

		for (Fly::NodeProxy node : nodeGraphProxy.IterateNodes())
		{

			const Fly::Vec2 newPos = ToFlyVec2(ImNodes::GetNodeGridSpacePos(node.GetID()));
			Fly::Vec2 oldPos = node.GetPosition();
			if (dragStarted && ImNodes::IsNodeSelected(node.GetID()))
			{
				nodeGraphContext.mNodeDragData.emplace(node.GetID(), Fly::NodeDragData{ .mStartPos = oldPos });
			}
			else if (dragEnded)
			{
				auto it = nodeGraphContext.mNodeDragData.find(node.GetID());

				if (it != nodeGraphContext.mNodeDragData.end())
				{
					it->second.mEndPos = newPos;
				}
			}
			if (newPos != oldPos)
			{
				node.SetPosition(newPos);
			}
		}

		if (dragEnded)
		{
			nodeGraphContext.mNodeGraph.CommitNodeDrag(nodeGraphContext.mNodeDragData, nodeGraphContext.mCommandTracker.get());
			nodeGraphContext.mNodeDragData.clear();
		}

		Fly::NodeID hoveredNodeID = Fly::InvalidID<Fly::NodeID>();
		if (ImNodes::IsNodeHovered(&hoveredNodeID.mID) && ImGui::IsKeyPressed(ImGuiKey_MouseRight))
		{
			nodeGraphContext.mClickedNode = Fly::NodeProxy(hoveredNodeID, nodeGraphContext.mNodeGraph);

			ImGui::OpenPopup("Node Popup");
		}

		if (ImGui::BeginPopup("Node Popup"))
		{
			ImGui::Text("Node Options");

			ImGui::Separator();
			if (ImGui::Selectable("Destroy Node"))
			{
				nodeGraphContext.mClickedNode.Destroy(nodeGraphContext.mCommandTracker.get());
				ImGui::CloseCurrentPopup();
			}

			ImGui::BeginDisabled(!nodeGraphContext.mClickedNode.HasAnyConnectedLinks());
			if (ImGui::Selectable("Destroy Links"))
			{
				nodeGraphContext.mClickedNode.DestroyConnectedLinks(nodeGraphContext.mCommandTracker.get());
			}
			ImGui::EndDisabled();

			ImGui::Separator();

			if (nodeGraphContext.mClickedNode.IsReplacable() && ImGui::BeginMenu("Data Type"))
			{
				const std::vector<Fly::DataTypeProxy> replacableDataTypeProxys = nodeGraphContext.mClickedNode.GetNodeType().GetReplacableDataTypes();
				for (const Fly::DataTypeProxy& replacableDataTypeProxy : replacableDataTypeProxys)
				{
					ImGui::PushID(static_cast<int>(replacableDataTypeProxy.GetID()));
					if (ImGui::Selectable(replacableDataTypeProxy.GetName().c_str()))
					{
						nodeGraphContext.mNodeGraph.ReplaceTemplateNode(nodeGraphContext.mClickedNode, replacableDataTypeProxy, nodeGraphContext.mCommandTracker.get());
						ImGui::CloseCurrentPopup();
					}
					ImGui::PopID();
				}

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}
		else
		{
			nodeGraphContext.mClickedNode = {};
		}

		Fly::PinID hoveredPinID;
		if (ImNodes::IsPinHovered(&hoveredPinID.mID) && ImGui::IsKeyPressed(ImGuiKey_MouseRight))
		{
			nodeGraphContext.mClickedPin = Fly::PinProxy(hoveredPinID, nodeGraphContext.mNodeGraph);

			ImGui::OpenPopup("Pin Popup");

		}

		if (ImGui::BeginPopup("Pin Popup"))
		{

			ImGui::Text("Pin Options");

			ImGui::Separator();

			Fly::GenericDataTypeProxy pinDataType(nodeGraphContext.mClickedPin.GetDataTypeID());
			ImGui::Text("Type: %s", pinDataType.GetName().c_str());


			if (nodeGraphContext.mClickedPin.GetIODirection() == Fly::eIODirection::Input)
			{
				nodeGraphContext.mClickedPin.ViewAndEdit(nodeGraphContext.mCommandTracker.get());
			}
			else
			{
				nodeGraphContext.mClickedPin.View();
			}

			ImGui::Separator();

			ImGui::BeginDisabled(!nodeGraphContext.mClickedPin.IsSplitable());
			if (ImGui::Selectable("Split Pin"))
			{
				nodeGraphContext.mClickedPin.Split(nodeGraphContext.mCommandTracker.get());
			}
			ImGui::EndDisabled();

			ImGui::BeginDisabled(!nodeGraphContext.mClickedPin.IsRecombinable());
			if (ImGui::Selectable("Recombine Pin"))
			{
				nodeGraphContext.mClickedPin.RecombineParentPin(nodeGraphContext.mCommandTracker.get());
			}
			ImGui::EndDisabled();

			ImGui::BeginDisabled(!nodeGraphContext.mClickedPin.HasAnyConnectedLinks());
			if (ImGui::Selectable("Destroy Links"))
			{
				nodeGraphContext.mClickedPin.DestroyConnectedLinks(nodeGraphContext.mCommandTracker.get());
			}
			ImGui::EndDisabled();

			ImGui::Separator();

			if (nodeGraphContext.mClickedPin.IsParentNodeReplacable() && ImGui::BeginMenu("Choose Type"))
			{
				const Fly::GenericDataTypeID d = nodeGraphContext.mClickedPin.GetDataTypeID();
				const Fly::DataTypeID* clickedPinDataTypeID = std::get_if<Fly::DataTypeID>(&d.mID);
				if (clickedPinDataTypeID)
				{
					const std::vector<Fly::DataTypeProxy> replacableDataTypes = Fly::GetNodeOwner(nodeGraphContext.mClickedPin).GetNodeType().GetReplacableDataTypes();
					for (const Fly::DataTypeProxy& replacableDataType : replacableDataTypes)
					{
						ImGui::PushID(static_cast<int>(replacableDataType.GetID()));
						if (ImGui::Selectable(replacableDataType.GetName().c_str(), replacableDataType.GetID() == *clickedPinDataTypeID))
						{
							nodeGraphContext.mNodeGraph.ReplaceTemplateNode(nodeGraphContext.mClickedPin, replacableDataType, nodeGraphContext.mCommandTracker.get());
							ImGui::CloseCurrentPopup();
						}
						ImGui::PopID();
					}
				}


				ImGui::EndMenu();
			}


			ImGui::EndPopup();
		}
		else
		{
			nodeGraphContext.mClickedPin = Fly::PinProxy();
		}


		// See if links should be created

		Fly::PinID createdLinkPinID1;
		Fly::PinID createdLinkPinID2;

		if (ImNodes::IsLinkCreated(&createdLinkPinID1.mID, &createdLinkPinID2.mID))
		{
			nodeGraphContext.mNodeGraph.TryCreateLink(Fly::PinProxy(createdLinkPinID1, nodeGraphContext.mNodeGraph), Fly::PinProxy(createdLinkPinID2, nodeGraphContext.mNodeGraph), nodeGraphContext.mCommandTracker.get());
			nodeGraphContext.mPinsToHighlight.clear();
		}

		// See if links or nodes should be destroyed

		if (ImGui::IsKeyPressed(ImGuiKey_Delete))
		{
			std::vector<Fly::LinkID> selectedLinks(ImNodes::NumSelectedLinks());
			std::vector<Fly::NodeID> selectedNodes(ImNodes::NumSelectedNodes());

			if (!selectedLinks.empty())
			{
				ImNodes::GetSelectedLinks(reinterpret_cast<int*>(selectedLinks.data()));
			}

			if (!selectedNodes.empty())
			{
				ImNodes::GetSelectedNodes(reinterpret_cast<int*>(selectedNodes.data()));
			}

			if (!selectedLinks.empty() || !selectedNodes.empty())
			{
				nodeGraphContext.mNodeGraph.DestroySelection(selectedNodes, selectedLinks, nodeGraphContext.mCommandTracker.get());
			}

			ImNodes::ClearNodeSelection();
			ImNodes::ClearLinkSelection();
		}

		// Highlight pins
		{
			Fly::PinID startedPinID;
			if (ImNodes::IsLinkStarted(&startedPinID.mID))
			{
				nodeGraphContext.mStartedLinkPinID = startedPinID;

				const Fly::PinProxy startedPin(startedPinID, nodeGraphContext.mNodeGraph);

				nodeGraphContext.mPinsToHighlight = startedPin.GetPotentialConnections();

				std::erase_if(nodeGraphContext.mPinsToHighlight,
					[&](const Fly::PinProxy& pinProxy)-> bool
					{
						return pinProxy.GetID() == startedPin.GetID();
					}
				);
			}
		}

		// Dropped link
		Fly::PinID droppedPinID = Fly::InvalidID<Fly::PinID>();

		if (ImNodes::IsLinkDropped(&droppedPinID.mID))
		{
			ImGui::OpenPopup(NodeGraphContext::NODE_SEARCH_POPUP_NAME);
			nodeGraphContext.mLinkCreationPinID = droppedPinID;
			nodeGraphContext.mPinsToHighlight.clear();
			nodeGraphContext.mSearchNodeData.mNodeTypeSearch[0] = '\0';

			UpdateClickPos(nodeGraphContext);

			auto categoryFunc = [&nodeGraphContext](NodeTypeCategory& aMainCategory) -> void
				{
					const Fly::PinProxy pinProxy(nodeGraphContext.mLinkCreationPinID, nodeGraphContext.mNodeGraph);

					auto nodeTypePredicate = [pinProxy](const Fly::NodeTypeProxy& nodeType)
						{
							const auto pinTypes = Fly::SelectByIODirection(Fly::InvertIODirection(pinProxy.GetIODirection()), nodeType.GetInputPinTypes(), nodeType.GetOutputPinTypes());
							for (const Fly::PinTypeProxy pinType : pinTypes)
							{
								if (pinType.GetDataTypeID() == pinProxy.GetDataTypeID())
								{
									return true;
								}
							}
							return false;
						};

					if (nodeGraphContext.mSearchNodeData.mNodeTypeSearch[0] == '\0')
					{
						for (const Fly::NodeTypeProxy& nodeType : Fly::IterateNodeTypes(nodeTypePredicate))
						{
							PopulateNodeCategories(nodeType.GetName(), nodeType, aMainCategory);
						}
					}
					else
					{
						for (const Fly::NodeTypeProxy& nodeType : Fly::IterateNodeTypes(nodeTypePredicate))
						{
							const bool isSearched = SearchString(nodeType.GetName(), std::string_view(nodeGraphContext.mSearchNodeData.mNodeTypeSearch));
							if (isSearched)
							{
								aMainCategory.mNodeTypes.push_back(nodeType);
							}
						}
					}

				};

			auto onClickCallback = [&nodeGraphContext](const Fly::NodeTypeProxy& nodeType) -> void
				{
					nodeGraphContext.mNodeGraph.CreateNodeAutoLink(nodeType, nodeGraphContext.mLinkCreationPinID, Fly::Vec2{ nodeGraphContext.mNodeCreationClickPos.x,nodeGraphContext.mNodeCreationClickPos.y }, nodeGraphContext.mCommandTracker.get());

					nodeGraphContext.mPinsToHighlight.clear();
					nodeGraphContext.mSearchNodeData.mNodeTypeSearch[0] = '\0';
				};

			nodeGraphContext.mSearchNodeData.mCategoryFunction = categoryFunc;
			nodeGraphContext.mSearchNodeData.mOnClickFunction = onClickCallback;
		}

		if (Fly::IsDebugging())
		{

			if (ImGui::Begin("Debug Data"))
			{

				ImGui::Text("Links:");

				for (const Fly::LinkProxy& link : nodeGraphContext.mNodeGraph.IterateLinks())
				{
					if (link.IsDestroyed())
					{
						continue;
					}
					ImGui::Separator();
					ImGui::Text("In: %d, Out: %d", link.GetInputPin().GetID(), link.GetOutputPin().GetID());
				}

				ImGui::Separator();

				if (nodeGraphContext.mHoveredPin)
				{
					ImGui::Text("Hovered PinID: %d", nodeGraphContext.mHoveredPin.GetID());
					ImGui::Text("Connections:");
					for (const Fly::PinID connectionID : nodeGraphContext.mHoveredPin.GetConnectedPinIDs())
					{
						ImGui::Text("%d", connectionID);
					}
				}

			}
			ImGui::End();
		}

	}

	void NodeCreation(NodeGraphContext& nodeGraphContext)
	{

		if (ImGui::IsKeyPressed(ImGuiKey_MouseRight) && nodeGraphContext.mIsNodeEditorHovered && !nodeGraphContext.mClickedPin && !nodeGraphContext.mClickedNode)
		{
			ImGui::OpenPopup(NodeGraphContext::NODE_SEARCH_POPUP_NAME);
			UpdateClickPos(nodeGraphContext);
			nodeGraphContext.mSearchNodeData.mNodeTypeSearch[0] = '\0';


			auto categoryFunc = [&](NodeTypeCategory& mainCategory) -> void
				{

					if (nodeGraphContext.mSearchNodeData.mNodeTypeSearch[0] == '\0')
					{
						auto filterPredicate = [](const Fly::NodeTypeProxy& nodeType) { return Fly::HasNotFlag(nodeType.GetTraits(), Fly::eNodeTrait::NonTrivial); };
						for (const Fly::NodeTypeProxy& nodeType : Fly::IterateNodeTypes(filterPredicate))
						{
							PopulateNodeCategories(nodeType.GetName(), nodeType, mainCategory);
						}
					}
					else
					{
						for (const Fly::NodeTypeProxy& nodeType : Fly::IterateNodeTypes())
						{
							const bool isSearched = SearchString(nodeType.GetName(), std::string_view(nodeGraphContext.mSearchNodeData.mNodeTypeSearch));
							if (isSearched)
							{
								mainCategory.mNodeTypes.push_back(nodeType);
							}
						}
					}
				};

			auto onClickCallback = [&](const Fly::NodeTypeProxy& nodeTypeProxy) -> void
				{

					nodeGraphContext.mNodeGraph.CreateNode(nodeTypeProxy, Fly::Vec2{ nodeGraphContext.mNodeCreationClickPos.x, nodeGraphContext.mNodeCreationClickPos.y }, nodeGraphContext.mCommandTracker.get());


					nodeGraphContext.mPinsToHighlight.clear();
					nodeGraphContext.mSearchNodeData.mNodeTypeSearch[0] = '\0';
				};
			nodeGraphContext.mSearchNodeData.mCategoryFunction = categoryFunc;
			nodeGraphContext.mSearchNodeData.mOnClickFunction = onClickCallback;
		}

		ShowNodeSearchMenu(nodeGraphContext);
	}

	bool ShowNodeSearchMenu(const std::vector<Fly::NodeTypeProxy>& nodeTypes, const NodeGraphContext& nodeGraphContext)
	{
		bool wasClicked = false;
		for (int i = 0; i < nodeTypes.size(); i++)
		{
			const Fly::NodeTypeProxy& nodeType = nodeTypes[i];

			const bool isCurrentlySelected = nodeGraphContext.mSearchNodeData.mCurrentIndex == i;
			if (ImGui::MenuItem(nodeType.GetShortName().c_str(), nullptr, isCurrentlySelected) ||
				(isCurrentlySelected && ImGui::IsKeyPressed(ImGuiKey_Enter)))
			{
				nodeGraphContext.mSearchNodeData.mOnClickFunction(nodeType);
				wasClicked = true;
			}
		}
		return wasClicked;
	}

	bool ShowNodeSearchMenuByCategory(const NodeTypeCategory& category, const NodeGraphContext& nodeGraphContext)
	{
		bool wasClicked = false;
		if (ImGui::BeginMenu(category.mName.c_str()))
		{

			for (const NodeTypeCategory& childCategory : category.mChildCategories)
			{
				wasClicked |= ShowNodeSearchMenuByCategory(childCategory, nodeGraphContext);
			}

			wasClicked |= ShowNodeSearchMenu(category.mNodeTypes, nodeGraphContext);

			ImGui::EndMenu();
		}
		return wasClicked;
	}

	void ShowNodeSearchMenu(NodeGraphContext& nodeGraphContext)
	{
		if (ImGui::BeginPopup(NodeGraphContext::NODE_SEARCH_POPUP_NAME))
		{

			NodeTypeCategory mainCategory{ "Create Node" };
			nodeGraphContext.mSearchNodeData.mCategoryFunction(mainCategory);

			ImGui::SetKeyboardFocusHere();
			ImGui::InputTextWithHint("##", "Node Type", nodeGraphContext.mSearchNodeData.mNodeTypeSearch, IM_ARRAYSIZE(nodeGraphContext.mSearchNodeData.mNodeTypeSearch));

			bool wasClicked = false;

			if (nodeGraphContext.mSearchNodeData.mNodeTypeSearch[0] == '\0')
			{

				for (const NodeTypeCategory& category : mainCategory.mChildCategories)
				{
					wasClicked |= ShowNodeSearchMenuByCategory(category, nodeGraphContext);
				}
			}
			else
			{
				wasClicked |= ShowNodeSearchMenu(mainCategory.mNodeTypes, nodeGraphContext);
			}

			if (wasClicked)
			{
				nodeGraphContext.mSearchNodeData.mCurrentIndex = 0;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		else
		{
			nodeGraphContext.mSearchNodeData = {};
		}
	}

	void PopulateNodeCategories(const std::string& name, const Fly::NodeTypeProxy& nodeType, NodeTypeCategory& category)
	{
		std::string categoryName = name.substr(0, name.find_first_of('/'));
		std::string rest = name.substr(categoryName.length(), name.length() - 1);

		if (rest.empty())
		{
			category.mNodeTypes.push_back(nodeType);
			return;
		}

		rest = rest.substr(1, rest.length());

		bool hasFoundCategory = false;
		for (NodeTypeCategory& childCategory : category.mChildCategories)
		{
			if (childCategory.mName == categoryName)
			{
				PopulateNodeCategories(rest, nodeType, childCategory);
				hasFoundCategory = true;
				break;
			}
		}

		if (!hasFoundCategory)
		{
			category.mChildCategories.emplace_back(categoryName);
			PopulateNodeCategories(rest, nodeType, category.mChildCategories.back());
		}
	}

	NodeGraphContext::NodeGraphContext()
	{
		mImNodesContext = ImNodes::CreateContext();
		mCommandTracker = std::make_unique<Fly::CommandTracker>();
	}

	NodeGraphContext::~NodeGraphContext()
	{
		ImNodes::DestroyContext(mImNodesContext);
	}
}

#pragma once
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/Point2.hpp"
#include "Engine/Math/Point3.hpp"
#include "Engine/Math/Transform3.hpp"
#include "Engine/Math/UnitVector2.hpp"
#include "Engine/Math/UnitVector3.hpp"
#include "Engine/Utility/Color.hpp"
#include "Engine/Utility/RGBColor.hpp"
#include "Engine/Math/Angle.hpp"
#include "Engine/Math/Shapes/AABB2.hpp"
#include "Engine/Math/Shapes/AABB3.hpp"
#include "Engine/Math/Shapes/Cylinder.hpp"
#include "Engine/Math/TransformMode.hpp"

#include "Engine/Utility/Camera.hpp"
#include "Engine/Graphics/Sprite/Sprite2D.hpp"
#include "Engine/Graphics/Sprite/Sprite3D.hpp"
#include "Engine/Graphics/Light/PointLight.hpp"
#include "Engine/Graphics/Light/DirectionalLight.hpp"
#include "Engine/Math/Shapes/Shape.hpp"
#include "Engine/Asset/AssetTypes/TextureAsset.hpp"
#include "Engine/Asset/AssetTypes/MeshAsset.hpp"
#include "Engine/Asset/AssetTypes/AnimationAsset.hpp"

#include <string>
#include <array>

#include "Engine/Reflection/Reflection.hpp"
#include "Engine/Reflection/EditDataFunctions.hpp"
#include "Engine/Reflection/LoadDataFunctions.hpp"
#include "Engine/Reflection/SaveDataFunctions.hpp"

inline bool Global_ECS_Registered_Datatype_float = CLX::TypeRegistration::RegisterType<float>();
inline bool Global_ECS_Registered_Datatype_int = CLX::TypeRegistration::RegisterType<int>();
inline bool Global_ECS_Registered_Datatype_bool = CLX::TypeRegistration::RegisterType<bool>();
inline bool Global_ECS_Registered_Datatype_char = CLX::TypeRegistration::RegisterType<char>();

namespace std
{
	REGISTER_DATATYPE(string);
	REGISTER_DATATYPE_ARRAY(array, CLX::TextureAssetHandle, 3);
}

namespace CLX
{
	REGISTER_DATATYPE(Vector2f);
	REGISTER_DATATYPE(Vector3f);
	REGISTER_DATATYPE(Vector4f);
	REGISTER_DATATYPE(Point2f);
	REGISTER_DATATYPE(Point3f);
	REGISTER_DATATYPE(Transform);
	REGISTER_DATATYPE(UnitVector2f);
	REGISTER_DATATYPE(UnitVector3f);
	REGISTER_DATATYPE(Color);
    REGISTER_DATATYPE(RGBColor);
	REGISTER_DATATYPE(Radiansf);
	REGISTER_DATATYPE(Degreesf);
	REGISTER_DATATYPE(AABB2f);
	REGISTER_DATATYPE(AABB2i);
	REGISTER_DATATYPE(Cylinderf);
	REGISTER_DATATYPE(AABB3f);
	REGISTER_DATATYPE(Spheref);
	REGISTER_DATATYPE(EntityID);
	REGISTER_DATATYPE(eTransformMode);
	REGISTER_DATATYPE(Shape);

	REGISTER_DATATYPE(PointLight);
	REGISTER_DATATYPE(DirectionalLight);
	REGISTER_DATATYPE(Camera);
	REGISTER_DATATYPE(Sprite2D);
	REGISTER_DATATYPE(Sprite3D);

	REGISTER_DATATYPE(MeshAssetHandle);
	REGISTER_DATATYPE(ModelAssetHandle);
	REGISTER_DATATYPE(AnimatedModelAssetHandle);
	REGISTER_DATATYPE(TextureAssetHandle);
	REGISTER_DATATYPE(AnimationAssetHandle);
	REGISTER_DATATYPE(SceneAssetHandle);
	REGISTER_DATATYPE(EntityCompositionAssetHandle);

    //REGISTER_MEMBER_2(CustomVariableData("Near Plane", &Camera::GetNearPlane, &Camera::SetNearPlane), EditorSpeed{0.001f});
}
#pragma once
#include "Utility/Math/Vector2.hpp"
#include "Utility/Math/Vector3.hpp"
#include "Utility/Math/Vector4.hpp"
#include "Utility/Math/Transform3.hpp"
#include "Utility/Math/TransformMode.hpp"

#include "Utility/Camera.hpp"
#include "Graphics/Sprite/Sprite2D.hpp"
#include "Graphics/Light/PointLight.hpp"
#include "Graphics/Light/DirectionalLight.hpp"
#include "Utility/Asset/TextureAsset.hpp"
#include "Utility/Asset/MeshAsset.hpp"
#include "Utility/Asset/AnimationAsset.hpp"

#include <string>
#include <array>

#include "Engine/Reflection/Reflection.hpp"
#include "Engine/Reflection/EditDataFunctions.hpp"
#include "Engine/Reflection/LoadDataFunctions.hpp"
#include "Engine/Reflection/SaveDataFunctions.hpp"

inline Simple::__RegisterDataType<float> Global_ECS_Registered_Datatype_float;
inline Simple::__RegisterDataType<int> Global_ECS_Registered_Datatype_int;
inline Simple::__RegisterDataType<bool> Global_ECS_Registered_Datatype_bool;
inline Simple::__RegisterDataType<char> Global_ECS_Registered_Datatype_char;

namespace std
{
	REGISTER_DATATYPE(string);
	REGISTER_DATATYPE_ARRAY(array, Simple::TextureAssetHandle, 3);
}

namespace Simple
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
	REGISTER_DATATYPE(Radiansf);
	REGISTER_DATATYPE(Degreesf);
	REGISTER_DATATYPE(AABB2f);
	REGISTER_DATATYPE(AABB2i);
	REGISTER_DATATYPE(EntityID);
	REGISTER_DATATYPE(eTransformMode);
}

namespace Simple
{
	REGISTER_DATATYPE(PointLight);
	REGISTER_DATATYPE(DirectionalLight);
	REGISTER_DATATYPE(Camera);
	REGISTER_DATATYPE(Sprite2D);

	REGISTER_DATATYPE(MeshAssetHandle);
	REGISTER_DATATYPE(ModelAssetHandle);
	REGISTER_DATATYPE(AnimatedModelAssetHandle);
	REGISTER_DATATYPE(TextureAssetHandle);
	REGISTER_DATATYPE(AnimationAssetHandle);
	REGISTER_DATATYPE_POINTER(PixelShader);
	REGISTER_DATATYPE_POINTER(VertexShader);
}
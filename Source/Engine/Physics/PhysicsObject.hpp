#pragma once
#include "Engine/Physics/Velocity.hpp"
#include "Engine/Physics/Mass.hpp"
#include "Engine/Physics/Gravity.hpp"
#include "Utility/Shapes/Sphere.hpp"
#include "Utility/Shapes/Cylinder.hpp"
#include "Utility/Color.hpp"
#include <variant>

namespace Simple
{
	namespace Physics
	{

		enum class ePhysicsObjectType : uint8_t
		{
			Static,
			Dynamic,
		};

		using PhysicsShape = std::variant<Spheref, Cylinderf>;

		class PhysicsObject final
		{
		public:

			PhysicsObject() = default;

			[[nodiscard]] constexpr const Velocityf& GetVelocity() const { return myVelocity; }
			constexpr void SetVelocity(const Velocityf& aVelocity) { myVelocity = aVelocity; }
			constexpr void AddVelocity(const Velocityf& aVelocity) { myVelocity += aVelocity; }
			[[nodiscard]] constexpr Massf& GetMass() { return myMass; }
			[[nodiscard]] constexpr Massf GetMass() const { return myMass; }
			constexpr void SetMass(const Massf& pMass) { myMass = pMass; }

			[[nodiscard]] constexpr PhysicsShape& GetShape() { return myShape; }
			[[nodiscard]] constexpr const PhysicsShape& GetShape() const { return myShape; }

			template<typename T>
			constexpr void SetShape(const T& aShape)
			{
				myShape = aShape;
			}

			constexpr void SetPosition(const Point3f& aPosition)
			{
				std::visit([&aPosition](auto& shape) { shape.SetCenter(aPosition); }, myShape);
			}
			[[nodiscard]] constexpr Point3f GetPosition() const
			{
				return std::visit([](const auto& shape) { return shape.GetCenter(); }, myShape);
			}

			[[nodiscard]] constexpr ePhysicsObjectType GetType() const { return myType; }
			constexpr void SetType(const ePhysicsObjectType aType) { myType = aType; }

			[[nodiscard]] constexpr Color GetColor() const { return myColor; }
			constexpr void SetColor(const Color& aColor) { myColor = aColor; }

			[[nodiscard]] constexpr float GetCurve() const { return myCurve; }
			constexpr void SetCurve(const float aCurve) { myCurve = aCurve; }

		private:

			Velocityf myVelocity;
			Massf myMass;
			Accelerationf myGravity = EARTH_GRAVITY_ACCELERATION<float>;
			PhysicsShape myShape;
			ePhysicsObjectType myType{ ePhysicsObjectType::Dynamic };
			Color myColor = Colors::Blue;
			float myCurve = 0.f;
		};
	}
}
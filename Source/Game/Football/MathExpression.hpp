#pragma once
#include <variant>
#include <functional>

template<typename T>
class MathFunction
{
public:

	constexpr explicit MathFunction(T(*pFunction)(T, T))
		: mFunction(pFunction)
	{
	}

	constexpr T operator()(const T& pValue1, const T& pValue2) const
	{
		return mFunction(pValue1, pValue2);
	}

private:

	T(*mFunction)(T, T) = nullptr;
};

template<typename T>
using MathOperation = std::variant<std::plus<T>, std::minus<T>, std::multiplies<T>, std::divides<T>, MathFunction<T>>;

struct FunctionInput {};

template<typename T>
class MathExpression
{
	class InternalMathExpression
	{
	public:

		InternalMathExpression() = default;

		InternalMathExpression(const MathExpression& pExpression)
			: mExpression(std::make_unique<MathExpression>(pExpression))
		{
		}

		InternalMathExpression(const InternalMathExpression& pOther)
			: mExpression(pOther.mExpression ? std::make_unique<MathExpression>(*pOther.mExpression) : nullptr)
		{
		}

		InternalMathExpression(InternalMathExpression&& pOther) noexcept
			: mExpression(std::move(pOther.mExpression))
		{
		}

		InternalMathExpression& operator=(const InternalMathExpression& pOther)
		{
			mExpression = pOther.mExpression ? std::make_unique<MathExpression>(*pOther.mExpression) : nullptr;
			return *this;
		}

		InternalMathExpression& operator=(InternalMathExpression&& pOther) noexcept
		{
			mExpression = std::move(pOther.mExpression);
			return *this;
		}

		T operator()(const T& pValue) const
		{
			return (*mExpression)(pValue);
		}

	private:

		std::unique_ptr<MathExpression> mExpression;
	};

	using ExpressionVariant = std::variant<T, InternalMathExpression, FunctionInput>;

public:

	MathExpression() = default;

	MathExpression(const ExpressionVariant& pExpression1, const ExpressionVariant& pExpression2, MathOperation<T> pOperation)
		: mExpression1(pExpression1)
		, mExpression2(pExpression2)
		, mOperation(pOperation)
	{
	}

	MathExpression(const ExpressionVariant& pExpression1, const MathExpression& pExpression2, MathOperation<T> pOperation)
		: mExpression1(pExpression1)
		, mExpression2(InternalMathExpression(pExpression2))
		, mOperation(pOperation)
	{
	}

	MathExpression(const MathExpression& pExpression1, const ExpressionVariant& pExpression2, MathOperation<T> pOperation)
		: mExpression1(InternalMathExpression(pExpression1))
		, mExpression2(pExpression2)
		, mOperation(pOperation)
	{
	}

	MathExpression(const MathExpression& pExpression1, const MathExpression& pExpression2, MathOperation<T> pOperation)
		: mExpression1(InternalMathExpression(pExpression1))
		, mExpression2(InternalMathExpression(pExpression2))
		, mOperation(pOperation)
	{
	}

	constexpr T operator()(const T& pValue) const
	{
		struct ExpressionVisitor
		{
			T mValue{};

			T operator()(const T& p) const
			{
				return p;
			}

			T operator()(const InternalMathExpression& pExp) const
			{
				return pExp(mValue);
			}

			T operator()(FunctionInput) const
			{
				return mValue;
			}
		};

		const ExpressionVisitor expressionVisitor{ pValue };
		const T value1 = std::visit(expressionVisitor, mExpression1);
		const T value2 = std::visit(expressionVisitor, mExpression2);
		return std::visit([value1, value2](const auto& pOperation) { return pOperation(value1, value2); }, mOperation);
	}

	void ReplaceInput(const MathExpression& pExpression)
	{
		struct ReplaceVisitor
		{
			MathExpression& mOwner;
			const MathExpression& mExpression;
			ExpressionVariant& mSelf;

			void operator()(const T&) const
			{
			}

			void operator()(const InternalMathExpression&) const
			{
				mOwner.ReplaceInput(mExpression);
			}

			void operator()(FunctionInput) const
			{
				mSelf = mExpression;
			}
		};

		ReplaceVisitor replaceVisitor1{ *this, pExpression, mExpression1 };
		ReplaceVisitor replaceVisitor2{ *this, pExpression, mExpression2 };

		std::visit(replaceVisitor1, mExpression1);
		std::visit(replaceVisitor2, mExpression2);
	}

private:

	ExpressionVariant mExpression1;
	ExpressionVariant mExpression2;
	MathOperation<T> mOperation;
};

using MathExpressionf = MathExpression<float>;
using MathExpressiond = MathExpression<double>;

template<typename T>
MathExpression<T> operator+(const MathExpression<T>& pExpression1, const MathExpression<T>& pExpression2)
{
	return MathExpression<T>(pExpression1, pExpression2, std::plus<T>{});
}

template<typename T>
MathExpression<T> operator+(const MathExpression<T>& pExpression, const T& pValue)
{
	return MathExpression<T>(pExpression, pValue, std::plus<T>{});
}

template<typename T>
MathExpression<T> operator+(const T& pValue, const MathExpression<T>& pExpression)
{
	return MathExpression<T>(pValue, pExpression, std::plus<T>{});
}

template<typename T>
MathExpression<T> operator-(const MathExpression<T>& pExpression1, const MathExpression<T>& pExpression2)
{
	return MathExpression<T>(pExpression1, pExpression2, std::minus<T>{});
}

template<typename T>
MathExpression<T> operator-(const MathExpression<T>& pExpression, const T& pValue)
{
	return MathExpression<T>(pExpression, pValue, std::minus<T>{});
}

template<typename T>
MathExpression<T> operator-(const T& pValue, const MathExpression<T>& pExpression)
{
	return MathExpression<T>(pValue, pExpression, std::minus<T>{});
}

template<typename T>
MathExpression<T> operator*(const MathExpression<T>& pExpression1, const MathExpression<T>& pExpression2)
{
	return MathExpression<T>(pExpression1, pExpression2, std::multiplies<T>{});
}

template<typename T>
MathExpression<T> operator*(const MathExpression<T>& pExpression, const T& pValue)
{
	return MathExpression<T>(pExpression, pValue, std::multiplies<T>{});
}

template<typename T>
MathExpression<T> operator*(const T& pValue, const MathExpression<T>& pExpression)
{
	return MathExpression<T>(pValue, pExpression, std::multiplies<T>{});
}

template<typename T>
MathExpression<T> operator/(const MathExpression<T>& pExpression1, const MathExpression<T>& pExpression2)
{
	return MathExpression<T>(pExpression1, pExpression2, std::divides<T>{});
}

template<typename T>
MathExpression<T> operator/(const MathExpression<T>& pExpression, const T& pValue)
{
	return MathExpression<T>(pExpression, pValue, std::divides<T>{});
}

template<typename T>
MathExpression<T> operator/(const T& pValue, const MathExpression<T>& pExpression)
{
	return MathExpression<T>(pValue, pExpression, std::divides<T>{});
}

template<typename T>
MathExpression<T> ToDerivative(const MathExpression<T>& pExpression)
{
	constexpr T Almost0{ static_cast<T>(0.000000001) };
	constexpr T TestValue = 100;
	static_assert(TestValue + Almost0 != TestValue);
	MathExpression<T> newExpression = pExpression;
	newExpression.ReplaceInput(MathExpression<T>(FunctionInput{}, Almost0, std::plus<T>{}));
	return (newExpression - pExpression) / Almost0;
}
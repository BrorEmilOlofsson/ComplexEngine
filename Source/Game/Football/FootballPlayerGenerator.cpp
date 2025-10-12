#include "Game/Precompiled/GamePch.hpp"
#include "FootballPlayerGenerator.hpp"
#include "Probability.hpp"
#include "MathExpression.hpp"
#include "NameGenerator.hpp"
#include "HumanNames.hpp"
#include "Random.hpp"

MathExpressionf GaussianExpression()
{
	auto r = MathExpressionf(FunctionInput{}, 2.f, MathFunction(std::powf)) * -1.f;
	return MathExpressionf(Napier<float>, r, MathFunction(std::powf));
}

MathExpressionf GaussianExpression(const float pMean, const float pStdDeviation)
{
	const auto exp1 = MathExpressionf(MathExpressionf(FunctionInput{}, pMean, std::minus<float>{}), 2.f, MathFunction(std::powf));
	const auto exp2 = MathExpressionf(Napier<float>, (exp1 / (pStdDeviation * pStdDeviation * 2.f)) * -1.f, MathFunction(std::powf));
	return (1.f / (pStdDeviation * std::sqrtf(2.f * static_cast<float>(std::_Pi_val)))) * exp2;
}

Height GenerateHeight()
{
	//auto heightFunction = GaussianExpression(180.f, 20.f);
	return Height(GenerateNormalRandomValue(180.f, 7.6f));
}

eNationalAssociation GenerateNationalAssociation()
{
	return static_cast<eNationalAssociation>(GetRandomNumber(std::size_t{0}, EnumCount<eNationalAssociation>::value - 1));
}

FootballPlayer GenerateFootballPlayer()
{
	FootballPlayer player;

	auto der = ToDerivative(MathExpressiond(FunctionInput{}, 2.0f, MathFunction<double>(std::pow)));

	const double y0 = der(2);
	y0;

	player.SetMainCitizenship(GenerateNationalAssociation());
	player.GetStats().mHeight = GenerateHeight();
	player.SetName(GenerateName(Names::GetNamesByAssociation(player.GetMainCitizenship(), Names::FirstNamesByAssociation), Names::GetNamesByAssociation(player.GetMainCitizenship(), Names::LastNamesByAssociation)));

	return player;
}
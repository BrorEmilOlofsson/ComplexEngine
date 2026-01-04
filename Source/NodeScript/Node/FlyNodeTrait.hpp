#pragma once
#include "../FlyDefines.hpp"

namespace FLY_NAMESPACE
{

    enum class eNodeTrait : size_t
    {
        None = 0,
        Invalid = 1 << 0,
        CPPGenerated = 1 << 1,
        EditorGenerated = 1 << 2,
        Getter = 1 << 3,
        Setter = 1 << 4,
        Operator = 1 << 5,
        Wildcard = 1 << 6,
        TemplateSpec = 1 << 7,
        Template = Wildcard | TemplateSpec,
        CustomEvent = 1 << 8,
        HasFlow = 1 << 9,
        HasImplicitFlow = 1 << 10,
        TakesExecutionContext = 1 << 11,
        TakesInternalExecutionContext = 1 << 12,
        Target = 1 << 13,
        Trait = 1 << 14,
        NonTrivial = Invalid | Getter | Setter | Operator | TemplateSpec | CustomEvent | Target,
        Accessor = Getter | Setter,
        All = ~None
    };

    [[nodiscard]] constexpr eNodeTrait operator|(eNodeTrait a, eNodeTrait b)
    {
        return static_cast<eNodeTrait>(static_cast<std::underlying_type_t<eNodeTrait>>(a) | static_cast<std::underlying_type_t<eNodeTrait>>(b));
    }

    enum class eNodeOperatorType : unsigned int
    {
        None = 0,
        Equal = 1 << 0,
        NotEqual = 1 << 1,
        Greater = 1 << 2,
        Less = 1 << 3,
        GreaterEqual = 1 << 4,
        LessEqual = 1 << 5,
        And = 1 << 6,
        Or = 1 << 7,
        Not = 1 << 8,
        Print = 1 << 9,
        Add = 1 << 10,
        Subtract = 1 << 11,
        Multiply = 1 << 12,
        Divide = 1 << 13,
        Modulo = 1 << 14,
        IncrementPrefix = 1 << 15,
        DecrementPrefix = 1 << 16,

        Comparator = Equal | NotEqual | Greater | Less | GreaterEqual | LessEqual,
        Logical = And | Or | Not,
        Math = Add | Subtract | Multiply | Divide | Modulo,
        All = Comparator | Logical | Math | Print | IncrementPrefix | DecrementPrefix
    };
}
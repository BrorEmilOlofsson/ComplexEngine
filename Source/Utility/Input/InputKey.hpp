#pragma once
#include "Utility/EnumUtility.hpp"

namespace Simple
{

    enum class eInputKey : unsigned char
    {
        None = 0x00,

        LMB = 0x01,
        RMB = 0x02,
        Cancel = 0x03,
        MMB = 0x04,

        X1 = 0x05,
        X2 = 0x06,

        Backspace = 0x08,
        Tab = 0x09,

        Clear = 0x0C,
        Enter = 0x0D,

        Shift = 0x10,
        Ctrl = 0x11,
        Alt = 0x12,
        Pause = 0x13,
        CapsLock = 0x14,
        Esc = 0x1B,
        Space = 0x20,
        PageUp = 0x21,
        PageDown = 0x22,
        End = 0x23,
        Home = 0x24,
        Left = 0x25,
        Up = 0x26,
        Right = 0x27,
        Down = 0x28,
        Insert = 0x2D,
        Delete = 0x2E,

        // Number keys
        D0 = 0x30,
        D1 = 0x31,
        D2 = 0x32,
        D3 = 0x33,
        D4 = 0x34,
        D5 = 0x35,
        D6 = 0x36,
        D7 = 0x37,
        D8 = 0x38,
        D9 = 0x39,

        // A-Z
        A = 0x41,
        B = 0x42,
        C = 0x43,
        D = 0x44,
        E = 0x45,
        F = 0x46,
        G = 0x47,
        H = 0x48,
        I = 0x49,
        J = 0x4A,
        K = 0x4B,
        L = 0x4C,
        M = 0x4D,
        N = 0x4E,
        O = 0x4F,
        P = 0x50,
        Q = 0x51,
        R = 0x52,
        S = 0x53,
        T = 0x54,
        U = 0x55,
        V = 0x56,
        W = 0x57,
        X = 0x58,
        Y = 0x59,
        Z = 0x5A,

        // Function keys
        F1 = 0x70,
        F2 = 0x71,
        F3 = 0x72,
        F4 = 0x73,
        F5 = 0x74,
        F6 = 0x75,
        F7 = 0x76,
        F8 = 0x77,
        F9 = 0x78,
        F10 = 0x79,
        F11 = 0x7A,
        F12 = 0x7B,

        // Numpad
        NumPad0 = 0x60,
        NumPad1 = 0x61,
        NumPad2 = 0x62,
        NumPad3 = 0x63,
        NumPad4 = 0x64,
        NumPad5 = 0x65,
        NumPad6 = 0x66,
        NumPad7 = 0x67,
        NumPad8 = 0x68,
        NumPad9 = 0x69,
        Multiply = 0x6A,
        Add = 0x6B,
        Separator = 0x6C,
        Subtract = 0x6D,
        Decimal = 0x6E,
        Divide = 0x6F,
        // Other
        PrintScreen = 0x2C,
        NumLock = 0x90,
        ScrollLock = 0x91,
        LShift = 0xA0,
        RShift = 0xA1,
        LCtrl = 0xA2,
        RCtrl = 0xA3,
        LAlt = 0xA4,
        RAlt = 0xA5,
        Menu = 0x5D,
    };

    template<>
    struct EnumCount<eInputKey> : std::integral_constant<std::size_t, 256> {};

    constexpr std::string_view EnumToString(const eInputKey key)
    {
        switch (key)
        {
        case eInputKey::None: return "None";
        case eInputKey::LMB: return "LMB";
        case eInputKey::RMB: return "RMB";
        case eInputKey::Cancel: return "Cancel";
        case eInputKey::MMB: return "MMB";
        case eInputKey::X1: return "X1";
        case eInputKey::X2: return "X2";
        case eInputKey::Backspace: return "Backspace";
        case eInputKey::Tab: return "Tab";
        case eInputKey::Clear: return "Clear";
        case eInputKey::Enter: return "Enter";
        case eInputKey::Shift: return "Shift";
        case eInputKey::Ctrl: return "Ctrl";
        case eInputKey::Alt: return "Alt";
        case eInputKey::Pause: return "Pause";
        case eInputKey::CapsLock: return "CapsLock";
        case eInputKey::Esc: return "Esc";
        case eInputKey::Space: return "Space";
        case eInputKey::PageUp: return "PageUp";
        case eInputKey::PageDown: return "PageDown";
        case eInputKey::End: return "End";
        case eInputKey::Home: return "Home";
        case eInputKey::Left: return "Left";
        case eInputKey::Up: return "Up";
        case eInputKey::Right: return "Right";
        case eInputKey::Down: return "Down";
        case eInputKey::Insert: return "Insert";
        case eInputKey::Delete: return "Delete";

        case eInputKey::D0: return "0";
        case eInputKey::D1: return "1";
        case eInputKey::D2: return "2";
        case eInputKey::D3: return "3";
        case eInputKey::D4: return "4";
        case eInputKey::D5: return "5";
        case eInputKey::D6: return "6";
        case eInputKey::D7: return "7";
        case eInputKey::D8: return "8";
        case eInputKey::D9: return "9";

        case eInputKey::A: return "A";
        case eInputKey::B: return "B";
        case eInputKey::C: return "C";
        case eInputKey::D: return "D";
        case eInputKey::E: return "E";
        case eInputKey::F: return "F";
        case eInputKey::G: return "G";
        case eInputKey::H: return "H";
        case eInputKey::I: return "I";
        case eInputKey::J: return "J";
        case eInputKey::K: return "K";
        case eInputKey::L: return "L";
        case eInputKey::M: return "M";
        case eInputKey::N: return "N";
        case eInputKey::O: return "O";
        case eInputKey::P: return "P";
        case eInputKey::Q: return "Q";
        case eInputKey::R: return "R";
        case eInputKey::S: return "S";
        case eInputKey::T: return "T";
        case eInputKey::U: return "U";
        case eInputKey::V: return "V";
        case eInputKey::W: return "W";
        case eInputKey::X: return "X";
        case eInputKey::Y: return "Y";
        case eInputKey::Z: return "Z";

        case eInputKey::F1: return "F1";
        case eInputKey::F2: return "F2";
        case eInputKey::F3: return "F3";
        case eInputKey::F4: return "F4";
        case eInputKey::F5: return "F5";
        case eInputKey::F6: return "F6";
        case eInputKey::F7: return "F7";
        case eInputKey::F8: return "F8";
        case eInputKey::F9: return "F9";
        case eInputKey::F10: return "F10";
        case eInputKey::F11: return "F11";
        case eInputKey::F12: return "F12";

        case eInputKey::NumPad0: return "NumPad0";
        case eInputKey::NumPad1: return "NumPad1";
        case eInputKey::NumPad2: return "NumPad2";
        case eInputKey::NumPad3: return "NumPad3";
        case eInputKey::NumPad4: return "NumPad4";
        case eInputKey::NumPad5: return "NumPad5";
        case eInputKey::NumPad6: return "NumPad6";
        case eInputKey::NumPad7: return "NumPad7";
        case eInputKey::NumPad8: return "NumPad8";
        case eInputKey::NumPad9: return "NumPad9";
        case eInputKey::Multiply: return "Multiply";
        case eInputKey::Add: return "Add";
        case eInputKey::Separator: return "Separator";
        case eInputKey::Subtract: return "Subtract";
        case eInputKey::Decimal: return "Decimal";
        case eInputKey::Divide: return "Divide";

        case eInputKey::PrintScreen: return "PrintScreen";
        case eInputKey::NumLock: return "NumLock";
        case eInputKey::ScrollLock: return "ScrollLock";

        case eInputKey::LShift: return "LShift";
        case eInputKey::RShift: return "RShift";
        case eInputKey::LCtrl: return "LCtrl";
        case eInputKey::RCtrl: return "RCtrl";
        case eInputKey::LAlt: return "LAlt";
        case eInputKey::RAlt: return "RAlt";

        case eInputKey::Menu: return "Menu";

        default: return "Unknown";
        }
    }
}
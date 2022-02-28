#pragma once
#include <string_view>
#include <string>
#include <iostream>
#include <iomanip>
#include "magic_enum.hpp"
using namespace std;
enum struct Rarity {
    Common = 1,
    Rare = 2,
    Epic = 3,
    Legendary = 4
};


enum struct TypeEquipment {
    Nil = 0,
    Range = 1,
    Melee = 2,
    Armour = 3
};

struct IEquipment {
    string indent;
    TypeEquipment type;
    int level;
    Rarity rarity;

    IEquipment(const string_view& name, const TypeEquipment& tp, const string_view& lvl, const string_view& rare);

    virtual void print();
};

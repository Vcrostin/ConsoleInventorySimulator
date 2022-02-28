#pragma once
#include "equipment.h"
#include "function_lib.h"

struct WeaponElem : public IEquipment {
    double damage;
    double speed;

    WeaponElem(const string_view& name, const TypeEquipment& tp, const string_view& lvl, const string_view& rare, const string_view& damage, const string_view& speed);

    void print() override;
};

shared_ptr<WeaponElem> make_weapon(const string_view& name, const TypeEquipment& tp);

#pragma once
#include <memory>
#include "equipment.h"
#include "function_lib.h"
struct ArmourElem : public IEquipment {
    double protection;

    ArmourElem(const string_view& name, const TypeEquipment& tp, const string_view& lvl, const string_view& rare, const string_view& protection);

    void print() override;
};

shared_ptr<ArmourElem> make_armour(const string_view& name, const TypeEquipment& tp);

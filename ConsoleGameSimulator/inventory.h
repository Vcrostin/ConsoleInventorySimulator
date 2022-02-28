#pragma once
#include "equipment.h"
#include "buff.h"
#include "weapon.h"
#include "armour.h"

struct Inventory {
    unordered_map<string_view, shared_ptr<IEquipment>> value;

    unordered_map<string_view, shared_ptr<Buff>> buffs;

    void print_inventory(ostream& out) const;

    void print_buffs(ostream& out) const;
};

ostream& operator<< (ostream& out, const Inventory& inv);

shared_ptr<IEquipment> make_equipment(const vector<string_view>& str_vct);

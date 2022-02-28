#include "equipment.h"

IEquipment::IEquipment(const string_view& name, const TypeEquipment& tp, const string_view& lvl, const string_view& rare) {
    indent = name;
    level = stoi(string(lvl));
    rarity = magic_enum::enum_cast<Rarity>(rare).value();
    type = tp;
}

void IEquipment::print() {
    cout << left << fixed << setprecision(1) << setw(20) << indent << " " << setw(7) << magic_enum::enum_name(type) << " " << setw(2) << level << " " << setw(7) << magic_enum::enum_name(rarity) << " ";
}
#include "inventory.h"

inline void Inventory::print_inventory(ostream& out) const {
    for (auto& [k, v] : value) {
        v->print();
        out << endl;
    }
}

inline void Inventory::print_buffs(ostream& out) const {
    for (auto& [k, v] : buffs) {
        v->print();
        out << endl;
    }
}

ostream& operator<< (ostream& out, const Inventory& inv) {
    inv.print_inventory(out);
    out << endl;
    inv.print_buffs(out);
    return out;
}

shared_ptr<IEquipment> make_equipment(const vector<string_view>& str_vct) {
    TypeEquipment type = TypeEquipment::Nil;
    string construct = "";
    for (const auto& str : str_vct) {
        auto str_arr = split(str, ':');
        for (auto& s : str_arr) {
            s = strip(s);
        }
        if (str_arr[0] == "type") {
            type = magic_enum::enum_cast<TypeEquipment>(str_arr[1]).value();
        }
        else {
            construct += ":" + string(str_arr[1]);
        }
    }

    switch (type)
    {
    case TypeEquipment::Range:
        return shared_ptr<WeaponElem>(make_weapon(construct, type));
        break;
    case TypeEquipment::Melee:
        return shared_ptr<WeaponElem>(make_weapon(construct, type));
        break;
    case TypeEquipment::Armour:
        return shared_ptr<ArmourElem>(make_armour(construct, type));
        break;
    default:
        throw new invalid_argument("Type in equipment is not specified");
        break;
    }

}

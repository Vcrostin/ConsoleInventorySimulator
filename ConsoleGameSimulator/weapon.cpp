#include "weapon.h"

WeaponElem::WeaponElem(const string_view& name, const TypeEquipment& tp, const string_view& lvl, const string_view& rare, const string_view& damage, const string_view& speed) :
    IEquipment(name, tp, lvl, rare) {
    this->damage = stod(string(damage));
    this->speed = stod(string(speed));
}

void WeaponElem::print() {
    IEquipment::print();
    cout << damage << " " << speed << " ";
}

shared_ptr<WeaponElem> make_weapon(const string_view& name, const TypeEquipment& tp) {
    auto spl = split(strip(name), ':');
    if (spl.size() != 5) {
        throw new invalid_argument("Incorect sequance in Weapon");
    }
    return shared_ptr<WeaponElem>(new WeaponElem(spl[0], tp, spl[1], spl[2], spl[3], spl[4]));
}

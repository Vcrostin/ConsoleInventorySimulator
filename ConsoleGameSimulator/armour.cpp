#include "armour.h"


ArmourElem::ArmourElem(const string_view& name, const TypeEquipment& tp, const string_view& lvl, const string_view& rare, const string_view& protection) :
    IEquipment(name, tp, lvl, rare) {
    this->protection = stod(string(protection));
}

void ArmourElem::print() {
    IEquipment::print();
    cout << protection << " ";
}

shared_ptr<ArmourElem> make_armour(const string_view& name, const TypeEquipment& tp) {
    auto spl = split(strip(name), ':');
    if (spl.size() != 4) {
        throw new invalid_argument("Incorect sequance in Armour");
    }
    return shared_ptr<ArmourElem>(new ArmourElem(spl[0], tp, spl[1], spl[2], spl[3]));
}

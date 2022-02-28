#include "buff_instances.h"

DamageBuff::DamageBuff(const string_view& indent, Filters&& filters2parse, const TypeModificators& tm, double value) :
    Buff(indent, move(filters2parse), tm) {
    this->value = value;
}

void DamageBuff::print() {
    Buff::print();
    cout << value << " ";
}

bool DamageBuff::operation(shared_ptr<IEquipment>& eq) {
    if (eq->type == TypeEquipment::Range || eq->type == TypeEquipment::Melee) {
        dynamic_pointer_cast<WeaponElem>(eq)->damage += value;
        return true;
    }
    return false;
}

DamageBuff::~DamageBuff() {

}

SpeedBuff::SpeedBuff(const string_view& indent, Filters&& filters2parse, const TypeModificators& tm, double value) :
    Buff(indent, move(filters2parse), tm) {
    this->value = value;
}

void SpeedBuff::print() {
    Buff::print();
    cout << value << " ";
}

bool SpeedBuff::operation(shared_ptr<IEquipment>& eq) {
    if (eq->type == TypeEquipment::Range || eq->type == TypeEquipment::Melee) {
        dynamic_pointer_cast<WeaponElem>(eq)->speed += value;
        return true;
    }
    return false;
}

SpeedBuff::~SpeedBuff() {

}

ProtectionBuff::ProtectionBuff(const string_view& indent, Filters&& filters2parse, const TypeModificators& tm, double value) :
    Buff(indent, move(filters2parse), tm) {
    this->value = value;
}

void ProtectionBuff::print() {
    Buff::print();
    cout << value << " ";
}

bool ProtectionBuff::operation(shared_ptr<IEquipment>& eq) {
    if (eq->type == TypeEquipment::Armour) {
        dynamic_pointer_cast<ArmourElem>(eq)->protection += value;
        return true;
    }
    return false;
}

ProtectionBuff::~ProtectionBuff() {

}

shared_ptr<Buff> make_buff(const string_view& ident_part, const string_view& filters_part, const string_view& other_args) {
    string indent = string(strip((split(strip(split(ident_part, ',')[0]), ':')[1])));
    auto split_other_args = split(strip(other_args), ',');
    string content = "";
    TypeModificators type = TypeModificators::Nil;
    for (auto& str : split_other_args) {
        auto str_arr = split(str, ':');
        for (auto& s : str_arr) {
            s = strip(s);
        }
        if (str_arr[0] == "type") {
            type = magic_enum::enum_cast<TypeModificators>(str_arr[1]).value();
        }
        else {
            content += ":" + string(str_arr[1]);
        }
    }
    Filters filter_data;
    filter_data.SetPrinted(filters_part);
    auto filters_arr = split(filters_part, '}');
    for (auto& f : filters_arr) {
        f = strip(f);
    }
    for (const auto& filter : filters_arr) {
        filter_data.add_filters(filter);
    }
    vector<string_view> values = split(content, ':');
    switch (type)
    {
    case TypeModificators::DamageBuff:
        if (values.size() != 1)
            throw new invalid_argument("DamageBuff accepted only 1 additional argument!");
        return shared_ptr<DamageBuff>(new DamageBuff(indent, move(filter_data), type, stod(string(values[0]))));
        break;
    case TypeModificators::ProtectionBuff:
        if (values.size() != 1)
            throw new invalid_argument("ProtectionBuff accepted only 1 additional argument!");
        return shared_ptr<ProtectionBuff>(new ProtectionBuff(indent, move(filter_data), type, stod(string(values[0]))));
        break;
    case TypeModificators::SpeedBuff:
        if (values.size() != 1)
            throw new invalid_argument("SpeedBuff accepted only 1 additional argument!");
        return shared_ptr<SpeedBuff>(new SpeedBuff(indent, move(filter_data), type, stod(string(values[0]))));
        break;
    default:
        throw new invalid_argument("Type in buff is not specified");
        break;
    }
}

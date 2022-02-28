#include "filters.h"

Filters::Filters() {
    printed_data = "";
}

void Filters::SetPrinted(const string_view& str) {
    printed_data = str;
}

bool Filters::check_filers(const shared_ptr<IEquipment>& equipment) {
    bool check_type = false;
    if (all_filters.count("type") > 0) {
        for (auto& f : all_filters["type"]) {
            check_type = check_type || f(equipment);
        }
        if (!check_type)
            return false;
    }
    if (all_filters.count("level") > 0) {
        for (auto& f : all_filters["level"]) {
            if (!f(equipment))
                return false;
        }
    }
    bool check_rarity = false;
    if (all_filters.count("rarity") > 0) {
        for (auto& f : all_filters["rarity"]) {
            check_rarity = check_rarity || f(equipment);
        }
        return check_rarity;
    }
    return true;
}

void Filters::add_filters(const string_view& str) {
    auto str_arr = split(str, ',');
    for (auto& s : str_arr) {
        s = strip(s);
    }
    // one of
    if (str_arr.size() == 0)
        throw new invalid_argument("filter must contain more then one parameter");
    if (str_arr[0] == "type") {
        if (str_arr.size() != 2)
            throw new invalid_argument("Type is accepted exactly 1 additional argument");
        TypeEquipment te = magic_enum::enum_cast<TypeEquipment>(str_arr[1]).value();
        all_filters["type"].push_back([te](const shared_ptr<IEquipment>& eq) {
            return te == eq->type;
            });
    }
    // all of
    else if (str_arr[0] == "level") {
        if (str_arr.size() != 3)
            throw new invalid_argument("Level is accepted exactly 2 additional argument");
        int lvl = stoi(string(str_arr[2]));
        if (str_arr[1] == "==") {
            all_filters["level"].push_back([lvl](const shared_ptr<IEquipment>& eq) {
                return eq->level == lvl;
                });
        }
        else if (str_arr[1] == ">=") {
            all_filters["level"].push_back([lvl](const shared_ptr<IEquipment>& eq) {
                return eq->level >= lvl;
                });
        }
        else if (str_arr[1] == ">") {
            all_filters["level"].push_back([lvl](const shared_ptr<IEquipment>& eq) {
                return eq->level > lvl;
                });
        }
        else if (str_arr[1] == "<") {
            all_filters["level"].push_back([lvl](const shared_ptr<IEquipment>& eq) {
                return eq->level < lvl;
                });
        }
        else if (str_arr[1] == "<=") {
            all_filters["level"].push_back([lvl](const shared_ptr<IEquipment>& eq) {
                return eq->level <= lvl;
                });
        }
        else
            throw new invalid_argument("Level first additional argument must be compare symbol");
    }
    // one of
    else if (str_arr[0] == "rarity") {
        if (str_arr.size() != 2)
            throw new invalid_argument("Rarity is accepted exactly 1 additional argument");
        Rarity rare = magic_enum::enum_cast<Rarity>(str_arr[1]).value();
        all_filters["type"].push_back([rare](const shared_ptr<IEquipment>& eq) {
            return rare == eq->rarity;
            });
    }
    else
        throw new invalid_argument("1st argument must be atribute of IEquipment");
}

#pragma once
#include "equipment.h"
#include <functional>
#include "function_lib.h"

enum struct TypeModificators {
    Nil = 0,
    DamageBuff = 1,
    ProtectionBuff = 2,
    SpeedBuff = 3
};

struct Filters {
    unordered_map<string_view, list<function<bool(const shared_ptr<IEquipment>&)>>> all_filters;

    string printed_data;
    Filters();

    void SetPrinted(const string_view& str);

    bool check_filers(const shared_ptr<IEquipment>& equipment);

    void add_filters(const string_view& str);

};

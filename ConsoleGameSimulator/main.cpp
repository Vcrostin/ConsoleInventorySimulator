#include <string>
#include <list>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>
#include <iomanip>
#include <functional>
// to simplify enum code
#include "magic_enum.hpp"
using namespace std;

string_view strip(const string_view& str, vector<char> strip_chars = {' ', ',', '{', '}', '"', ':'}) {
    if (str.size() == 0) {
        return "";
    }
    auto it_f = str.begin();
    while (it_f != str.end() && count(strip_chars.begin(), strip_chars.end(), *it_f) > 0) {
        it_f++;
    }
    if (it_f == str.end()) {
        return "";
    }
    auto it_b = str.end();
    //to get value;
    it_b--;
    while (it_b != it_f && count(strip_chars.begin(), strip_chars.end(), *it_b) > 0) {
        it_b--;
    }
    // return correct range
    it_b++;
    return { it_f, it_b };
}

vector<string_view> split(const string_view& str, char delim)
{
    vector<string_view> tokens;

    auto start = str.begin(), end = find(start, str.end(), delim);
    while (start != str.end())
    {
        if (start != end)
            tokens.push_back(string_view(start, end));
        start = end != str.end() ? end + 1 : end;
        while (start != str.end() && *start == delim)
            start++;
        end = find(start, str.end(), delim);
    }

    return tokens;
}

enum struct Rarity {
    Common = 1,
    Rare = 2,
    Epic = 3,
    Legendary = 4
};


enum struct TypeEquipment {
    Nil = 0,
    Range = 1,
    Melee = 2,
    Armour = 3
};

struct IEquipment {
    string indent;
    TypeEquipment type;
    int level;
    Rarity rarity;
    
    IEquipment(const string_view& name, const TypeEquipment& tp, const string_view& lvl, const string_view& rare) {
        indent = name;
        level = stoi(string(lvl));
        rarity = magic_enum::enum_cast<Rarity>(rare).value();
        type = tp;
    }

    virtual void print() {
        cout << left << fixed << setprecision(1) << setw(20) << indent << " " << setw(7) << magic_enum::enum_name(type) << " " << setw(2) << level << " " << setw(7) << magic_enum::enum_name(rarity) << " ";
    }

    virtual IEquipment* casted() {
        return this;
    }
};

struct ArmourElem: public IEquipment {
    double protection;

    ArmourElem(const string_view& name, const TypeEquipment& tp, const string_view& lvl, const string_view& rare, const string_view& protection) :
        IEquipment(name, tp, lvl, rare) {
        this->protection = stod(string(protection));
    }

    void print() override {
        IEquipment::print();
        cout << protection << " ";
    }

    ArmourElem* casted() override {
        return this;
    }
};

shared_ptr<ArmourElem> make_armour(const string_view& name, const TypeEquipment& tp) {
    auto spl = split(strip(name), ':');
    if (spl.size() != 4) {
        throw new invalid_argument("Incorect sequance in Armour");
    }
    return shared_ptr<ArmourElem>(new ArmourElem(spl[0], tp, spl[1], spl[2], spl[3]));
}

struct WeaponElem : public IEquipment {
    double damage;
    double speed;

    WeaponElem(const string_view& name, const TypeEquipment& tp, const string_view& lvl, const string_view& rare, const string_view& damage, const string_view& speed) :
        IEquipment(name, tp, lvl, rare) {
        this->damage = stod(string(damage));
        this->speed = stod(string(speed));
    }

    void print() override {
        IEquipment::print();
        cout << damage << " " << speed << " ";
    }

    WeaponElem* casted() override {
        return this;
    }
};

shared_ptr<WeaponElem> make_weapon(const string_view& name, const TypeEquipment& tp) {
    auto spl = split(strip(name), ':');
    if (spl.size() != 5) {
        throw new invalid_argument("Incorect sequance in Weapon");
    }
    return shared_ptr<WeaponElem>(new WeaponElem(spl[0], tp, spl[1], spl[2], spl[3], spl[4]));
}

enum struct TypeModificators {
    Nil = 0,
    DamageBuff = 1,
    ProtectionBuff = 2,
    SpeedBuff = 3
};

struct Filters {
    unordered_map<string_view, list<function<bool(const shared_ptr<IEquipment>&)>>> all_filters;

    string printed_data;
    Filters() {
        printed_data = "";
    }

    void SetPrinted(const string_view& str) {
        printed_data = str;
    }

    bool check_filers(const shared_ptr<IEquipment>& equipment) {
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

    void add_filters(const string_view& str) {
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

};

struct Buff {
    string indent;
    TypeModificators type;
    Filters filters;

    Buff(const string_view& indent, Filters&& filters2parse, const TypeModificators& tm) {
        this->indent = indent;
        type = tm;
        filters = move(filters2parse);
    }

    virtual bool operation(shared_ptr<IEquipment>& eq) = 0;

    virtual void print() {
        cout << left << fixed << setprecision(1) << setw(20) << indent << " " << setw(20) << magic_enum::enum_name(type) << " " << filters.printed_data << " ";
    }

    virtual ~Buff() {

    }
};

struct DamageBuff : Buff {
    double value;

    DamageBuff(const string_view& indent, Filters&& filters2parse, const TypeModificators& tm, double value) :
    Buff(indent, move(filters2parse), tm) {
        this->value = value;
    }

    void print() override {
        Buff::print();
        cout << value << " ";
    }

    bool operation(shared_ptr<IEquipment>& eq) override {
        if (eq->type == TypeEquipment::Range || eq->type == TypeEquipment::Melee) {
            dynamic_pointer_cast<WeaponElem>(eq)->damage += value;
            return true;
        }
        return false;
    }

    ~DamageBuff() {

    }
};


struct SpeedBuff : Buff {
    double value;

    SpeedBuff(const string_view& indent, Filters&& filters2parse, const TypeModificators& tm, double value) :
        Buff(indent, move(filters2parse), tm) {
        this->value = value;
    }

    void print() override {
        Buff::print();
        cout << value << " ";
    }

    bool operation(shared_ptr<IEquipment>& eq) override {
        if (eq->type == TypeEquipment::Range || eq->type == TypeEquipment::Melee) {
            dynamic_pointer_cast<WeaponElem>(eq)->speed += value;
            return true;
        }
        return false;
    }

    ~SpeedBuff() {

    }
};

struct ProtectionBuff : Buff {
    double value;

    ProtectionBuff(const string_view& indent, Filters&& filters2parse, const TypeModificators& tm, double value) :
        Buff(indent, move(filters2parse), tm) {
        this->value = value;
    }

    void print() override {
        Buff::print();
        cout << value << " ";
    }

    bool operation(shared_ptr<IEquipment>& eq) override {
        if (eq->type == TypeEquipment::Armour) {
            dynamic_pointer_cast<ArmourElem>(eq)->protection += value;
            return true;
        }
        return false;
    }

    ~ProtectionBuff() {

    }
};

struct Inventory {
    unordered_map<string_view, shared_ptr<IEquipment>> value;

    unordered_map<string_view, shared_ptr<Buff>> buffs;

    void print_inventory(ostream& out) const {
        for (auto& [k, v] : value) {
            v->print();
            out << endl;
        }
    }

    void print_buffs(ostream& out) const {
        for (auto& [k, v] : buffs) {
            v->print();
            out << endl;
        }
    }
};

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

Inventory inventory_init() {
    Inventory inventory;
    {
        ifstream inv_inp("inventory_input.txt");
        string str;
        while (getline(inv_inp, str)) {
            if (str.length() == 0) {
                continue;
            }
            auto cp_str = strip(str);
            auto splitted_str = split(cp_str, ',');
            auto eq = make_equipment(splitted_str);
            inventory.value[eq->indent] = eq;
        }
        inv_inp.close();
    }
    {
        ifstream filter_inp("buff_modificator_input.txt");
        string str;
        while (getline(filter_inp, str)) {
            if (str.length() == 0) {
                continue;
            }
            auto filters_left = split(str, '[');
            if (filters_left.size() != 2)
                throw new invalid_argument("There mus be only one '['");
            auto filters_right = split(filters_left[1], ']');
            if (filters_right.size() != 2)
                throw new invalid_argument("There mus be only one ']'");
            auto bf = make_buff(filters_left[0], filters_right[0], filters_right[1]);
            inventory.buffs[bf->indent] = bf;
        }
        filter_inp.close();
    }
    return inventory;
}

enum struct MENU_ACTIONS {
    ERROR = -1,
    QUIT = 0,
    SHOW_ALL = 1,
    SHOW_INV = 2,
    SHOW_BUFF = 3,
    BUFF = 4
};

MENU_ACTIONS print_menu() {
    cout << "**************************************\n";
    cout << "Choose the action :\n";
    cout << "**************************************\n";
    cout << "0: quit\n";
    cout << "1: show all objects\n";
    cout << "2: show all inventory\n";
    cout << "3: show all buffs\n";
    cout << "4: use buff on element\n";
    cout << "**************************************" << endl;
    string inp;
    getline(cin, inp);
    cout << endl;
    if (inp.size() != 1) {
        return MENU_ACTIONS::ERROR;
    }
    if (isdigit(inp.front())) {
        int digit = stoi(inp);
        if (digit >= static_cast<int>(MENU_ACTIONS::QUIT) && digit <= static_cast<int>(MENU_ACTIONS::BUFF)) {
            return static_cast<MENU_ACTIONS>(digit);
        }
    }
    return MENU_ACTIONS::ERROR;
}

void buff_init(Inventory& inventory) {
    cout << endl;
    cout << "**************************************\n";
    cout << "Input equipment name" << endl;
    string indent;
    getline(cin, indent);

    cout << "Input buff name" << endl;
    string buff;
    getline(cin, buff);
    cout << "**************************************\n";


    if (inventory.buffs.count(buff) > 0 && inventory.value.count(indent) > 0) {
        if (inventory.buffs[buff]->filters.check_filers(inventory.value[indent])) {
            if (inventory.buffs[buff]->operation(inventory.value[indent])) {
                cout << "Buff used succesfully" << endl;
                return;
            }
        }
    }
    cout << "There is a error in buff usage" << endl;
}

void menu_init(Inventory& inventory) {
    do {
        switch (print_menu())
        {
        case MENU_ACTIONS::ERROR:
            cout << "ERROR, try again" << endl;
            break;
        case MENU_ACTIONS::QUIT:
            return;
            break;
        case MENU_ACTIONS::SHOW_ALL:
            cout << inventory << endl;
            break;
        case MENU_ACTIONS::SHOW_INV:
            inventory.print_inventory(cout);
            break;
        case MENU_ACTIONS::SHOW_BUFF:
            inventory.print_buffs(cout);
            break;
        case MENU_ACTIONS::BUFF:
            buff_init(inventory);
            break;
        default:
            break;
        }

    } while (true);
}


int main() {
    Inventory inventory = inventory_init();
    menu_init(inventory);
}

#include "menu.h"

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

MenuActions print_menu() {
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
        return MenuActions::ERROR;
    }
    if (isdigit(inp.front())) {
        int digit = stoi(inp);
        if (digit >= static_cast<int>(MenuActions::QUIT) && digit <= static_cast<int>(MenuActions::BUFF)) {
            return static_cast<MenuActions>(digit);
        }
    }
    return MenuActions::ERROR;
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
        case MenuActions::ERROR:
            cout << "ERROR, try again" << endl;
            break;
        case MenuActions::QUIT:
            return;
            break;
        case MenuActions::SHOW_ALL:
            cout << inventory << endl;
            break;
        case MenuActions::SHOW_INV:
            inventory.print_inventory(cout);
            break;
        case MenuActions::SHOW_BUFF:
            inventory.print_buffs(cout);
            break;
        case MenuActions::BUFF:
            buff_init(inventory);
            break;
        default:
            break;
        }

    } while (true);
}

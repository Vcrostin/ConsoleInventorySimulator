#pragma once
#include "inventory.h"
#include "equipment.h"
#include "buff_instances.h"
#include <fstream>

Inventory inventory_init();

enum struct MenuActions {
    ERROR = -1,
    QUIT = 0,
    SHOW_ALL = 1,
    SHOW_INV = 2,
    SHOW_BUFF = 3,
    BUFF = 4
};

MenuActions print_menu();

void buff_init(Inventory& inventory);

void menu_init(Inventory& inventory);

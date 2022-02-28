#include "menu.h"
using namespace std;

int main() {
    Inventory inventory = inventory_init();
    menu_init(inventory);
}

#include "buff.h"

Buff::Buff(const string_view& indent, Filters&& filters2parse, const TypeModificators& tm) {
    this->indent = indent;
    type = tm;
    filters = move(filters2parse);
}

void Buff::print() {
    cout << left << fixed << setprecision(1) << setw(20) << indent << " " << setw(20) << magic_enum::enum_name(type) << " " << filters.printed_data << " ";
}

Buff::~Buff()
{
}

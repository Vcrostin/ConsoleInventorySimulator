#pragma once
#include "filters.h"

struct Buff {
    string indent;
    TypeModificators type;
    Filters filters;

    Buff(const string_view& indent, Filters&& filters2parse, const TypeModificators& tm);

    virtual bool operation(shared_ptr<IEquipment>& eq) = 0;

    virtual void print();

    virtual ~Buff();
};

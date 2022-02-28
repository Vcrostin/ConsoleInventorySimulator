#pragma once
#include "buff.h"
#include "weapon.h"
#include "armour.h"

struct DamageBuff : Buff {
    double value;

    DamageBuff(const string_view& indent, Filters&& filters2parse, const TypeModificators& tm, double value);

    void print() override;

    bool operation(shared_ptr<IEquipment>& eq) override;

    ~DamageBuff();
};


struct SpeedBuff : Buff {
    double value;

    SpeedBuff(const string_view& indent, Filters&& filters2parse, const TypeModificators& tm, double value);

    void print() override;

    bool operation(shared_ptr<IEquipment>& eq) override;

    ~SpeedBuff();
};

struct ProtectionBuff : Buff {
    double value;

    ProtectionBuff(const string_view& indent, Filters&& filters2parse, const TypeModificators& tm, double value);

    void print() override;

    bool operation(shared_ptr<IEquipment>& eq) override;

    ~ProtectionBuff();
};

shared_ptr<Buff> make_buff(const string_view& ident_part, const string_view& filters_part, const string_view& other_args);

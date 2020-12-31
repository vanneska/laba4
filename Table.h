#ifndef _TABLE_1_H_
#define _TABLE_1_H_
#include <iostream>
#include <map>
#include <string>
#include "Room.h"

namespace Program {
    std::ostream& operator<<(std::ostream&, const std::pair<const int, Rooms::Room*>&);

    class ConstTableIt {
    private:
        std::map<const int, Rooms::Room*>::const_iterator cur;
    public:
        ConstTableIt() {}

        ConstTableIt(std::map<const int, Rooms::Room*>::iterator it) : cur(it) {}

        ConstTableIt(std::map<const int, Rooms::Room*>::const_iterator it) : cur(it) {}

        int operator!=(const ConstTableIt&) const;

        int operator==(const ConstTableIt&) const;

        const std::pair<const int, Rooms::Room*>& operator*();

        const std::pair<const int, Rooms::Room*>* operator->();

        ConstTableIt& operator++();

        ConstTableIt operator++(int);
    };

    class Table {
    private:
        std::map<const int, Rooms::Room*> arr;
    public:
        Table() {}

        Table(const Table&);

        ~Table();

        Table& operator=(const Table&);

        friend class ConstTableIt;

        typedef ConstTableIt Const_Iterator;

        Const_Iterator find(const int&) const;

        bool insert(const int&, const Rooms::Room*);

        bool remove(const int&);

        int lastIndex();

        Const_Iterator begin() const;

        Const_Iterator end() const;
    };
};
#endif

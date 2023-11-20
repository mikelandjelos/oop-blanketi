#ifndef POSETILAC_H
#define POSETILAC_H

#include <string>
#include <iostream>

class Posetilac {
protected:
    double mCenaUlaznice;
    std::string mIme, mPrezime;
    virtual std::ostream& Write(std::ostream&) const;
public:
    Posetilac(double, std::string, std::string);
    virtual ~Posetilac();
    friend std::ostream&
        operator<<(std::ostream&, const Posetilac&);
    virtual double Koef() const = 0;
    inline double GetCenaUl() { return mCenaUlaznice; }
};

#endif
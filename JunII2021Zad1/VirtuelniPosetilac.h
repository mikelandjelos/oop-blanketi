#ifndef VIRTUELNI_POSETILAC_H
#define VIRTUELNI_POSETILAC_H

#include "Posetilac.h"
#include <iostream>
#include <string>

class VirtuelniPosetilac : public Posetilac {
private:
    std::string mKorisnickoIme;
    std::ostream& Write(std::ostream&) const override;
public:
    VirtuelniPosetilac(std::string, double,
        std::string, std::string);
    virtual ~VirtuelniPosetilac();
    double Koef() const override;
};

#endif
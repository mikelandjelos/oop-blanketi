#include "VirtuelniPosetilac.h"

std::ostream& VirtuelniPosetilac::Write(std::ostream& izlTok) const {
    return Posetilac::Write(izlTok)
        << "\nKorisnicko ime: " << mKorisnickoIme;
}

VirtuelniPosetilac::VirtuelniPosetilac(std::string korIme, double cenaUl,
    std::string ime, std::string prez)
    : Posetilac(cenaUl, ime, prez), mKorisnickoIme(korIme) {}

VirtuelniPosetilac::~VirtuelniPosetilac() {
    mKorisnickoIme.clear();
}

// ovo nije bas kao u zadatku
double VirtuelniPosetilac::Koef() const {
    return mCenaUlaznice;
}
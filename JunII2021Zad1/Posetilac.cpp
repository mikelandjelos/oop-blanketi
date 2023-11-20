#include "Posetilac.h"

Posetilac::Posetilac(double cenaUl,
    std::string ime, std::string prezime)
    : mCenaUlaznice(cenaUl), mIme(ime),
    mPrezime(prezime) {}

Posetilac::~Posetilac() {
    mIme.clear();
    mPrezime.clear();
}

std::ostream& Posetilac::Write(std::ostream& izlTok) const {
    return izlTok << "Ime: " << mIme
        << "\nPrezime: " << mPrezime
        << "\nCena ulaznice: " << mCenaUlaznice;
}

std::ostream&
operator<<(std::ostream& izlTok, const Posetilac& p) {
    return p.Write(izlTok); // pokrecemo virt. meh.
}
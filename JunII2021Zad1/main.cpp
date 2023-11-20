#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include "Posetilac.h"
#include "VirtuelniPosetilac.h"

#define BRVPS 4

int main() {

    Posetilac** lista = new Posetilac * [BRVPS]
    {nullptr, nullptr, nullptr, nullptr};
    std::vector<std::string> korImena =
    { "mm", "adj", "sdj", "mj" };
    std::vector<std::string> imena =
    { "Milan", "Aleksandra", "Slobodan", "Milka" };
    std::vector<std::string> prezimena =
    { "Madic", "Djordjevic", "Djordjevic", "Jaksic" };

    for (int i = 0; i < BRVPS; ++i) {
        lista[i] =
            new VirtuelniPosetilac(korImena[i], rand() % 100 + 1,
                imena[i], prezimena[i]);
        std::cout << *lista[i] << '\n';
        delete lista[i];
    }

    delete[] lista;

    return 0;
}
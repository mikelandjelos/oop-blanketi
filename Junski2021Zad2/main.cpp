#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>

#define BROJREALNIH 10
#define BROJBANANA 4

enum Izuzetak {
	errDodavanje,
	errIzbacivanje,
	errKreiranje
};

using namespace std;

template <class Element, int n>
class Kolekcija {
private:
	Element mKolekcija[n];
	int mBrEl;
public:
	Kolekcija() 
		: mBrEl(0) {
		if (n <= 0)
			throw Izuzetak::errKreiranje;
	}
	virtual ~Kolekcija() {}
	void Dodaj(Element& el) {
		if (mBrEl == n)
			throw Izuzetak::errDodavanje;
		mKolekcija[mBrEl++] = el;
	}
	double Prosek() {
		double suma = 0;
		int i = 0;
		for (; i < mBrEl; ++i)
			suma += mKolekcija[i];
		return suma / i;
	}
	void Izbaci(double procenat) {
		if (procenat > 100 || procenat < 0
			|| mBrEl == 0)
			throw Izuzetak::errIzbacivanje;
		double avg = Prosek();
		for (int i = 0; i < mBrEl; ++i) {
			if (abs(100 - mKolekcija[i] * 100 / avg) > procenat) {
				for (int j = i; j < mBrEl - 1; ++j)
					mKolekcija[j] = mKolekcija[j + 1];
				mBrEl--, i--;
			}
		}
	}
	template <class Element, int n>
	friend ostream& operator<<(ostream& izlTok, const Kolekcija<Element, n>& kolekcija);
};

/* friend funkcije templejtskih klasa ne treba da budu
   definisane unutar same klase zato sto to moze da dovede
   do greske C2995 -> visestruka definicija funkcije
   */
template <class Element, int n1>
ostream& operator<<(ostream& izlTok, const Kolekcija<Element, n1>& kolekcija) {
	for (int i = 0; i < kolekcija.mBrEl; ++i)
		izlTok << kolekcija.mKolekcija[i] << '\n';
	return izlTok;
}

class Banana {
private:
	double mTezina;
	bool mZrelost;
public:
	Banana() {}
	Banana(double tezina, bool zrelost)
		: mTezina(tezina), mZrelost(zrelost) {
		if (mZrelost == false)
			mTezina = -1;
	}
	virtual ~Banana() {}
	friend ostream& operator<<(ostream& izlTok, const Banana& banana) {
		return izlTok << "Tezina: " << banana.mTezina
			<< ", Zrelost: " << ((banana.mZrelost) ? "zrela" : "trula");
	}
	double operator*(double real) {
		return mTezina * real;
	}
	friend double& operator+=(double& real, const Banana& banana) {
		return real += banana.mTezina;
	}
};

int main() {
	
	try {

		Kolekcija<double, BROJREALNIH> kolekcijaReal;

		for (int i = 0; i < BROJREALNIH; ++i) {
			double temp = (rand() % 100 + 1) + ((rand() % 100) * 0.24);
			kolekcijaReal.Dodaj(temp);
		}

		std::cout << "\n\t\tKolekcija pre izbacivanja:\n" << kolekcijaReal
			<< "Prosek: " << kolekcijaReal.Prosek() << '\n';

		kolekcijaReal.Izbaci(20);

		std::cout << "\n\t\tKolekcija nakon izbacivanja:\n\n" << kolekcijaReal;

		Kolekcija<Banana, BROJBANANA> kolekcijaBanana;

		Banana temp(120, true);		// 0
		kolekcijaBanana.Dodaj(temp);
		temp = Banana(76, true);	// 1
		kolekcijaBanana.Dodaj(temp);
		temp = Banana(76, true);	// 2
		kolekcijaBanana.Dodaj(temp);
		temp = Banana(5, false);	// 3
		kolekcijaBanana.Dodaj(temp);

		std::cout << "\n\t\tKolekcija banana pre izbacivanja:\n\n" << kolekcijaBanana
			<< "Prosek: " << kolekcijaReal.Prosek() << '\n';

		kolekcijaBanana.Izbaci(40);

		std::cout << "\n\t\tKolekcija banana nakon izbacivanja:\n\n" << kolekcijaBanana;
	
	}
	catch (Izuzetak ex) {
		switch (ex) {
		case Izuzetak::errKreiranje:
			cerr << "\n\t\tGreska u kreiranju kolekcije!\n";
			return Izuzetak::errKreiranje;
		case Izuzetak::errDodavanje:
			cerr << "\n\t\tGreska u dodavanju elementa u kolekciju!\n";
			return Izuzetak::errDodavanje;
		case Izuzetak::errIzbacivanje:
			cerr << "\n\t\tGreska pri izbacivanju elementa iz kolekcije!\n";
			return Izuzetak::errIzbacivanje;
		}
	}

	return 0;
}
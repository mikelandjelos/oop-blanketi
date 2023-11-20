#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

enum Izuzetak {
	errDodavanje,
	errIzbacivanje,
};

template <class Element, int n>
class Skup {
private:
	Element* mSkup;
	int mBrEl;
public:
	Skup()
		: mBrEl(0) {
		mSkup = new Element[n];
	}
	Skup(const Skup<Element, n>& s) {
		mBrEl = s.mBrEl;
		mSkup = new Element[n];
		for (int i = 0; i < mBrEl; ++i)
			mSkup[i] = s.mSkup[i];
	}
	virtual ~Skup() {
		delete[] mSkup;
	}
	inline int Size() { return mSkup; }
	void Dodaj(Element& el) {
		if (mBrEl == n)
			throw Izuzetak::errDodavanje;
		for (int i = 0; i < mBrEl; ++i)
			if (mSkup[i] == el)
				return;
		mSkup[mBrEl++] = el;
	}
	void Izbaci(Element& el) {
		if (mBrEl == 0) // sigurno ne postoji
			throw Izuzetak::errIzbacivanje;
		int indeks = -1;
		for (int i = 0; i < mBrEl; ++i)
			if (mSkup[i] == el) {
				indeks = i;
				break;
			}
		if (indeks == -1) // nije pronadjen
			throw Izuzetak::errIzbacivanje;
		mBrEl--;
		for (int i = indeks; i < mBrEl; ++i)
			mSkup[i] = mSkup[i + 1];
	}
	template <class Element, int n1, int n2>
	friend Skup<Element, n1 + n2> operator+(
		const Skup<Element, n1>& levi, const Skup<Element, n2> desni);
	template <class Element, int n>
	friend ostream& operator<<(ostream& izlTok,
		const Skup<Element, n>& s);
};

template <class Element, int n>
ostream& operator<<(ostream& izlTok,
	const Skup<Element, n>& s) {
	for (int i = 0; i < s.mBrEl; ++i)
		izlTok << s.mSkup[i] << '\n';
	return izlTok;
}

template <class Element, int n1, int n2>
Skup<Element, n1 + n2> operator+(const Skup<Element, n1>& levi,
	const Skup<Element, n2> desni) {
	Skup<Element, n1 + n2> unija;
	for (int i = 0; i < levi.mBrEl; ++i)
		unija.Dodaj(levi.mSkup[i]);
	for (int i = 0; i < desni.mBrEl; ++i)
		unija.Dodaj(desni.mSkup[i]);
	return unija;
}

#define BROJINT1 20
#define BROJINT2 10

int main() {
	
	Skup<int, BROJINT1> skupInt1;
	int izbaci = 41;

	for (int i = 0; i < BROJINT1; ++i) {
		int temp(rand() % 100 + i);
		skupInt1.Dodaj(temp);
	}

	cout << "\n\t\tSkupInt1:\n\n" << skupInt1 << '\n';

	Skup<int, BROJINT2> skupInt2;

	for (int i = 0; i < BROJINT2; ++i) {
		int temp(rand() % 100 + i);
		skupInt2.Dodaj(temp);
	}

	cout << "\n\t\tSkupInt2:\n\n" << skupInt2 << '\n';

	Skup<int, BROJINT1 + BROJINT2> skupIntUnija(skupInt1 + skupInt2);

	cout << "\n\t\SkupIntUnija:\n\n" << skupIntUnija << '\n';

	return 0;
}
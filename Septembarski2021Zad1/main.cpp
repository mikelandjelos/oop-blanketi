#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

enum Izuzetak {
	errDodavanje,
	errUklanjanje,
};

class Lezaljka {
protected:
	unsigned mID;
	double mCenaLezaljke;
	double mCenaHranaPice;
public:
	Lezaljka(unsigned id, double cenaIzn) 
		: mID(id), mCenaLezaljke(cenaIzn),
		mCenaHranaPice(0) {}
	virtual ~Lezaljka() {}
	inline unsigned GetID() { return mID; }
	inline double GetCenaLezaljke() { return mCenaLezaljke; }
	inline double GetCenaHranaPice() { return mCenaHranaPice; }
	Lezaljka& operator+=(double novaNarudzbina) {
		mCenaHranaPice += novaNarudzbina;
		return *this;
	}
	virtual ostream& Print(ostream& izlTok) {
		return izlTok << "ID: " << mID << '\n'
			<< "Cena lezaljke: " << mCenaLezaljke << '\n'
			<< "Cena narucene hrane/pica: " << mCenaHranaPice << '\n';
	}
};

class LezaljkaPopust : public Lezaljka {
private:
	double mPopust;
	double mCenaPriIzn;
public:
	LezaljkaPopust(double popust, unsigned id,
		double cenaIzn, double cenaHranaPice)
		: Lezaljka(id, cenaIzn), mPopust(popust),
		mCenaPriIzn((popust / 100) * cenaHranaPice) {
		mCenaHranaPice = cenaHranaPice;
	}
	virtual ~LezaljkaPopust() {}
	ostream& Print(ostream& izlTok) override {
		return Lezaljka::Print(izlTok) <<
			"Popust: " << mPopust << '\n'
			<< "Cena hrane/pica pri iznajmljivanju: "
			<< mCenaPriIzn << '\n';
	}
};

class Restoran {
private:
	Lezaljka** mLezaljke;
	int mBrLez, mBrIznLez;
public:
	Restoran(int brLez) 
		: mBrLez(brLez), mBrIznLez(0) {
		mLezaljke = new Lezaljka * [brLez];
		for (int i = 0; i < brLez; ++i)
			mLezaljke[i] = nullptr;
	}
	virtual ~Restoran() {
		for (int i = 0; i < mBrIznLez; ++i)
			delete mLezaljke[i];
		delete[] mLezaljke;
	}
	void Dodaj(Lezaljka* lez) {
		if (mBrIznLez == mBrLez ||
			lez == nullptr)
			throw Izuzetak::errDodavanje;
		bool flag = true;
		for (int i = 0; i < mBrIznLez; ++i)
			if (lez->GetID() == mLezaljke[i]->GetID()) {
				flag = false;
				break;
			}
		if (flag)
			mLezaljke[mBrIznLez++] = lez,
			lez = nullptr;
	}
	bool Ukloni(unsigned id) {
		if (mBrIznLez == 0)
			throw Izuzetak::errUklanjanje;
		int indeks = 0;
		for (; indeks < mBrIznLez; ++indeks)
			if (mLezaljke[indeks]->GetID() == id)
				break;
		if (indeks == mBrIznLez)
			return false; // lezaljka sa datom oznakom nije nadjena
		// ako je pak nadjena idemo na njeno brisanje iz niza
		delete mLezaljke[indeks],
			mLezaljke[indeks] = nullptr;
		for (int i = indeks; i < mBrIznLez - 1; ++i)
			mLezaljke[i] = mLezaljke[i + 1];
		mLezaljke[--mBrIznLez] = nullptr;
		return true;
	}
	bool Naruci(unsigned id, double novaNarudzbina) {
		int indeks = 0;
		for (; indeks < mBrIznLez; ++indeks)
			if (mLezaljke[indeks]->GetID() == id)
				break;
		if (indeks == mBrIznLez)
			return false; // lezaljka sa datom oznakom nije nadjena
		*mLezaljke[indeks] += novaNarudzbina;
		return true;
	}
	double Zarada() {
		double zarada = 0;
		for (int i = 0; i < mBrIznLez; ++i)
			zarada += mLezaljke[i]->GetCenaHranaPice()
			+ mLezaljke[i]->GetCenaLezaljke();
		return zarada;
	}
	double ZaradaIznajmljivanje() {
		double zarada = 0;
		for (int i = 0; i < mBrIznLez; ++i)
			zarada += mLezaljke[i]->GetCenaLezaljke();
		return zarada;
	}
	friend ostream& operator<<(ostream& izlTok, const Restoran& rest) {
		for (int i = 0; i < rest.mBrIznLez; ++i)
			rest.mLezaljke[i]->Print(izlTok);
		return izlTok;
	}
};

#define BROJLEZALJKI 6

int main() {
	
	Restoran restoran(BROJLEZALJKI);

	Lezaljka* temp[6] = {
		new Lezaljka(1, 20),
		new Lezaljka(2, 30),
		new Lezaljka(3, 15),
		new LezaljkaPopust(20, 4, 20, 20),
		new LezaljkaPopust(20, 5, 30, 30),
		new LezaljkaPopust(20, 6, 15, 45)
	};

	for (int i = 0; i < BROJLEZALJKI; ++i)
		restoran.Dodaj(temp[i]),
		restoran.Naruci(i, 15);

	cout << restoran << "\n\n";

	restoran.Ukloni(4);

	cout << restoran << '\n';

	cout << "Ukupna zarada: " << restoran.Zarada()
		<< "\nZaradjeno na lezaljkama: " << restoran.ZaradaIznajmljivanje()
		<< endl;

	return 0;
}
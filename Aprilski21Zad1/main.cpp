#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstring>
#include <limits>

using namespace std;

#define BROJF 12
#define BROJR 5

class Proizvod {
protected:
	string mBarKod, mTip;
public:
	// konstruktor
	Proizvod(string barKod, string tip) {
		mBarKod = barKod;
		mTip = tip;
	}
	// virtuelni destruktor
	virtual ~Proizvod() {
		mBarKod.clear();
		mTip.clear();
	}
	// izlazni tok podataka (u svrhu upisa u fajl
	// koji ce predstavljati izvestaj za menadzera)
	ostream& Ispis(ostream& izlTok) {
		return izlTok << "Tip: " << mTip
			<< ", Barkod: " << mBarKod << '\n';
	}
	virtual bool Defektnost() = 0;
};

class Kondenzator : public Proizvod {
private:
	double mKapacitivnost;
public:
	Kondenzator(double kapacitivnost, string barKod) 
		: Proizvod(barKod, "Kondenzator")
		, mKapacitivnost(kapacitivnost) {}
	// nije potrebna dodatna funkcija destruktora
	// zato sto nemamo nova polja koja referenciraju
	// dinamicku zonu memorije, pa telo mozemo ostaviti
	// praznim
	virtual ~Kondenzator() {}
	bool Defektnost() override {
		if (mKapacitivnost == 0)
			return true; // defektan
		return false; // nije defektan
	}
};

class Otpornik : public Proizvod {
private:
	double mOtpornost, mMaxTemp,
		mMinTemp;
public:
	Otpornik(double otpornost, double maxTemp,
		double minTemp, string barKod)
		: Proizvod(barKod, "Otpornik")
		, mOtpornost(otpornost), mMaxTemp(maxTemp), mMinTemp(minTemp) {}
	// -||-
	virtual ~Otpornik() {}
	bool Defektnost() override {
		if (mOtpornost == 0 
			|| mOtpornost == std::numeric_limits<double>::infinity())
			return true; // defektan
		return false; // nije defektan
	}
};

class PokretnaTraka {
private:
	Proizvod** mTraka;
	int mKapacitet, mBrPr;
public:
	// mBrPr odredjuje trenutni broj proizvoda
	// na traci, a mKapacitet ukupan broj proizvoda
	// koji moze da stane na traku
	PokretnaTraka(int kapacitet) 
		: mKapacitet(kapacitet), mBrPr(0) {
		mTraka = new Proizvod * [kapacitet];
		for (int i = 0; i < mKapacitet; ++i)
			mTraka[i] = nullptr;
	}
	virtual ~PokretnaTraka() {
		for (int i = 0; i < mBrPr; ++i)
			if (mTraka[i] != nullptr)
				delete mTraka[i];
		delete[] mTraka, mTraka = nullptr;
	}
	void PovecajKapacitet(int brMesta) {
		Proizvod** pom = new Proizvod * [mKapacitet += brMesta];
		for (int i = 0; i < mBrPr; ++i)
			pom[i] = mTraka[i];
		delete[] mTraka;
		mTraka = pom;
	}
	void Ukloni(ostream& izlTok) {
		if (mBrPr == 0) {
			std::cerr << "Nemoguce uklanjanje, traka je prazna.\n";
			exit(1);
		}
		/* 
			moramo da radimo delete pri uklanjanju 
			zato sto radimo sa pointerima 
		*/
		mTraka[0]->Ispis(izlTok);
		delete mTraka[0], mTraka[0] = nullptr;
		for (int i = 0; i < mBrPr - 1; ++i)
			mTraka[i] = mTraka[i + 1];
		mTraka[mBrPr--] = nullptr;
	}
	void Dodaj(Proizvod* proizv) {
		if (mBrPr == mKapacitet) {
			std::cerr << "Nemoguce dodavanje, traka je prepunjena.\n";
			exit(1);
		}
		mTraka[mBrPr++] = proizv;
	}
	int PraznaMesta() {
		return mKapacitet - mBrPr;
	}
	int BrDefektnih() {
		int br = 0;
		for (int i = 0; i < mBrPr; ++i)
			if (mTraka[i]->Defektnost())
				br++;
		return br;
	}
};

int main() {

	PokretnaTraka traka1(20);
	string barkod = "1a52b239gh";
	int otpornost = 133;
	double kapacitivnost = 0.00000002;
	ofstream outFajl("izlaz.txt");

	for (int i = 0; i < BROJR; i++) {
		Proizvod* temp = new Otpornik(otpornost += 30, -6.2, 78.3, barkod);
		barkod[9] += 2;
		traka1.Dodaj(temp);
	}

	for (int i = 0; i < BROJF; i++) {
		Proizvod* temp = new Kondenzator(kapacitivnost -= 0.000000001, barkod);
		barkod[9] += 2;
		traka1.Dodaj(temp);
	}

	cout << "Broj defektnih na traci: " << traka1.BrDefektnih()
		<< ", Prazna mesta: " << traka1.PraznaMesta() << '\n';

	traka1.PovecajKapacitet(5);

	for (int i = 0; i < BROJF + BROJR; ++i) {
		traka1.Ukloni(outFajl);
	}

	cout << "Broj defektnih na traci: " << traka1.BrDefektnih()
		<< ", Prazna mesta: " << traka1.PraznaMesta() << '\n';
	 
	outFajl.close();
	
	return 0;
}
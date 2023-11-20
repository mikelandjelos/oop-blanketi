#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

enum TipVoznogParka {
	Automobilski,	// 0
	Kamionski		// 1
};

using namespace std;

class Vozilo {
protected:
	char* mNaziv;
	char* mRegistracija;
	double mMaxBrzina, mPotrosnja;
public:
	Vozilo() {
		mNaziv = mRegistracija = NULL,
			mMaxBrzina = mPotrosnja = 0;
	}
	Vozilo(const char* naziv, const char* registracija,
		double maxBrzina, double potrosnja) 
		: mMaxBrzina(maxBrzina), mPotrosnja(potrosnja) {
		mNaziv = (naziv != NULL) ? 
			new char[strlen(naziv) + 1] : NULL;
		strcpy(mNaziv, naziv);
		mRegistracija = (registracija != NULL) ?
			new char[strlen(registracija) + 1] : NULL;
		strcpy(mRegistracija, registracija);
	}
	virtual ~Vozilo() {
		if (mNaziv != NULL)
			delete[] mNaziv,
			mNaziv = NULL;
		if (mRegistracija != NULL)
			delete[] mRegistracija,
			mRegistracija = NULL;
	}
	virtual ostream& Upis(ostream& izlTok) {
		izlTok.write(mNaziv, strlen(mNaziv));
		izlTok << '\n';
		izlTok.write(mRegistracija, strlen(mRegistracija));
		return izlTok << '\n' << mMaxBrzina
			<< '\n' << mPotrosnja;
	}
	virtual istream& Citanje(istream& ulTok) {
		// ovo sa staticki alociranim pomocnim stringom
		// je skakljivo resenje (nije potpuno fleksibilno)
		//  ali dosta olaksava posao
		char pom[80];
		ulTok.getline(pom, 79);
		if (mNaziv != NULL)
			delete[] mNaziv;
		mNaziv = (pom != NULL) ?
			new char[strlen(pom) + 1] : NULL;
		strcpy(mNaziv, pom);
		ulTok.getline(pom, 79);
		if (mRegistracija != NULL)
			delete[] mRegistracija;
		mRegistracija = (pom != NULL) ?
			new char[strlen(pom) + 1] : NULL;
		strcpy(mRegistracija, pom);
		return ulTok >> mMaxBrzina >> mPotrosnja;
	}
};

class Automobil : public Vozilo {
private:
	unsigned mBrPutnika;
public:
	Automobil()
		: Vozilo(), mBrPutnika(0) {}
	Automobil(const char* naziv, const char* registracija,
		double maxBrzina, double potrosnja, unsigned brPutnika)
		: Vozilo(naziv, registracija, maxBrzina, potrosnja),
		mBrPutnika(brPutnika) {}
	virtual ~Automobil() {}
	ostream& Upis(ostream& izlTok) override {
		return Vozilo::Upis(izlTok) << '\n' << mBrPutnika;
	}
	istream& Citanje(istream& ulTok) override {
		return Vozilo::Citanje(ulTok) >> mBrPutnika;
	}
};

class Kamion : public Vozilo {
private:
	double mNosivost;
public:
	Kamion() : Vozilo(), mNosivost(0) {}
	Kamion(const char* naziv, const char* registracija,
		double maxBrzina, double potrosnja, double nosivost)
		: Vozilo(naziv, registracija, maxBrzina, potrosnja),
		mNosivost(nosivost) {}
	virtual ~Kamion() {}
	ostream& Upis(ostream& izlTok) override {
		return Vozilo::Upis(izlTok) << '\n' << mNosivost;
	}
	istream& Citanje(istream& ulTok) override {
		return Vozilo::Citanje(ulTok) >> mNosivost;
	}
};

class VozniPark {
private:
	Vozilo** mNiz;
	int mMaxBrVoz, mBrVoz;
	TipVoznogParka mTip;
	void TeloDestruktora() {
		for (int i = 0; i < mBrVoz; ++i)
			if (mNiz[i] != nullptr)
				delete mNiz[i];
		delete[] mNiz;
	}
public:
	VozniPark(int maxBrVoz, TipVoznogParka tip)
		: mMaxBrVoz(maxBrVoz), mTip(tip), mBrVoz(0) {
		mNiz = new Vozilo * [maxBrVoz];
		for (int i = 0; i < mMaxBrVoz; ++i)
			mNiz[i] = nullptr;
	}
	virtual ~VozniPark() {
		TeloDestruktora();
	}
	bool Dodaj(Vozilo* vozilo) {
		if (vozilo == nullptr)
			return false;
		if (mBrVoz == mMaxBrVoz) {
			mMaxBrVoz *= 1.5;
			Vozilo** pom = new Vozilo * [mMaxBrVoz];
			for (int i = 0; i < mBrVoz; ++i)
				pom[i] = mNiz[i];
			for (int i = mBrVoz; i < mMaxBrVoz; ++i)
				pom[i] = nullptr;
			delete[] mNiz;
			mNiz = pom;
		}
		mNiz[mBrVoz++] = vozilo;
		vozilo = nullptr;
		return true;
	}
	bool Upis(char const* putanja) {
		ofstream outFajl(putanja);
		if (!outFajl.good()) {
			outFajl.close();
			return false;
		}
		outFajl << mMaxBrVoz
			<< '\n' << mBrVoz
			<< '\n' << mTip << '\n';
		for (int i = 0; i < mBrVoz; ++i)
			mNiz[i]->Upis(outFajl) << '\n';
		outFajl.close();
		return true;
	}
	bool Citanje(char const* putanja) {
		ifstream ulTok(putanja);
		if (!ulTok.good()) {
			ulTok.close();
			return false;
		}
		TeloDestruktora();
		int tip;
		ulTok >> mMaxBrVoz >> mBrVoz
			>> tip;
		ulTok.ignore();
		mTip = (TipVoznogParka)tip;
		mNiz = new Vozilo * [mMaxBrVoz];
		for (int i = 0; i < mBrVoz; ++i) {
			switch (mTip) {
			case TipVoznogParka::Automobilski:
				mNiz[i] = new Automobil;
				break;
			case TipVoznogParka::Kamionski:
				mNiz[i] = new Kamion;
				break;
			}
			mNiz[i]->Citanje(ulTok),
			ulTok.ignore();
		}
		for (int i = mBrVoz; i < mMaxBrVoz; ++i)
			mNiz[i] = nullptr;
		return true;
	}
};

#define BROJVOZILA 5

int main() {

	VozniPark Automobili(BROJVOZILA, TipVoznogParka::Automobilski);
	ifstream inFajl("Vozila.txt");

	Vozilo* temp = 
		new Automobil("BMW", "BG-1024-AC", 160, 0.9, 6);

	Automobili.Dodaj(temp);

	temp = new Vozilo;

	temp->Citanje(inFajl);

	Automobili.Dodaj(temp);

	Automobili.Upis("izlaz.txt");

	VozniPark Automobili_cpy(BROJVOZILA, TipVoznogParka::Automobilski);

	Automobili_cpy.Citanje("izlaz.txt");
	Automobili_cpy.Upis("isprobavanje.txt");

	inFajl.close();

	return 0;
}
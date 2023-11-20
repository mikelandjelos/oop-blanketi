#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

class Vozilo {
protected:
	char * mNaziv, * mRegistracija;
	double mMaxBrzina, mPotrosnja;
public:
	Vozilo() {}
	inline double MaxBrzina() { return mMaxBrzina; }
	inline double Potrosnja() { return mPotrosnja; }
	Vozilo(const char* naziv, const char* registracija,
		double maxBrzina, double potrosnja) 
		: mMaxBrzina(maxBrzina), mPotrosnja(potrosnja) {
		mNaziv = (naziv != NULL) ? new char[strlen(naziv) + 1] : NULL;
		if (naziv != NULL)
			strcpy(mNaziv, naziv);
		mRegistracija = (registracija != NULL) ? 
			new char[strlen(registracija) + 1] : NULL;
		if (registracija != NULL)
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
	virtual ostream& Upisi(ostream& izlTok) {
		izlTok << "Naziv: ";
		izlTok.write(mNaziv, strlen(mNaziv));
		izlTok << "\nRegistracija: ";
		izlTok.write(mRegistracija, strlen(mRegistracija));
		return izlTok << "\nMaksimalna brzina: " << mMaxBrzina << '\n'
			<< "Potrosnja: " << mPotrosnja;
	}
	virtual istream& Ucitaj(istream& ulTok) {
		if (mNaziv != NULL)
			delete[] mNaziv;
		mNaziv = new char[30];
		ulTok.getline(mNaziv, '\n');
		if (mRegistracija != NULL)
			delete[] mRegistracija;
		mRegistracija = new char[30];
		ulTok.getline(mRegistracija, '\n');
		return ulTok >> mMaxBrzina >> mPotrosnja;
	}
	/* ova funkcija zapravo uvodi dobru problematiku */
	virtual Vozilo* Uporedi(Vozilo* prm) = 0;
};

class Automobil : public Vozilo {
private:
	unsigned mMaxBrPutnika;
public:
	Automobil() {}
	Automobil(const char* naziv, const char* registracija,
		double maxBrzina, double potrosnja,
		unsigned maxBrPutnika) 
		: Vozilo(naziv, registracija,
			maxBrzina, potrosnja), mMaxBrPutnika(maxBrPutnika) {}
	virtual ~Automobil() {}
	ostream& Upisi(ostream& izlTok) override {
		Vozilo::Upisi(izlTok);
		return izlTok << "\nMaksimalni broj putnika : " << mMaxBrPutnika;
	}
	istream& Ucitaj(istream& ulTok) override {
		Vozilo::Ucitaj(ulTok);
		return ulTok >> mMaxBrPutnika;
	}
	Automobil* Uporedi(Vozilo* prm) override {
		return (this->MaxBrzina() > prm->MaxBrzina()) ? this : (Automobil*)prm;
	}
};

class Kamion : public Vozilo {
private:
	double mNosivost;
public:
	Kamion() {}
	Kamion(const char* naziv, const char* registracija,
		double maxBrzina, double potrosnja,
		double nosivost)
		: Vozilo(naziv, registracija,
			maxBrzina, potrosnja), mNosivost(nosivost) {}
	virtual ~Kamion() {}
	ostream& Upisi(ostream& izlTok) override {
		Vozilo::Upisi(izlTok);
		return izlTok << "\nNosivost : " << mNosivost;
	}
	istream& Ucitaj(istream& ulTok) override {
		Vozilo::Ucitaj(ulTok);
		return ulTok >> mNosivost;
	}
	Kamion* Uporedi(Vozilo* prm) override {
		return (this->Potrosnja() > prm->Potrosnja()) ? this : (Kamion*)prm;
	}
};

class VozniPark {
private:
	/* itd itd itd */
public:
};

int main() {
	
	/*int brAuto, brKami;
	ifstream inFajlAuto("automobili.txt");
	ifstream inFajlKami("kamioni.txt");
	ofstream outFajlAuto("automobili_izlaz.txt");
	ofstream outFajlKami("kamioni_izlaz.txt");

	inFajlAuto >> brAuto,
		inFajlAuto.ignore();

	inFajlKami >> brKami,
		inFajlKami.ignore();

	Vozilo** automobili = new Vozilo * [brAuto];

	for (int i = 0; i < brAuto; ++i) {
		automobili[i] = new Automobil;
		automobili[i]->Ucitaj(inFajlAuto);
		inFajlAuto.ignore();
		automobili[i]->Upisi(outFajlAuto) << '\n';
	}

	for (int i = 0; i < brAuto; ++i)
		delete automobili[i];
	delete[] automobili;

	inFajlAuto.close(), inFajlKami.close(),
		outFajlAuto.close(), outFajlKami.close();*/

	return 0;
}
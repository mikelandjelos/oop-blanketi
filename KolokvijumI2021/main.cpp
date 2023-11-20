/*
	Kratka napomena:
	 - Sigurno ne bih radio ovaj zadatak ovoliko
	 detaljno na ispitu, zato sto mi je
	 trebalo jedno 1.5h da samo njega otkucam, ali je dobra
	 praksa sve sto je ovamo upotrebljeno (bar mislim),
	 tako da smatram da vredi da se pogleda;
*/

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

#define R 1000

enum class Izuzetak : unsigned char {
	errCistocaZlata = 69,	// 69
	errCistocaKamena,		// 70
	errKoefSlatkoce,		// 71
	errKoefKiselosti,		// 72
	errMasa,				// 73
	errBoja,				// 74
	errKoeficijenti,		// 75
	errDodavanje,			// 76
	errOtkazivanje,			// 77
};

class Predmet {
protected:
	double mMasa;
	virtual std::ostream& 
		Write(std::ostream& izlTok) const = 0;
public:
	// na ovaj nacin je operator<< 'virtuelizovan'
	// iako friend globalne funkcije ne mogu biti
	// virtuelne -> ovo je ucinjeno zbog kompaktnosti
	// zapisa i izmedju ostalog vece fleksibilnosti
	// ; Write je stavljena u protected
	// deo kako niko osim projektanta klase ne bi mogao da joj
	// pristupi, vec se od korisnika iskljucivo ocekuje upotreba
	// operatora << za postavljanje podataka na neki izl. tok
	Predmet() : mMasa(0) {}
	Predmet(double masa) : mMasa(masa) {
		if (masa < 0)
			throw Izuzetak::errMasa;
	}
	virtual ~Predmet() {}
	friend std::ostream& 
		operator<<(std::ostream& izlTok, const Predmet& predmet) {
		// zbog & je omogucen rad virtuelnog mehanizma
		return predmet.Write(izlTok);
	}
	// virtuelna metoda vraca vrednost
	virtual double Vrednost() const = 0;
};
 
class ZlatnaPoluga : public Predmet {
private:
	// krece se od 1 do 24 (24 <=> 100%)
	unsigned mStepenCistoce;
public:
	ZlatnaPoluga() 
		: Predmet(), mStepenCistoce(0) {}
	ZlatnaPoluga(double masa, unsigned stepenCistoce) 
		: Predmet(masa), mStepenCistoce(stepenCistoce) {
		if (stepenCistoce < 0 || stepenCistoce > 24)
			throw Izuzetak::errCistocaZlata;
	}
	virtual ~ZlatnaPoluga() {}
	std::ostream& 
		Write(std::ostream& izlTok) const override{
		return izlTok << "Masa: " << mMasa
			<< "\nStepen cistoce poluge: " << mStepenCistoce
			<< "\nVrednost: " << Vrednost();
	}
	double Vrednost() const override {
		return (mMasa / 24) * mStepenCistoce;
	}
};

class DragiKamen : public Predmet {
private:
	double mCistoca;
	char* mBoja;
	unsigned mRez;
public:
	DragiKamen() 
		: Predmet(), mCistoca(0), mBoja(NULL),
	mRez(0) {}
	DragiKamen(double masa, double cistoca, 
		const char* boja, unsigned rez) 
		: Predmet(masa), mCistoca(cistoca), mRez(rez) {
		if (cistoca < 0 || cistoca > 1)
			throw Izuzetak::errCistocaKamena;
		if (boja == NULL)
			throw Izuzetak::errBoja;
		mBoja = new char[strlen(boja) + 1];
		strcpy(mBoja, boja);
	}
	virtual ~DragiKamen() {
		if (mBoja != NULL)
			delete[] mBoja,
			mBoja = NULL;
	}
	std::ostream& 
		Write(std::ostream& izlTok) const override {
		izlTok << "Cistoca kamena: " << mCistoca 
			<< "\nBoja: ";
		izlTok.write(mBoja, strlen(mBoja));
		return izlTok << "\nRez (br. stranica): " << mRez
			<< "\nMasa: " << mMasa
			<< "\nVrednost: " << Vrednost();
	}
	double Vrednost() const override {
		return mMasa * mCistoca;
	}
};

class Jabuka : public Predmet {
private:
	double mKoefKis, mKoefSlat;
	char* mBoja;
public:
	Jabuka() 
		: Predmet(0), mKoefKis(0), 
		mKoefSlat(0), mBoja(NULL) {}
	Jabuka(double masa, double koefKis,
		double koefSlat, const char* boja) 
		: Predmet(masa), mKoefKis(koefKis),
	mKoefSlat(koefSlat) {
		if (koefKis < 0 || koefSlat > 1
			|| koefSlat < 0 || koefSlat > 1)
			throw Izuzetak::errKoeficijenti;
		if (boja == NULL)
			throw Izuzetak::errBoja;
		mBoja = new char[strlen(boja) + 1];
		strcpy(mBoja, boja);
	}
	virtual ~Jabuka() {
		if (mBoja != NULL)
			delete[] mBoja,
			mBoja = NULL;
	}
	std::ostream& 
		Write(std::ostream& izlTok) const override {
		izlTok << "Masa: " << mMasa
			<< "\nKoef. kiselosti: " << mKoefKis
			<< "\nKoef. slatkoce: " << mKoefSlat << '\n';
		izlTok.write(mBoja, strlen(mBoja));
		return izlTok << "\nVrednost: " << Vrednost();
	}
	double Vrednost() const override {
		return (mKoefKis < 0.5 || mKoefSlat < 0.5) ? 0
			: mMasa * mKoefKis * mKoefSlat * R;
	}
};

class Ranac {
private:
	Predmet** mRanac;
	int mKapacitet, mTrenBr;
public:
	Ranac(int kapacitet) 
		: mKapacitet(kapacitet), mTrenBr(0) {
		mRanac = new Predmet * [kapacitet];
		for (int i = 0; i < kapacitet; ++i)
			mRanac[i] = nullptr;
	}
	virtual ~Ranac() {
		for (int i = 0; i < mTrenBr; ++i)
			if (mRanac[i] != nullptr)
				delete mRanac[i];
		delete[] mRanac;
	}
	bool Dodaj(Predmet** pPredmet) {
		if (mTrenBr == mKapacitet)
			throw Izuzetak::errDodavanje;
		if (*pPredmet == nullptr)
			return false;
		mRanac[mTrenBr++] = *pPredmet;
		// ovo je isprobavanje, ne bih tako na ispitu
		//  -> secemo vezu
		// originalnog pointera
		*pPredmet = nullptr;
		return true;
	}
	void Izbaci(int indeks) {
		if (mTrenBr == 0 ||
			indeks < 0 || indeks >= mTrenBr)
			throw Izuzetak::errOtkazivanje;
		delete[] mRanac[indeks],
			mRanac[indeks] = nullptr;
		for (int i = indeks; i < mTrenBr - 1; ++i)
			mRanac[i] = mRanac[i + 1];
		mRanac[--mTrenBr] = nullptr;
	}
	friend std::ostream& 
		operator<<(std::ostream& izlTok, const Ranac& ranac) {
		izlTok << "Kapacitet ranca: " << ranac.mKapacitet
			<< "\nTrenutni broj predmeta u rancu: "
			<< ranac.mTrenBr << '\n';
		for (int i = 0; i < ranac.mTrenBr; ++i)
			izlTok << "Predmet " << i + 1 << ":\n"
			<<  * ranac.mRanac[i] << '\n';
		return izlTok;
	}
};

#define BRPREDMETA 5

int main() {
	
	try {

		Ranac ranac(BRPREDMETA);

		// ova linija koda je hendlovana
		// tako da povratna vrednost maina bude
		// -78, sto zapravo signalizira da se javila
		// greska u izbacivanju predmeta iz ranca
		// ranac.Izbaci(-1);

		Predmet* temp = new ZlatnaPoluga(10, 15);
		ranac.Dodaj(&temp);

		temp = new DragiKamen(0.1, 0.78, "crvena", 6);
		ranac.Dodaj(&temp);

		temp = new Jabuka(0.05, 0.56, 0.8, "zelena");
		ranac.Dodaj(&temp);

		std::cout << "\n\n\tRanac pre izbacivanja\n\n" << ranac;

		ranac.Izbaci(0);

		std::cout << "\n\n\tRanac nakon izbacivanja\n\n" << ranac;
	
	}
	catch (Izuzetak ex) {
		switch (ex) {
		case Izuzetak::errBoja:
			return -((int)Izuzetak::errBoja + 1);
		case Izuzetak::errCistocaKamena:
			return -((int)Izuzetak::errCistocaKamena + 1);
		case Izuzetak::errCistocaZlata:
			return -((int)Izuzetak::errCistocaZlata + 1);
		case Izuzetak::errKoeficijenti:
			return -((int)Izuzetak::errKoeficijenti + 1);
		case Izuzetak::errKoefKiselosti:
			return -((int)Izuzetak::errKoefKiselosti + 1);
		case Izuzetak::errKoefSlatkoce:
			return -((int)Izuzetak::errKoefSlatkoce + 1);
		case Izuzetak::errMasa:
			return -((int)Izuzetak::errMasa + 1);
		case Izuzetak::errOtkazivanje:
			return -((int)Izuzetak::errOtkazivanje + 1);
		}
	}

	return 0;
}
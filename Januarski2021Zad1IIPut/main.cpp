#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>

class Domacinstvo {
protected:
	int mId; // jedinstveni broj
	double mPovrsina; // povrsina pod objektima u m^2
	double mUtrosenaEn; // utrosena energija u kWh
public:
	Domacinstvo(int id, double povrsina, double utrosenaEn) 
		: mId(id), mPovrsina(povrsina), mUtrosenaEn(utrosenaEn) {}
	virtual ~Domacinstvo() {}
	inline int getId() const { return mId; }
	inline double getPovrsina() const { return mPovrsina; }
	inline double getUtrosenaEn() const { return mUtrosenaEn; }
	virtual double SvotaNovca(double cenaPokWh) {
		return cenaPokWh * mUtrosenaEn;
	}
	virtual std::ostream& Ispis(std::ostream& izlTok) const {
		return izlTok << "Id: " << mId
			<< "\nPovrsina pod objektima [m^2]: " << mPovrsina
			<< "\nUtrosena energija [kWh]: " << mUtrosenaEn;
	}
};

class SolarnoDomacinstvo : public Domacinstvo {
private:
	double mUstedjenaEn; // ustedjena energija u kWh
	double mKoefPodsticaja;
	double mCenaOdrzavanja;
public:
	SolarnoDomacinstvo(double ustedjenaEn, double koefPodsticaja,
		double cenaOdrzavanja, int id, double povrsina, double utrosenaEn) 
		: Domacinstvo(id, povrsina, utrosenaEn), mUstedjenaEn(ustedjenaEn),
		mKoefPodsticaja(koefPodsticaja), mCenaOdrzavanja(cenaOdrzavanja) {}
	virtual ~SolarnoDomacinstvo() {}
	double SvotaNovca(double cenaPokWh) override {
		return Domacinstvo::SvotaNovca(cenaPokWh) 
			- mKoefPodsticaja * mUstedjenaEn * cenaPokWh;
	}
	std::ostream& Ispis(std::ostream& izlTok) const override {
		return Domacinstvo::Ispis(izlTok)
			<< "\nUstedjena energina [kWh]: " << mUstedjenaEn
			<< "\nKoef. podsticaja: " << mKoefPodsticaja
			<< "\nCena odrzavanja: " << mCenaOdrzavanja;
	}
};

class Preduzece {
private:
	Domacinstvo** mDomacinstva;
	int mMax, mBr;
	// mMax - maks. moguci broj domacinstava
	// mBr - trenutni broj domacinstava
public:
	Preduzece(int max)
		: mDomacinstva(new Domacinstvo* [max]),
		mMax(max), mBr(0) {
		for (int i = 0; i < max; ++i)
			mDomacinstva[i] = nullptr;
	}
	virtual ~Preduzece() {
		for (int i = 0; i < mBr; ++i)
			if (mDomacinstva[i] != nullptr)
				delete mDomacinstva[i];
		delete[] mDomacinstva;
	}
	void Dodaj(Domacinstvo** pdom) {
		if (mBr == mMax || *pdom == nullptr)
			std::cerr << "\nGreska pri dodavanju domacinstva "
			"- nema vise mesta!\n",
			std::exit(-1);
		for (int i = 0; i < mBr; ++i)
			if (mDomacinstva[i]->getId() == (*pdom)->getId())
				std::cerr << "\nGreska pri dodavanju domacinstva - "
				"takvo domacinstvo vec postoji!\n",
				std::exit(-2);
		mDomacinstva[mBr++] = *pdom;
		*pdom = nullptr;
	}
	bool Ukloni(int id) {
		int indeks = -1;
		if (mBr == 0)
			return false;
		for (int i = 0; i < mBr; ++i)
			if (id == mDomacinstva[i]->getId()) {
				indeks = i;
				break;
			}
		if (indeks == -1)
			return false;
		delete mDomacinstva[indeks];
		for (int i = indeks; i < mBr - 1; ++i)
			mDomacinstva[i] = mDomacinstva[i + 1];
		mDomacinstva[--mBr] = nullptr;
		return true;
	}
	friend std::ostream& operator<<(std::ostream& izlTok, 
		const Preduzece& preduzece) {
		izlTok << "Broj domacinstava: " << preduzece.mBr << '\n';
		for (int i = 0; i < preduzece.mBr; ++i)
			preduzece.mDomacinstva[i]->Ispis(izlTok) << '\n';
		return izlTok;
	}
	double ProsecnaPotrosnja() {
		double ukEn = 0, ukPovrs = 0;
		int i = 0;
		for (; i < mBr; ++i)
			ukEn += mDomacinstva[i]->getUtrosenaEn(),
			ukPovrs += mDomacinstva[i]->getPovrsina();
		return ukEn / i / ukPovrs;
	}
};

#define BRDOM 5

int main() {
	
	Preduzece jkpPirot(BRDOM);
	int id = 1;
	std::vector<Domacinstvo*> domacinsva = {
		new Domacinstvo(id, 2.36, 150.65),
		new Domacinstvo(5, 2.37, 150.65),
		new SolarnoDomacinstvo(2.36, 150.68, 450.7, 17, 1.2, 1000),
		new SolarnoDomacinstvo(2.36, 150.69, 450.7, 13, 1.2, 1000),
		new SolarnoDomacinstvo(2.36, 150.63, 450.7, 2, 1.2, 1000)
	};

	for (int i = 0; i < BRDOM; ++i)
		jkpPirot.Dodaj(&domacinsva[i]);

	std::cout << "\n\t\tPre uklanjanja:\n\n" << jkpPirot
		<< "\n\tProsecna potrosnja: " << jkpPirot.ProsecnaPotrosnja();

	jkpPirot.Ukloni(id);

	std::cout << "\n\t\tNakon uklanjanja:\n\n" << jkpPirot
		<< "\n\tProsecna potrosnja: " << jkpPirot.ProsecnaPotrosnja();

	return 0;
}
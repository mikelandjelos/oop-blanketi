#include <iostream>
#include <fstream>
#include <string>

#define LETOVANJA 3
#define OBILASCI 2

using namespace std;

class Putovanje {
protected:
	string mNazivDestinacije;
	unsigned mBrojNocenja;
	double mCenaNocenja;
public:
	Putovanje(string nazivDestinacije, unsigned brojNocenja,
		double cenaNocenja) 
		: mNazivDestinacije(nazivDestinacije), 
		mBrojNocenja(brojNocenja), mCenaNocenja(cenaNocenja) {}
	virtual ~Putovanje() {
		mNazivDestinacije.clear();
	}
	// ciste virtuelne
	virtual double Zanimljivost() = 0;
	virtual double UkupnaCena() = 0;
	friend ostream& operator<<(ostream& izlTok, Putovanje& putovanje) {
		return izlTok << "Podaci o putovanju:\n"
			<< " - Naziv destinacije - " << putovanje.mNazivDestinacije << '\n'
			<< " - Broj nocenja - " << putovanje.mBrojNocenja << '\n'
			<< " - Cena nocenja - " << putovanje.mCenaNocenja << '\n'
			<< " - Zanimljivost putovanja - " << putovanje.Zanimljivost() << '\n';
	}
	// ukoliko je potrebno izmeniti cenu nocenja
	inline double& GetCenaNocenja() { return mCenaNocenja; }
};
class Letovanje : public Putovanje {
private:
	string mNazivMora;
	double mProsecnaTemperatura;
	unsigned mBrojPlaza;
public:
	Letovanje(string nazivDestinacije, unsigned brojNocenja,
		double cenaNocenja, string nazivMora, 
		double prosecnaTemp, unsigned brPlaza) 
		: Putovanje(nazivDestinacije, brojNocenja, cenaNocenja),
	mNazivMora(nazivMora), mProsecnaTemperatura(prosecnaTemp),
	mBrojPlaza(brPlaza) {}
	virtual ~Letovanje() {
		mNazivMora.clear();
	}
	double Zanimljivost() override {
		return mBrojPlaza * ((mProsecnaTemperatura > 26) ? 0.8 : 0.6);
	}
	double UkupnaCena() override {
		return mBrojNocenja * mCenaNocenja;
	}
};

class ObilazakGrada : public Putovanje {
private:
	unsigned mBrojAtrakcija;
	double mCenaPrevoza;
public:
	ObilazakGrada(string nazivDestinacije, unsigned brojNocenja,
		double cenaNocenja, unsigned brojAtrakcija,
		double cenaPrevoza)
		: Putovanje(nazivDestinacije, brojNocenja, cenaNocenja),
		mBrojAtrakcija(brojAtrakcija), mCenaPrevoza(cenaPrevoza) {}
	virtual ~ObilazakGrada() {}
	double Zanimljivost() override {
		return (double)mBrojAtrakcija;
	}
	double UkupnaCena() override {
		return mBrojNocenja * mCenaNocenja + mCenaPrevoza;
	}
};

class TuristickaOrganizacija {
private:
	Putovanje** mPonuda;
	int mBrojPonuda;
	int mKapacitet;
public:
	TuristickaOrganizacija(int kapacitet) 
		: mBrojPonuda(0), mKapacitet(kapacitet) {
		mPonuda = new Putovanje * [kapacitet];
		for (int i = 0; i < kapacitet; ++i)
			mPonuda[i] = nullptr;
	}
	virtual ~TuristickaOrganizacija() {
		for (int i = 0; i < mBrojPonuda; ++i)
			if (mPonuda[i] != nullptr)
				delete mPonuda[i];
		delete[] mPonuda;
	}
	bool Dodaj(Putovanje* putovanje) {
		if (mKapacitet == mBrojPonuda)
			return false;
		int indeks = 0;
		while (indeks < mBrojPonuda) {
			if (mPonuda[indeks]->UkupnaCena() < putovanje->UkupnaCena())
				break;
			indeks++;
		}
		for (int i = mBrojPonuda; i > indeks; --i)
			mPonuda[i] = mPonuda[i - 1];
		mPonuda[indeks] = putovanje;
		putovanje = nullptr;
		mBrojPonuda++;
		return true;
	}
	bool Otkazi(int indeks) {
		if (mBrojPonuda == 0 ||
			indeks < 0 || indeks >= mBrojPonuda)
			return false;
		delete mPonuda[indeks], 
			mPonuda[indeks] = nullptr;
		for (int i = indeks; i < mBrojPonuda - 1; ++i)
			mPonuda[i] = mPonuda[i + 1];
		mBrojPonuda--;
		return true;
	}
	void Najzanimljivije() {
		int imax = 0;
		for (int i = 0; i < mBrojPonuda; ++i)
			if (mPonuda[imax]->Zanimljivost() < mPonuda[i]->Zanimljivost())
				imax = i;
		std::cout << *mPonuda[imax];
	}
	double AvgVrednost() {
		double pom = 0;
		int i = 0;
		for (; i < mBrojPonuda; ++i)
			pom += mPonuda[i]->UkupnaCena();
		return pom / i;
	}
	bool IzmeniCenu(int indeks, double novaCena) {
		if (mBrojPonuda == 0 ||
			indeks < 0 || indeks >= mBrojPonuda)
			return false;
		mPonuda[indeks]->GetCenaNocenja() = novaCena;
		return true;
	}
	friend ostream& operator<<(ostream& izlTok, const TuristickaOrganizacija& to) {
		for (int i = 0; i < to.mBrojPonuda; ++i)
			izlTok << *to.mPonuda[i] << '\n';
		return izlTok;
	}
};

int main() {

	TuristickaOrganizacija TourOrg(LETOVANJA + OBILASCI);
	ofstream outPonude("IzvestajAgencije.txt");

	for (int i = 0; i < LETOVANJA; ++i) {
		Putovanje* let = new Letovanje("Nei Pori", 10, 50, "Egejsko", 27.6, 45);
		TourOrg.Dodaj(let);
	}

	for (int i = 0; i < OBILASCI; ++i) {
		Putovanje* obilazak = new ObilazakGrada("Venezia", 1, 80, 20, 100);
		TourOrg.Dodaj(obilazak);
	}

	outPonude << "\n\t\tPodaci o turistickoj organizaciji\n\n" << TourOrg;

	outPonude.close();

	cout << "\n\t\tPodaci o najzanimljivijem putovanju\n\n";

	TourOrg.Najzanimljivije();

	TourOrg.Otkazi(3);
	TourOrg.IzmeniCenu(3, 37);
	cout << "\n\t\tPodaci o turistickoj organizaciji\n\n" << TourOrg
		<< "Prosecna cena putovanja: " << TourOrg.AvgVrednost() << '\n';
	
	return 0;
}
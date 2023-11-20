#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdlib>

using std::cout;
using std::endl;
using std::ostream;
using std::ofstream;

class Putovanje {
protected:
	char* mNaziv;
	int mBrNocenja;
	double mCenaNocenja;
	virtual ostream& Write(ostream& izlTok) const {
		// funkcija ostream::write(const char*, streamsize); ne
		// append-uje znak '\n' na kraj string-a koji 'stampa'
		// (tj. koji salje na izlazni tok podataka);
		izlTok << "Naziv putovanja: ";
		izlTok.write(mNaziv, strlen(mNaziv));
		return izlTok << "\nBroj nocenja: " << mBrNocenja 
			<< "\nCena jednog nocenja: " << mCenaNocenja
			<< "\nCena putovanja: " << Cena()
			<< "\nZanimljivost putovanja: " << Zanimljivost();
	}
public:
	Putovanje(const char* naziv, int brNocenja, double cenaNocenja) 
		: mBrNocenja(brNocenja), mCenaNocenja(cenaNocenja) {
		mNaziv = new char[strlen(naziv) + 1];
		strcpy(mNaziv, naziv);
	}
	virtual ~Putovanje() {
		if (mNaziv != NULL)
			delete[] mNaziv,
			mNaziv = NULL;
	}
	// ovo nam kasnije treba da bi mogli da izmenimo cenu nocenja
	inline double& GetCenaNocenja() { return mCenaNocenja; }
	// cisto virtuelna -> nije data specifikacija ove funkcije za 
	// objekte tipa Putovanje
	virtual double Zanimljivost() const = 0;
	virtual double Cena() const {
		return mBrNocenja * mCenaNocenja;
	}
	friend ostream& operator<<(ostream& izlTok, const Putovanje& putovanje) {
		// ovo pokrece virtuelni mehanizam (u skladu je sa uslovima
		// koje treba da ispuni da bi mogao da se primeni polimorfizam
		// zato sto je putovanje upucivacki tip, odnosno referenca)
		return putovanje.Write(izlTok);
	}
};

class Letovanje : public Putovanje {
private:
	double mProsecnaTempH2O;
	int mBrPlaza;
	char* mNazivMora;
	ostream& Write(ostream& izlTok) const override {
		Putovanje::Write(izlTok);
		return izlTok << "\nProsecna temperatura vode leti: " << mProsecnaTempH2O
			<< "\nBroj plaza: " << mBrPlaza 
			<< "\nNaziv mora: " << mNazivMora << '\n';
	}
public:
	Letovanje(double prosecnaTempH2O, int brPlaza, const char* nazivPlaze,
		const char* naziv, int brNocenja, double cenaNocenja) 
		: Putovanje(naziv, brNocenja, cenaNocenja), 
		mProsecnaTempH2O(prosecnaTempH2O), mBrPlaza(brPlaza) {
		mNazivMora = new char[strlen(nazivPlaze) + 1];
		strcpy(mNazivMora, nazivPlaze);
	}
	virtual ~Letovanje() {
		if (mNazivMora != NULL)
			delete[] mNazivMora,
			mNazivMora = NULL;
	}
	double Zanimljivost() const override {
		return mBrPlaza *
			(mProsecnaTempH2O > 26) ? 0.8 : 0.6;
	}
};

class ObilazakGrada : public Putovanje {
private:
	int mBrAtrakcija;
	double mCenaPrevoza;
	ostream& Write(ostream& izlTok) const override {
		Putovanje::Write(izlTok);
		return izlTok << "\nBroj turistickih atrakcija: " << mBrAtrakcija 
			<< "\nCena prevoza putovanja: " << mCenaPrevoza << '\n';
	}
public:
	ObilazakGrada(int brAtrakcija, double cenaPrevoza,
		const char* naziv, int brNocenja, double cenaNocenja) 
		: Putovanje(naziv, brNocenja, cenaNocenja),
		mBrAtrakcija(brAtrakcija), mCenaPrevoza(cenaPrevoza) {}
	virtual ~ObilazakGrada() {}
	double Zanimljivost() const override {
		return mBrAtrakcija;
	}
	double Cena() const override {
		return Putovanje::Cena() + mCenaPrevoza;
	}

};

class TuristickaOrganizacija {
private:
	Putovanje** mPutovanja;
	int mBrPutovanja, mKapacitet;
public:
	TuristickaOrganizacija(int kapacitet) 
		: mBrPutovanja(0), mKapacitet(kapacitet) {
 		mPutovanja = new Putovanje * [kapacitet];
		for (int i = 0; i < kapacitet; ++i)
			mPutovanja[i] = nullptr;
	}
	virtual ~TuristickaOrganizacija() {
		// ako uklanjamo/otkazujemo putovanja
		// moracemo da ih brisemo! Ova implementacija
		// desturktora na kraju brise samo 'zaostala'
		// putovanja
		for (int i = 0; i < mBrPutovanja; ++i)
			if (mPutovanja[i] != nullptr)
				delete mPutovanja[i],
				mPutovanja[i] = nullptr;
		delete[] mPutovanja,
			mPutovanja = nullptr;
	}
	bool Dodaj(Putovanje* put) {
		if (mBrPutovanja == mKapacitet)
			return false;
		int i = mBrPutovanja++;
		while (i > 0)
			if (put->Cena() < mPutovanja[i - 1]->Cena())
				mPutovanja[i] = mPutovanja[i - 1], i--;
			else
				break;
		mPutovanja[i] = put;
		return true;
	}
	bool IzmeniCenuNocenja(int indx, double novaCena) {
		if (indx < 0 || indx >= mBrPutovanja)
			return false;
		mPutovanja[indx]->GetCenaNocenja() = novaCena;
		return true;
	}
	friend ostream& operator<<(ostream& izlTok, const TuristickaOrganizacija& tOrg) {
		izlTok << "\nBroj zakazanih putovanja: " << tOrg.mBrPutovanja
			<< "\n\n\t\tLista zakazanih putovanja\n\n";
		for (int i = 0; i < tOrg.mBrPutovanja; ++i)
			izlTok << *tOrg.mPutovanja[i] << '\n';
		return izlTok;
	}
	void NajzanimljivijePutovanje() {
		int imax = 0;
		for (int i = 0; i < mBrPutovanja; ++i)
			if (mPutovanja[i]->Zanimljivost() >
				mPutovanja[imax]->Zanimljivost())
				imax = i;
		cout << *mPutovanja[imax] << '\n';
	}
	void AvgCena() {
		double ukCena = 0;
		int i = 0;
		for (; i < mBrPutovanja; ++i)
			ukCena += mPutovanja[i]->Cena();
		cout << "Srednja vrednost cene svih putovanja: " << ukCena / i << '\n';
	}
};

#define BRPUT 5

int main() {

	ofstream izlFajl("izlaz.txt");
	TuristickaOrganizacija tOrg(BRPUT);
	Putovanje* temp = new Letovanje(27.6, 3, "Jadransko", "Rovinj", 7, 19.99);
	tOrg.Dodaj(temp);
	temp = new Letovanje(28.2, 7, "Egejsko", "Nei Pori", 20, 29.99);
	tOrg.Dodaj(temp);
	temp = new Letovanje(22.3, 4, "Crno more", "Odesa", 10, 39.99);
	tOrg.Dodaj(temp);
	temp = new ObilazakGrada(13, 80, "Venecija", 1, 39.99);
	tOrg.Dodaj(temp);
	temp = new ObilazakGrada(29, 99.99, "Prag", 2, 89.99);
	tOrg.Dodaj(temp);

	izlFajl << tOrg;

	tOrg.AvgCena(), tOrg.NajzanimljivijePutovanje();
	
	izlFajl.close();

	return 0;
}
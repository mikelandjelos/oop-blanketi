#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

#define FOLKLOR 10
#define KOSARKA 3

enum class Izuzetak : int {
	errDodaj, // 0
	errOtkazi, // 1
	errListaSponzora, // 2
	errMinimalniBudzet // 3
};

template <class Aktivnost, int n>
class SportskiCentar {
private:
	Aktivnost mSale[n];
	int mBrAktivnosti;
public:
	SportskiCentar() : mBrAktivnosti(0) {}
	void Dodaj(const Aktivnost& aktivnost) {
		if (mBrAktivnosti == n)
			throw Izuzetak::errDodaj;
		int i = mBrAktivnosti++;
		while (i > 0 && mSale[i - 1] > aktivnost) {
			mSale[i] = mSale[i - 1];
			--i;
		}
		mSale[i] = aktivnost;
	}
	void Otkazi(int indeks) {
		if (indeks < 0 || indeks >= mBrAktivnosti 
			|| mBrAktivnosti == 0)
			throw Izuzetak::errOtkazi;
		for (int i = indeks; i < mBrAktivnosti - 1; ++i)
			mSale[i] = mSale[i + 1];
		mBrAktivnosti--;
	}
	double ProsecnaVrednost() {
		double vrednost = 0;
		int i;
		for (i = 0; i < mBrAktivnosti; ++i)
			vrednost += mSale[i];
		return vrednost / i;
	}
	Aktivnost& MaxVrednost() {
		int imax = 0;
		for (int i = 0; i < mBrAktivnosti; ++i)
			if (mSale[imax] < mSale[i])
				imax = i;
		return mSale[imax];
	}
	Aktivnost& MinVrednost() {
		int imin = 0;
		for (int i = 0; i < mBrAktivnosti; ++i)
			if (mSale[imin] > mSale[i])
				imin = i;
		return mSale[imin];
	}
	template <class Aktivnost, int n>
	friend std::ostream& operator<<(std::ostream&, const SportskiCentar<Aktivnost, n>&);
};

template <class Aktivnost, int n>
std::ostream& operator<<(std::ostream& izlTok, const SportskiCentar<Aktivnost, n>& aktivnost) {
	for (int i = 0; i < aktivnost.mBrAktivnosti; ++i)
		izlTok << aktivnost.mSale[i] << '\n';
	return izlTok;
}

struct RGB {
	int R;
	int G;
	int B;
};

class KosarkaskiKlub {
private:
	RGB mBojaDresa;
	vector<string> mListaSponzora;
	double mBudzetTima;
	int mBrojUl;
public:
	KosarkaskiKlub() {} // ovo ne bi trebalo da postoji
	KosarkaskiKlub(const KosarkaskiKlub& kk) {
		for (int i = 0; i < kk.mListaSponzora.size(); ++i)
			mListaSponzora.push_back(kk.mListaSponzora[i]);
		mBojaDresa = kk.mBojaDresa;
		mBudzetTima = kk.mBudzetTima;
		mBrojUl = kk.mBrojUl;
	}
	KosarkaskiKlub(istream& ulTok) {
		ulTok >> mBojaDresa.R >> mBojaDresa.G >> mBojaDresa.B;
		int brSponz;
		ulTok >> brSponz;
		ulTok.ignore();
		if (brSponz < 1)
			throw Izuzetak::errListaSponzora;
		mListaSponzora.clear();
		for (int i = 0; i < brSponz; ++i) {
			string temp;
			getline(ulTok, temp, '\n');
			mListaSponzora.push_back(temp);
		}
		ulTok >> mBudzetTima >> mBrojUl;
		if (mBudzetTima < 50000)
			throw Izuzetak::errMinimalniBudzet;
	}
	bool operator>(const KosarkaskiKlub& kklub) {
		if (mBudzetTima * mBrojUl * 10 > kklub.mBudzetTima * kklub.mBrojUl * 10)
			return true;
		return false;
	}
	bool operator<(const KosarkaskiKlub& kklub) {
		if (*this > kklub)
			return false;
		return true;
	}
	friend ostream& operator<<(ostream& izlTok, const KosarkaskiKlub& Kklub) {
		izlTok << "Boja dresa: R = " << Kklub.mBojaDresa.R
			<< " G = " << Kklub.mBojaDresa.G
			<< " B = " << Kklub.mBojaDresa.B << "\n";
		izlTok << "Lista sponzora:\n";
		for (int i = 0; i < Kklub.mListaSponzora.size(); ++i)
			izlTok << Kklub.mListaSponzora[i] << '\n';
		izlTok << "Budzet tima: " << Kklub.mBudzetTima
			<< "\nBroj sezonskih ulaznica: " << Kklub.mBrojUl << '\n';
		return izlTok;
	}
	friend double operator+=(double& dbl, KosarkaskiKlub& Kklub) {
		return dbl += Kklub.mBudzetTima * Kklub.mBrojUl * 10;
	}
};

typedef unsigned Folklor;

int main() {
	
	try {

		SportskiCentar<Folklor, FOLKLOR> SCFolklor;
		
		for (int i = 0; i < FOLKLOR; ++i) {
			unsigned temp = rand() % 100 + 1;
			cout << temp << '\n';
			SCFolklor.Dodaj(temp);
		}

		std::cout << '\n' << SCFolklor;

		cout << "\nMaxVrednost: " << SCFolklor.MaxVrednost() << '\n'
			<< "\nMinVrednost: " << SCFolklor.MinVrednost() << '\n'
			<< "\nAvgVrednost: " << SCFolklor.ProsecnaVrednost() << "\n\n";

		for (int i = 0; i < FOLKLOR; ++i)
			SCFolklor.Otkazi(0);

		SportskiCentar<KosarkaskiKlub, KOSARKA> SCKosarka;
		ifstream inTimovi("Timovi.txt");
		ofstream outTimovi("TimoviIzvestaj.txt");

		for (int i = 0; i < KOSARKA; ++i) {
			KosarkaskiKlub temp(inTimovi);
			std::cout << temp << '\n';
			SCKosarka.Dodaj(temp);
		}

		std::cout << '\n' << SCKosarka;

		outTimovi << "\nMaxVrednost:\n" << SCKosarka.MaxVrednost() << '\n'
			<< "\nMinVrednost:\n" << SCKosarka.MinVrednost() << '\n'
			<< "\nAvgVrednost:\n" << SCKosarka.ProsecnaVrednost() << '\n';

		for (int i = 0; i < KOSARKA; ++i)
			SCKosarka.Otkazi(0);

		inTimovi.close(), outTimovi.close();

	}
	catch (Izuzetak ex) {
		switch (ex) {
		case Izuzetak::errDodaj:
			cerr << "Greska u dodavanju aktivnosti!\n";
			break;
		case Izuzetak::errOtkazi:
			cerr << "Greska u otkazivanju aktivnosti!\n";
			break;
		case Izuzetak::errListaSponzora:
			cerr << "Tim mora imati barem jedan sponzor!\n";
			break;
		case Izuzetak::errMinimalniBudzet:
			cerr << "Tim mora imati minimalni budzet od 50k!\n";
		}
	}
	return 0;
}

/*KosarkaskiKlub(RGB bojaDresa, vector<string>& listaSponzora
		, double budzet, int brUl)
		: mBojaDresa(bojaDresa), mBudzetTima(budzet), mBrojUl(brUl) {
		if (listaSponzora.size() < 1)
			throw Izuzetak::errListaSponzora;
		if (budzet < 50000)
			throw Izuzetak::errMinimalniBudzet;
		for (int i = 0; i < listaSponzora.size(); ++i)
			mListaSponzora.push_back(listaSponzora[i]);
}*/
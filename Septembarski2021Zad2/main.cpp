#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>

enum Izuzetak {
	errDodavanje,			// 0
	errOtkazivanje,			// 1
	errMaksIzvodjaca,		// 2
	errMaksDuzinaTrajanja	// 3
};

template <class Dogadjaj, int BrojDana>
class Festival {
private:
	// maksimalni broj dogadjaja je 
	// 2 * BrojDana zato sto je moguce
	// da se u jedan dan smeste maks. 2 dogadjaja
	std::string mNaziv;
	Dogadjaj mRepertoar[2 * BrojDana];
	int mBrDogadjaja;
public:
	Festival(std::string naziv) 
		: mNaziv(naziv), mBrDogadjaja(0) {}
	virtual ~Festival() {
		mNaziv.clear();
	}
	void Dodaj(const Dogadjaj& dog) {
		if (mBrDogadjaja == 2 * BrojDana)
			throw Izuzetak::errDodavanje;
		mRepertoar[mBrDogadjaja++] = dog;
	}
	void Otkazi(int indeks) {
		if (mBrDogadjaja == 0 ||
			indeks < 0 || indeks >= mBrDogadjaja)
			throw Izuzetak::errOtkazivanje;
		mBrDogadjaja--;
	}
	template <class Dogadjaj, int BrojDana>
	friend std::ostream& operator<<(std::ostream& izlTok,
		const Festival<Dogadjaj, BrojDana>& festival);
};

template <class Dogadjaj, int BrojDana>
std::ostream& operator<<(std::ostream& izlTok,
	const Festival<Dogadjaj, BrojDana>& festival) {
	double prosek = 0;
	int i = 0;
	izlTok << festival.mNaziv << '\n';
	for (; i < festival.mBrDogadjaja; ++i) {
		prosek += festival.mRepertoar[i];
		izlTok << festival.mRepertoar[i] << '\n';
	}
	prosek /= i;
	return izlTok << prosek << '\n';
}

class MuzickiNastup {
private:
	std::string mNaziv;
	unsigned mBrIzv, mDuzina;
	double mKoefPop;
public:
	MuzickiNastup() {}
	MuzickiNastup(std::string naziv, unsigned brIzv,
		unsigned duzina, double koefPop)
		: mNaziv(naziv), mBrIzv(brIzv),
		mDuzina(duzina), mKoefPop(koefPop) {
		if (duzina > 120)
			throw Izuzetak::errMaksDuzinaTrajanja;
		if (brIzv > 10)
			throw Izuzetak::errMaksIzvodjaca;
	}
	virtual ~MuzickiNastup() {
		mNaziv.clear();
	}
	inline double Vrednost() const { 
		return mDuzina * mKoefPop;
	}
	friend double& operator+=(double& real, 
		const MuzickiNastup& muzNast) {
		return real += muzNast.Vrednost();
	}
	friend std::ostream& operator<<(std::ostream& izlTok,
		const MuzickiNastup& muzNast) {
		return izlTok << "Naziv: " << muzNast.mNaziv << '\n'
			<< "Broj izvodjaca: " << muzNast.mBrIzv << '\n'
			<< "Duzina nastupa(min): " << muzNast.mDuzina << '\n'
			<< "Koeficijent popularnosti: " << muzNast.mKoefPop;
	}
};

typedef int Film;

#define BROJFILMOVA 10
#define BROJMUZICKIH 3

int main() {

	try {

		Festival<Film, BROJFILMOVA>
			festivalFilm("Niski filmski festival");
		std::ofstream outFilm;

		for (int i = 0; i < BROJFILMOVA * 2; ++i)
			festivalFilm.Dodaj(rand() % 60 + 60);

		outFilm.open("filmski.txt");
		outFilm << "\n\t\tFestival pre otkazivanja filmova:\n"
			<< festivalFilm;

		for (int i = 0; i < 5; ++i)
			festivalFilm.Otkazi(rand() % 5 + 1);

		outFilm << "\n\t\tFestival nakon otkazivanja filmova:\n"
			<< festivalFilm;
		outFilm.close();

		Festival<MuzickiNastup, BROJMUZICKIH>
			festivalMuzika("Germany Bloodfest");
		std::vector<std::string> naziviMuzika = {
			"Iron Maiden 7th son 30th year anniversary tour",
			"Judas Priest 2022 world tour",
			"YJM Parabellum world tour",
			"Whitesnake farewell world tour",
			"Stratovarius 2022 world tour",
			"Tool all American tour 2022"
		};
		std::ofstream outMuzika;


		for (int i = 0; i < BROJMUZICKIH * 2; ++i) {
			MuzickiNastup temp(naziviMuzika[i], rand() % 10 + 1,
				rand() % 60 + 60, (rand() % 100) * 0.1 + 0.123);
			festivalMuzika.Dodaj(temp);
		}

		outMuzika.open("muzicki.txt");
		outMuzika << "\n\t\tFestival pre otkazivanja muzickih nastupa:\n"
			<< festivalMuzika;

		for (int i = 0; i < 2; ++i)
			festivalMuzika.Otkazi(rand() % 2 + 1);

		outMuzika << "\n\t\tFestival nakon otkazivanja muzickih nastupa:\n"
			<< festivalMuzika;
		outMuzika.close();
	
	}
	catch (Izuzetak ex) {
		switch (ex) {
		case Izuzetak::errDodavanje:
			return Izuzetak::errDodavanje;
		case Izuzetak::errOtkazivanje:
			return Izuzetak::errOtkazivanje;
		case Izuzetak::errMaksDuzinaTrajanja:
			return Izuzetak::errMaksDuzinaTrajanja;
		case Izuzetak::errMaksIzvodjaca:
			return Izuzetak::errMaksIzvodjaca;
		}
	}
	
	return 0;
}
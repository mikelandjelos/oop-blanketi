#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

enum class Izuzetak : unsigned {
	errDodavanje,
	errOtkazivanje
};

template <class Aktivnost, int n>
class Skola {
private:
	Aktivnost mSale[n];
	int mBrZauzetih;
public:
	Skola() 
		: mBrZauzetih(0) {}
	virtual ~Skola() {}
	void Dodaj(const Aktivnost& akt) {
		if (mBrZauzetih == n)
			throw Izuzetak::errDodavanje;
		int indeks = mBrZauzetih;
		for (int i = 0; i < mBrZauzetih; ++i)
			if (mSale[i] <= akt) {
				indeks = i;
				break;
			}
		for (int i = mBrZauzetih; i > indeks; --i)
			mSale[i] = mSale[i - 1];
		mSale[indeks] = akt;
		mBrZauzetih++;
	}
	void Otkazi(int indeks) {
		if (mBrZauzetih == 0 ||
			indeks < 0 || indeks >= mBrZauzetih)
			throw Izuzetak::errOtkazivanje;
		for (int i = indeks; i < mBrZauzetih - 1; ++i)
			mSale[i] = mSale[i + 1];
		mBrZauzetih--;
	}
	template <class Aktivnost, int n>
	friend std::ostream& operator<<(std::ostream& izlTok, 
			const Skola<Aktivnost, n>& skola);
	double ProsecnaVrednost() {
		double prosek = 0;
		int i = 0;
		for (; i < mBrZauzetih; ++i)
			prosek += mSale[i];
		return prosek /= i;
	}
	inline const Aktivnost& Max() {
		return mSale[0];
	}
	inline const Aktivnost& Min() {
		return mSale[mBrZauzetih - 1];
	}
};

template <class Aktivnost, int n> 
std::ostream& operator<<(std::ostream& izlTok, 
	const Skola<Aktivnost, n>& skola) {
	izlTok << "Ukupan broj sala: " << n
		<< "\nBroj zauzetih sala: " << skola.mBrZauzetih 
		<< "\nAktivnosti po salama:\n";
	for (int i = 0; i < skola.mBrZauzetih; ++i)
		izlTok << '\n' << skola.mSale[i];
	return izlTok;
}

typedef int Umetnost;

#define SKOLAUMETNOSTI 10

int main() {
	
	Skola<Umetnost, SKOLAUMETNOSTI> skUmetnost;

	for (int i = 0; i < SKOLAUMETNOSTI; ++i) {
		int temp(rand() % 100 + 1);
		skUmetnost.Dodaj(temp);
	}

	std::cout << "\n\n\tSkola pre izbacivanja:\n\n" << skUmetnost
		<< "\nProsecna vrednost aktivnosti: " << skUmetnost.ProsecnaVrednost()
		<< "\nNajvrednija aktivnost:\n" << skUmetnost.Max()
		<< "\nNajmanje vredna aktivnost:\n" << skUmetnost.Min();

	skUmetnost.Otkazi(5),
		skUmetnost.Otkazi(0);

	std::cout << "\n\n\tSkola nakon izbacivanja:\n\n" 
		<< skUmetnost << '\n';

	return 0;
}
#include <iostream>
#include <fstream>
#include <string>

#define BROJINDEKSA 20
#define BROJSTUDENATA 5

using namespace std;

enum Izuzetak {
	errDodavanje,	// 0
	errIzbacivanje, // 1
	errNalazenje,	// 2
	errRazlika,		// 3
	errSnimanje,	// 4
	errUcitavanje	// 5
};

template <class Element, int n>
class Spisak {
private:
	Element* mSpisak;
	int mBrEl;
public:
	Spisak() 
		: mBrEl(0) {
		mSpisak = new Element[n];
	}
	virtual ~Spisak() {
		delete[] mSpisak;
		mSpisak = nullptr;
	}
	void Dodaj(Element& el) {
		if (mBrEl == n)
			throw Izuzetak::errDodavanje;
		mSpisak[mBrEl++] = el;
	}
	void Izbaci(int index) {
		if (mBrEl == 0 ||
			index < 0 || index >= mBrEl)
			throw Izuzetak::errIzbacivanje;
		for (int i = index; i < mBrEl - 1; ++i)
			mSpisak[i] = mSpisak[i + 1];
		mBrEl--;
	}
	int Pronadji(int indeks) const {
		for (int i = 0; i < mBrEl; ++i)
			if (mSpisak[i] == indeks)
				return i;
		throw Izuzetak::errNalazenje;
	}
	int RazlikaPozicija(int indeks1, int indeks2) const {
		int i1 = -1, i2 = -1;
		for (int i = 0; i < mBrEl; ++i) {
			if (mSpisak[i] == indeks1 && i1 == -1)
				i1 = i;
			if (mSpisak[i] == indeks2 && i2 == -1)
				i2 = i;
		}
		if (i1 == -1 || i2 == -1)
			throw Izuzetak::errRazlika;
		return i1 - i2;
	}
	void VratiMaxMin(Element& max, Element& min) const {
		int imin = 0, imax = 0;
		for (int i = 0; i < mBrEl; ++i) {
			if (mSpisak[imin] > mSpisak[i])
				imin = i;
			if (mSpisak[imax] < mSpisak[i])
				imax = i;
		}
		max = mSpisak[imax];
		min = mSpisak[imin];
	}
	template <class Element, int n>
	friend ostream& operator<<(ostream& izlTok, const Spisak<Element, n>& spisak) {
		if (!izlTok.good())
			throw Izuzetak::errSnimanje;
		izlTok << spisak.mBrEl << '\n';
		for (int i = 0; i < spisak.mBrEl; ++i)
			izlTok << spisak.mSpisak[i] << '\n';
		return izlTok;
	}
	template <class Element, int n>
	friend istream& operator>>(istream& ulTok, Spisak<Element, n>& spisak) {
		if (!ulTok.good())
			throw Izuzetak::errUcitavanje;
		ulTok >> spisak.mBrEl;
		ulTok.ignore();
		for (int i = 0; i < spisak.mBrEl; ++i)
			ulTok >> spisak.mSpisak[i];
		return ulTok;
	}
};

class Student {
private:
	unsigned mBrInd;
	string mIme, mPrezime;
	double mAvgOcena;
public:
	Student() {}
	Student(istream& inStudent) {
		inStudent >> mBrInd,
			inStudent.ignore();
		inStudent >> mIme >> mPrezime;
		inStudent >> mAvgOcena;
	}
	virtual ~Student() { 
		mIme.clear(), mPrezime.clear();
	}
	bool operator==(int i) {
		if (mBrInd == i)
			return true;
		return false;
	}
	bool operator>(const Student& desni) {
		if (mBrInd > desni.mBrInd)
			return true;
		return false;
	}
	bool operator<(const Student& desni) {
		if (mBrInd < desni.mBrInd)
			return true;
		return false;
	}
	friend ostream& operator<<(ostream& izlTok, const Student& student) {
		return izlTok << "Ime i prezime: " << student.mIme
			<< ' ' << student.mPrezime << '\n'
			<< "Indeks: " << student.mBrInd << '\n'
			<< "Prosecna ocena iz prosle godine: " 
			<< student.mAvgOcena << '\n';
	}
	friend istream& operator>>(istream& ulTok, Student& student) {
		ulTok >> student.mBrInd,
			ulTok.ignore();
		return ulTok >> student.mIme >> student.mPrezime
			>> student.mAvgOcena;
	}
};

int main() {
	
	try {

		/*Spisak<int, BROJINDEKSA> spisakIndeksa;
		ofstream izlTok("izlaz.txt");

		for (int i = 0; i < BROJINDEKSA; ++i)
			spisakIndeksa.Dodaj(i += 18051),
			i -= 18051;

		izlTok << spisakIndeksa;

		for (int i = 0; i < BROJINDEKSA; ++i)
			spisakIndeksa.Izbaci(0);

		izlTok.close();*/

		Spisak<Student, BROJSTUDENATA> spisakStudenata;
		ifstream ulTok("StudentiUlaz.txt");
		ofstream izlTok("StudentiIzlaz.txt");

		for (int i = 0; i < BROJSTUDENATA; ++i) {
			Student temp(ulTok);
			spisakStudenata.Dodaj(temp);
		}

		izlTok << spisakStudenata;

		spisakStudenata.Izbaci(0);

		Student max, min;

		spisakStudenata.VratiMaxMin(max, min);

		cout << "Maks:\n" << max
			<< "\nMin:\n" << min;

		cout << "\n18234 je na poziciji: " 
			<< spisakStudenata.Pronadji(18234) << '\n';

		cout << "\nRazlika pozicija za 18234 i 18120 je: " 
			<< spisakStudenata.RazlikaPozicija(18234, 18120) << '\n';

		izlTok.close(), ulTok.close();
	
	}
	catch (Izuzetak ex) {
		switch (ex) {
		case Izuzetak::errDodavanje:
			cerr << "\n\t\tDoslo je do greske pri dodavanju elementa na spisak!\n";
			return Izuzetak::errDodavanje;
		case Izuzetak::errIzbacivanje:
			cerr << "\n\t\tDoslo je do greske pri izbacivanju elementa sa spiska!\n";
			return Izuzetak::errIzbacivanje;
		case Izuzetak::errNalazenje:
			cerr << "\n\t\tDoslo je do greske - zeljeni element nije na spisku!\n";
			return Izuzetak::errNalazenje;
		case Izuzetak::errRazlika:
			cerr << "\n\n\tDoslo je do greske - element nije nadjen na spisku!\n";
			return Izuzetak::errRazlika;
		case Izuzetak::errSnimanje:
			cerr << "\n\n\tDoslo je do greske pri pokusaju snimanja spiska u fajl!\n";
			return Izuzetak::errSnimanje;
		case Izuzetak::errUcitavanje:
			cerr << "\n\n\tDoslo je do greske pri pokusaju ucitavanju spiska iz fajla!\n";
		}
	}

	return 0;
}
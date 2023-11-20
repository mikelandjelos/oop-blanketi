#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <exception>

using std::cout;
using std::endl;
using std::string;
using std::ostream;
using std::istream;
using std::ofstream;
using std::ifstream;
using std::exception;
using std::cerr;

// sintaksa za klasnu enumeraciju je sledeca  (kljucne reci i tokeni C++-a
// su stavljeni izmedju dva simbola "; ovi simboli nisu, naravno, deo sintakse)
// "enum" "class" <Identifikator> ":" <primitivni_tip> "{" ... "}" ";"

template <class T>
class Raspored {
private:
	T* m_spisak;
	int m_br, m_kapac;
public:
	Raspored(int kapac) 
		: m_br(0), m_kapac(kapac) {
		if (kapac < 0)
			throw exception("Greska pri kreiranju rasporeda! (m_kapac: non_valid)");
		m_spisak = new T[kapac];
	}
	Raspored(const Raspored& rasp) 
		: m_br(rasp.m_br), m_kapac(rasp.m_kapac) {
		m_spisak = new T[rasp.m_kapac];
		for (int i = 0; i < rasp.m_br; ++i)
			m_spisak[i] = rasp.m_spisak[i];
	}
	virtual ~Raspored() {
		if (m_spisak != nullptr)
			delete[] m_spisak,
			m_spisak = nullptr;
	}
	void dodaj(T& el) {
		if (m_br == m_kapac)
			throw exception("Greska pri dodavanju elementa! (m_br: max)");
		m_spisak[m_br++] = el;
	}
	void ukloni(int indx) {
		if (indx < 0 || indx >= m_br || m_br == 0)
			throw exception("Greska pri uklanjanju elementa! (indx: non_valid)");
		for (int i = indx; i < m_br - 1; ++i)
			m_spisak[i] = m_spisak[i + 1];
		m_br--;
	}
	int pronadji(int indeks) {
		int i = 0;
		while (i < m_br && m_spisak[i] != indeks)
			i++;
		if (i == m_br)
			throw exception("Trazeni element ne postoji! (indeks: not_found)");
		return i;
	}
	int razlika(int indeks1, int indeks2) {
		return pronadji(indeks1) - pronadji(indeks2);
	}
	// ova funkcija je mnogo lepse mogla da se napise
	T** vratiMinMax() {
		int* minMax = new int[2]{ 0, 0 };
		T** ekstremi = new T * [2];
		for (int i = 1; i < m_br; ++i) {
			if (m_spisak[minMax[0]] > m_spisak[i]) {
				minMax[0] = i; // ovo je minimum
				continue;
			}
			if (m_spisak[minMax[1]] < m_spisak[i])
				minMax[1] = i; // ovo je maksimum
		}
		ekstremi[0] = &(m_spisak[minMax[0]]);
		ekstremi[1] = &(m_spisak[minMax[1]]);
		delete[] minMax;
		return ekstremi;
	}
	void invertuj() {
		for (int i = 0; i < m_br / 2; ++i) {		
			T pom = m_spisak[i];
			m_spisak[i] = m_spisak[m_br - 1 - i],
			m_spisak[m_br - 1 - i] = pom;
		}
	}
	template <class T>
	friend ostream& operator<<(ostream&, const Raspored<T>&);
	// template <class T>
	// friend istream& operator>>(istream&, Raspored<T>* const);
};

template <class T>
ostream& operator<<(ostream& izlTok, const Raspored<T>& rasp) {
	if (!izlTok.good())
		throw exception("\nNepostojeca datoteka! (izlTok: not_found)");
	izlTok << rasp.m_br << '\n';
	for (int i = 0; i < rasp.m_br; ++i)
		izlTok << rasp.m_spisak[i] << '\n';
	return izlTok;
}

#define BRIND 20

int main() {
	
	try {

		Raspored <int> raspInd(20);
		ofstream datoteka("datoteka.txt");
		int pretraga1, pretraga2, **minMax = new int*[2];

		for (int i = 0; i < BRIND; ++i) {
			int temp = rand() % 350 + 18050;
			raspInd.dodaj(temp);
			if (i == 5)
				pretraga1 = temp;
			if (i == 7)
				pretraga2 = temp;
		}

		cout << "\t\tPre uklanjanja:\n\n" << raspInd;

		raspInd.ukloni(8);
		raspInd.ukloni(1);

		cout << "\t\tNakon uklanjanja:\n\n" << raspInd;

		minMax = raspInd.vratiMinMax();

		cout << "Minimalni: " << *minMax[0] << '\n';
		cout << "Maksimalni: " << *minMax[1] << '\n';
	
		raspInd.invertuj();

		cout << "\t\tInvertovan:\n\n" << raspInd;
		datoteka << raspInd;
		datoteka.close();
	}
	catch (exception exc) {
		std::cerr << "\n\t\t" << exc.what() << '\n';
	}
	return 0;
}
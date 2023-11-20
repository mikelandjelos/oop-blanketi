#include <iostream>
#include <exception>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>

template <class T>
class Spisak {
protected:
	T* m_spisak;
	unsigned long m_trBr, m_kapacitet;
	double m_minOcena;
public:
	Spisak(int kapacitet, double minOcena) 
		: m_spisak(new T[kapacitet]), m_trBr(0),
		m_kapacitet(kapacitet), m_minOcena(minOcena) {}
	virtual ~Spisak() {
		if (m_spisak != nullptr)
			delete[] m_spisak,
				m_spisak = nullptr;
	}
	bool dodaj(T el) throw (std::exception*) {
		if (m_trBr == m_kapacitet)
			throw new std::exception("SPISAK_ERR: Nemoguce dodavanje filma na spisak!");
		if (el < m_minOcena) // bool operator<(const T&, double);
			return false;
		m_spisak[m_trBr++] = el; // T& operator=(T&, T&);
		return true;
	}
	void obrisi(int indx) throw (std::exception*) {
		if (indx < 0 || indx >= m_trBr)
			throw new std::exception("SPISAK_ERR: Nemoguce brisanje filma na spiska!");
		for (int i = indx; i < m_trBr - 1; ++i)
			m_spisak[i] = m_spisak[i + 1];
		m_trBr--;
	}
	void promeniOcenu(long indx, double novaOcena) throw (std::exception*) {
		if (indx < 0|| indx >= m_trBr)
			throw new std::exception("SPISAK_ERR: Nemoguca promena ocene!");
		if (novaOcena < m_minOcena) {
			obrisi(indx);
			return;
		}
		m_spisak[indx] = novaOcena; // T& operator=(T&, double);
	}
	unsigned long filmoviSaOcenom(double ocena) {
		if (ocena < m_minOcena || m_trBr == 0)
			return 0;
		unsigned long brojFilmova = 0;
		for (long i = 0; i < m_trBr; ++i)
			if (m_spisak[i] == ocena) // bool operator==(T&, double);
				brojFilmova++;
		return brojFilmova;
	}
	void minMaxFilm(T& min, T& max) throw (std::exception*) {
		if (m_trBr == 0)
			throw new std::exception("SPISAK_ERR: Nemoguce nalazenje trazenih filmova!");
		long imin = 0, imax = 0;
		for (long i = 0; i < m_trBr; ++i) {
			if (m_spisak[imin] > m_spisak[i]) // bool operator>(T&, T&);
				imin = i;
			if (m_spisak[imax] < m_spisak[i]) // bool operator<(T&, T&);
				imax = i;
		}
		min = m_spisak[imin];
		max = m_spisak[imax];
	}
	template <class T>
	friend std::ostream& operator<<(std::ostream&, const Spisak<T>&);
	template <class T>
	friend std::istream& operator>>(std::istream&, Spisak<T>&);
};

template <class T>
std::ostream& operator<<(std::ostream& izlTok, const Spisak<T>& spisak) {
	izlTok << spisak.m_trBr << '\n' << spisak.m_minOcena;
	for (long i = 0; i < spisak.m_trBr; ++i)
		izlTok << '\n' << spisak.m_spisak[i]; // std::ostream& operator<<(std::ostream&, const T&);
	return izlTok;
}

template <class T>
std::istream& operator>>(std::istream& ulTok, Spisak<T>& spisak) {
	ulTok >> spisak.m_trBr >> spisak.m_minOcena;
	spisak.m_kapacitet = spisak.m_trBr * 1.2;
	if (spisak.m_spisak != nullptr)
		delete[] spisak.m_spisak;
	spisak.m_spisak = new T[spisak.m_kapacitet];
	for (int i = 0; i < spisak.m_trBr; ++i) {
		ulTok >> spisak.m_spisak[i]; // std::istream& operator>>(std::istream&, T&);
		ulTok.ignore(); // ovo je zbog onog '\n'
	}
	return ulTok;
}

typedef double Ocena;

class Film {
private:
public:
};

#define BROCENA 20
#define BRFILMOVA 5

int main() {
	
	srand(time(NULL));

	try {

		Spisak<double> spisakOcena(BROCENA, (rand() % 2 + 5.) + (rand() % 100 + 1) / 100.);
		int indx;
		for (int i = 0; i < BROCENA; ++i)
			if (!spisakOcena.dodaj((rand() % 5 + 5.) + (rand() % 100 + 1) / 100.))
				std::cout << "Film nije dodat - ocena mu je manja od minimalne!\n";

		std::cout << "\n\t\tPre brisanja:\n\n" << spisakOcena << '\n';

		std::cout << "Film na kom indeksu zelite da obrisete: ";
		std::cin >> indx;
		spisakOcena.obrisi(indx);

		std::cout << "\n\t\tNakon brisanja:\n\n" << spisakOcena << '\n';
	}
	catch (std::exception* ex) {
		std::cerr << "Error message: " << ex->what() << '\n';
	}

	return 0;
}
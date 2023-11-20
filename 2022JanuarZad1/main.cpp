#include <iostream>
#include <fstream>
#include <exception>
#include <cstdlib>
#include <vector>

using namespace std;

class Figura {
protected:
	double m_x, m_y;
public:
	Figura() 
		: m_x(0), m_y(0) {}
	Figura(double x, double y) 
		: m_x(x), m_y(y) {}
	virtual ~Figura() {}
	virtual ostream& write(ostream& izlTok) const {
		return izlTok << m_x << ' ' << m_y;
	}
	virtual istream& read(istream& ulTok) {
		return ulTok >> m_x >> m_y;
	}
	virtual double povrsina() const = 0;
	virtual double obim() const = 0;
};

class Kvadrat : public Figura {
private:
	double m_a;
public:
	Kvadrat() 
		: Figura(), m_a(0) {}
	Kvadrat(double a,double x, double y) 
		: Figura(x, y), m_a(a) {
		if (a < 0) {
			throw exception("Nemoguce kreirati kvadrat!");
		}
	}
	virtual ~Kvadrat() {}
	ostream& write(ostream& izlTok) const override {
		return Figura::write(izlTok) << ' ' << m_a;
	}
	istream& read(istream& ulTok) override {
		return Figura::read(ulTok) >> m_a;
	}
	double povrsina() const override {
		return m_a * m_a;
	}
	double obim() const override {
		return 4 * m_a;
	}
};

class Pravougaonik : public Figura {
private:
	double m_a, m_b;
public:
	Pravougaonik() 
		: Figura(), m_a(0), m_b(0) {}
	Pravougaonik(double a, double b, double x, double y) 
		: Figura(x, y), m_a(a), m_b(b) {
		if (a < 0 || b < 0) {
			throw exception("Nemoguce kreirati pravougaonik!");
		}
	}
	virtual ~Pravougaonik() {}
	ostream& write(ostream& izlTok) const override {
		return Figura::write(izlTok) << ' ' << m_a << ' ' << m_b;
	}
	istream& read(istream& ulTok) override {
		return Figura::read(ulTok) >> m_a >> m_b;
	}
	double povrsina() const override {
		return m_a * m_b;
	}
	double obim() const override {
		return 2 * m_a + 2 * m_b;
	}
};

class Kolekcija {
private:
	Figura** m_figure;
	int m_brEl, m_kapacitet;
	void realokacija() {
		m_kapacitet *= 1.5;
		Figura** pom = new Figura * [m_kapacitet];
		for (int i = 0; i < m_brEl; ++i)
			pom[i] = m_figure[i];
		for (int i = m_brEl; i < m_kapacitet; ++i)
			pom[i] = nullptr;
		delete[] m_figure;
		m_figure = pom;
	}
public:
	Kolekcija(int kapacitet) 
		: m_brEl(0), m_kapacitet(kapacitet) {
		m_figure = new Figura * [kapacitet]; // napravi niz pointera
		for (int i = 0; i < kapacitet; ++i)
			m_figure[i] = nullptr;
	}
	virtual ~Kolekcija() {
		for (int i = 0; i < m_brEl; ++i)
			if (m_figure[i] != nullptr) {
				delete m_figure[i];
				m_figure[i] = nullptr;
			}
		delete[] m_figure;
		m_figure = nullptr;
	}
	void dodaj(Figura* ptr) {
		if (m_brEl == m_kapacitet)
			realokacija();
		m_figure[m_brEl++] = ptr;
	}
	double ukPovrsina() {
		double povrsina = 0;
		int br;
		for (br = 0; br < m_brEl; br++)
			povrsina += m_figure[br]->povrsina();
		return povrsina;
	}
	double ukObim() {
		double obim = 0;
		int br;
		for (br = 0; br < m_brEl; br++)
			obim += m_figure[br]->obim();
		return obim;
	}
	void sortObim() {
		for (int i = 0; i < m_brEl; ++i) {
			int imin = i;
			for (int j = i + 1; j < m_brEl; ++j)
				if (m_figure[j]->obim() < m_figure[imin]->obim())
					imin = j;
			Figura* pom = m_figure[i];
			m_figure[i] = m_figure[imin];
			m_figure[imin] = pom;
		}
	}
	friend ostream& operator<<(ostream& izlTok, const Kolekcija& kolekcija) {
		izlTok << kolekcija.m_brEl << ' ' << kolekcija.m_kapacitet << '\n';
		for (int i = 0; i < kolekcija.m_brEl; ++i)
			kolekcija.m_figure[i]->write(izlTok) << '\n';
		return izlTok;
	}
};

int main() {

	try {

		Kolekcija kolekcija(2);
		Figura* niz[5] = {
			new Kvadrat(5, 0, 0),
			new Kvadrat(12, -1, 5),
			new Pravougaonik(3, 2, 6, 7),
			new Pravougaonik(4, 1, 2, 3),
			new Kvadrat(5, 0, 0)
		};

		for (int i = 0; i < 4; ++i)
			kolekcija.dodaj(niz[i]);

		cout << kolekcija << '\n';

		kolekcija.sortObim();

		cout << '\n' << kolekcija << '\n';
	
	}
	catch (exception ex) {
		cout << ex.what() << '\n';
	}

	return 0;
}
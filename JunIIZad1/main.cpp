#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>

class Posetilac {
protected:
	int m_termin;
	std::string m_ime, m_prezime;
	std::ostream& printMiscellaneous(std::ostream& izlTok) const {
		return izlTok << "Koeficijent zainteresovanosti: " << koefZainteresovanosti()
			<< "\nPlaceno za ulaz: " << cenaKarte();
	}
public:
	Posetilac(int termin, std::string ime, std::string prezime) 
		: m_termin(termin), m_ime(ime), m_prezime(prezime) {}
	virtual std::ostream& print(std::ostream& izlTok) const {
		return izlTok << "Ime i prezime: " << m_ime << ' ' << m_prezime
			<< "\nTermin: " << m_termin;
	}
	virtual ~Posetilac() {
		m_ime.clear(),
			m_prezime.clear();
	}
	friend bool operator>(const Posetilac& levi, const Posetilac& desni) {
		return levi.m_termin > desni.m_termin;
	}
	friend bool operator==(const Posetilac& levi, int termin) {
		return levi.m_termin == termin;
	}
	virtual double koefZainteresovanosti() const = 0;
	virtual double cenaKarte() const = 0;
};

class VirtuelniPosetilac  : public Posetilac {
protected:
	std::string m_korisnickoIme;
	int m_brojSati, m_brojKomentara;
	double m_cenaPoSatu;
public:
	VirtuelniPosetilac(int termin, std::string ime, std::string prezime,
		std::string korisnickoIme, int brojSati, int brojKomentara,
		double cenaPoSatu) 
		: Posetilac(termin, ime, prezime), m_korisnickoIme(korisnickoIme),
		m_brojSati(brojSati), m_brojKomentara(brojKomentara), m_cenaPoSatu(cenaPoSatu) {}
	virtual ~VirtuelniPosetilac() {
		m_korisnickoIme.clear();
	}
	virtual std::ostream& print(std::ostream& izlTok) const override {
		Posetilac::print(izlTok)
			<< "\nKorisnicko ime: " << m_korisnickoIme
			<< "\nAktivan [h]: " << m_brojSati
			<< "\nBroj komentara: " << m_brojKomentara << '\n';
		return printMiscellaneous(izlTok);
	}
	double koefZainteresovanosti() const override {
		return m_brojKomentara * m_brojSati;
	}
	double cenaKarte() const override {
		return m_cenaPoSatu * m_brojSati;
	}
};

class StvarniPosetilac : public Posetilac {
protected:
	bool m_radniStatus;
	int m_brojOdgovora;
	double m_popust;
	double m_osnovnaCena;
public:
	StvarniPosetilac(int termin, std::string ime, std::string prezime,
		bool radniStatus, int brojOdgovora, double popust, double osnovnaCena) 
		: Posetilac(termin, ime, prezime), 
		m_radniStatus(radniStatus), m_brojOdgovora(brojOdgovora),
		m_popust(popust), m_osnovnaCena(osnovnaCena) {}
	virtual ~StvarniPosetilac() {}
	virtual std::ostream& print(std::ostream& izlTok) const override {
		Posetilac::print(izlTok)
			<< "\nRadni status: " << ((m_radniStatus) ? "zaposlen" : "nezaposlen")
			<< "\nBroj tacnih odgovora: " << m_brojOdgovora << '\n';
		return printMiscellaneous(izlTok);
	}
	double koefZainteresovanosti() const override {
		return m_brojOdgovora;
	}
	double cenaKarte() const override {
		return (1 - m_popust*100)* m_osnovnaCena;
	}
};

class DrevniZamak {
private:
	Posetilac** m_posetioci;
	unsigned long m_kapacitet, m_brojPosetilaca;
public:
	DrevniZamak(unsigned long kapacitet) 
		: m_kapacitet(kapacitet), m_brojPosetilaca(0),
		m_posetioci(new Posetilac * [kapacitet]) {
		for (int i = 0; i < kapacitet; ++i)
			m_posetioci[i] = 0;
	}
	virtual ~DrevniZamak() {
		for (int i = 0; i < m_brojPosetilaca; ++i)
			if (m_posetioci[i] != nullptr)
				delete m_posetioci[i];
		if (m_posetioci != nullptr)
			delete[] m_posetioci;
	}
	void dodaj(Posetilac* pposetilac) {
		if (m_brojPosetilaca == m_kapacitet) // tj ako nemamo vise mesta
			std::cerr << "PREPUNILI STE ZAMAK!!!",
			std::exit(1);
		int i = m_brojPosetilaca++;
		while (i > 0 && *m_posetioci[i - 1] > *pposetilac)
			m_posetioci[i--] = m_posetioci[i - 1];
		m_posetioci[i] = pposetilac;
	}
	bool otkazi(int termin) {
		int i;
		for (i = 0; i < m_brojPosetilaca; ++i)
			if (*m_posetioci[i] == termin)
				break;
		if (i == m_brojPosetilaca)
			return false;
		delete m_posetioci[i];
		for (int j = i; j < m_brojPosetilaca - 1; ++j)
			m_posetioci[j] = m_posetioci[j + 1];
		m_posetioci[m_brojPosetilaca-- - 1] = nullptr;
	}
	void evidentiraj() {
		std::ofstream outEvidencija("posete.txt");

		outEvidencija << "Broj posetilaca: " << m_brojPosetilaca
			<< "\n\n\t\tPosetioci:\n\n";

		for (int i = 0; i < m_brojPosetilaca; ++i)
			m_posetioci[i]->print(outEvidencija) << '\n';

		outEvidencija.close();
	}
	friend std::ostream& operator<<(std::ostream& outEvidencija, const DrevniZamak& drevniZamak) {
		outEvidencija << "Broj posetilaca: " << drevniZamak.m_brojPosetilaca
			<< "\n\n\t\tPosetioci:\n\n";
		for (int i = 0; i < drevniZamak.m_brojPosetilaca; ++i)
			drevniZamak.m_posetioci[i]->print(outEvidencija) << '\n';
		return outEvidencija;
	}
};

#define BRPOS 6

int main() {
	
	DrevniZamak dz(BRPOS);
	Posetilac* posetioci[BRPOS] = {
		new StvarniPosetilac(1, "Mihajlo", "Madic", false, 10, 0.3, 1500),
		new StvarniPosetilac(2, "Pera", "Peric", true, 12, 0.1, 1200),
		new StvarniPosetilac(3, "Mare", "Maric", true, 15, 0.2, 1700),
		new VirtuelniPosetilac(5, "Mihajlo", "Madic", "nick1", 6, 17, 100),
		new VirtuelniPosetilac(-1, "Peera", "Petrovic", "blabla", 6, 17, 100),
		new VirtuelniPosetilac(-2, "Geralt", "iz Rivije", "gers", 6, 17, 100)
	};

	for (int i = 0; i < BRPOS; ++i)
		dz.dodaj(posetioci[i]);

	dz.evidentiraj();

	dz.otkazi(-1);

	std::cout << dz << '\n';

	return 0;
}
#include <iostream>
#include <fstream>
#include <exception>
#include <string>
#include <cstdlib>

using namespace std;

template <class T>
class Polica {
private:
	T* m_niz;
	int m_kapacitet, m_brEl;
public:
	Polica(int kapacitet) 
		: m_kapacitet(kapacitet), m_brEl(0),
		m_niz(new T[kapacitet]) {}
	virtual ~Polica() {
		delete[] m_niz,
			m_niz = nullptr;
	}
	void dodaj(const T& el) {
		if (m_brEl == m_kapacitet)
			throw exception("POLICA_ERR: Polica je popunjena!");
		for (int i = 0; i < m_brEl; ++i)
			if (el == m_niz[i])
				throw exception("POLICA_ERR: Objekat je vec na polici!");
		for (int i = m_brEl - 1; i >= 0; --i)
			if (el < m_niz[i])
				m_niz[i + 1] = m_niz[i];
			else {
				m_niz[i] = el;
				break;
			}
		m_brEl++;
	}
	void izbaci(int indx) {
		if (indx < 0 || indx >= m_brEl || m_brEl == 0)
			throw exception("POLICA_ERR: Nemoguce izbacivanje objekta s police!");
		for (int i = indx; i < m_brEl - 1; ++i)
			m_niz[i] = m_niz[i + 1];
		m_brEl--;
	}
	template <class T>
	friend ostream& operator<<(ostream&, const Polica<T>&);
	template <class T>
	friend istream& operator>>(istream&, Polica<T>&);
};

template <class T>
ostream& operator<<(ostream& izlTok, const Polica<T>& polica) {
	for (int i = 0; i < polica.m_brEl; ++i)
		izlTok << polica.m_niz[i] << '\n';
	return izlTok;
}

template <class T>
istream& operator>>(istream& ulTok, Polica<T>& polica) {
	for (int i = 0; i < polica.m_kapacitet && !ulTok.eof(); ++i)
		ulTok >> polica.m_niz[i],
		ulTok.ignore(), polica.m_brEl++;
	return ulTok;
}

class Knjiga {
private:
	string m_naslov, m_autor;
	int m_brStrana, m_tiraz;
public:
	Knjiga() 
		: m_naslov("\0"), m_autor("\0"),
		m_brStrana(-1), m_tiraz(-1) {}
	Knjiga(string naslov, string autor, int brStrana, int tiraz) 
		: m_naslov(naslov), m_autor(autor), 
		m_brStrana(brStrana), m_tiraz(tiraz) {
		if (brStrana <= 0)
			throw exception("KNJIGA_ERR: Knjiga mora imati pozitivan broj strana!");
		if (tiraz < 200)
			throw exception("KNJIGA_ERR: Tiraz mora biti najmanje 200!");
	}
	virtual ~Knjiga() {
		m_naslov.clear(), m_autor.clear(), 
			m_brStrana = -2, m_tiraz = -2;
	}
	friend bool operator<(const Knjiga& leva, const Knjiga& desna) {
		int min = (leva.m_naslov.length() < desna.m_naslov.length())
			? leva.m_naslov.length() : desna.m_naslov.length();
		for (int i = 0; i < min; ++i)
			if (leva.m_naslov[i] < desna.m_naslov[i])
				return true;
		if (min == leva.m_naslov.length())
			return true;
		min = (leva.m_autor.length() < desna.m_autor.length())
			? leva.m_autor.length() : desna.m_autor.length();
		for (int i = 0; i < min; ++i)
			if (leva.m_autor[i] < desna.m_autor[i])
				return true;
		if (min == leva.m_autor.length())
			return true;
		if (leva.m_brStrana < desna.m_brStrana)
			return true;
		if (leva.m_tiraz < desna.m_tiraz)
			return true;
		return false;
	}
	friend bool operator>(const Knjiga& leva, const Knjiga& desna) {
		int min = (leva.m_naslov.length() < desna.m_naslov.length())
			? leva.m_naslov.length() : desna.m_naslov.length();
		for (int i = 0; i < min; ++i)
			if (leva.m_naslov[i] > desna.m_naslov[i])
				return true;
		if (min == desna.m_naslov.length())
			return true;
		min = (leva.m_autor.length() < desna.m_autor.length())
			? leva.m_autor.length() : desna.m_autor.length();
		for (int i = 0; i < min; ++i)
			if (leva.m_autor[i] > desna.m_autor[i])
				return true;
		if (min == desna.m_autor.length())
			return true;
		if (leva.m_brStrana > desna.m_brStrana)
			return true;
		if (leva.m_tiraz > desna.m_tiraz)
			return true;
		return false;
	}
	friend bool operator==(const Knjiga& leva, const Knjiga& desna) {
		if (leva < desna || leva > desna)
			return false;
		return true;
	}
	friend ostream& operator<<(ostream& izlTok, const Knjiga& knjiga) {
		return izlTok << "Naslov: " << knjiga.m_naslov
			<< "\nAutor: " << knjiga.m_autor
			<< "\nBroj stranica: " << knjiga.m_brStrana
			<< "\nTiraz: " << knjiga.m_tiraz;
	}
	friend istream& operator>>(istream& ulTok, Knjiga& knjiga) {
		getline(ulTok, knjiga.m_naslov);
		getline(ulTok, knjiga.m_autor);
		return ulTok >> knjiga.m_brStrana >> knjiga.m_tiraz;
	}
	Knjiga& operator=(const Knjiga& that) {
		m_naslov = that.m_naslov;
		m_autor = that.m_autor;
		m_brStrana = that.m_brStrana;
		m_tiraz = that.m_tiraz;
		return *this;
	}
};

class Disk {
private:
	int m_velicina, m_zauzetProstor; 
	int slobodanProstor() const { return m_velicina - m_zauzetProstor; }
public:
	Disk()
		: m_velicina(-1), m_zauzetProstor(-1) {}
	Disk(int velicina, int zauzetProstor) 
		: m_velicina(velicina), m_zauzetProstor(zauzetProstor) {
		if (m_velicina < 0 || m_zauzetProstor)
			throw exception("DISK_ERR: Nemoguce vrednosti karakteristika diska!");
	}
	virtual ~Disk() {}
	friend bool operator<(const Disk& levi, const Disk& desni) {
		if (levi.m_velicina < desni.m_velicina)
			return true;
		if (levi.m_zauzetProstor < desni.m_zauzetProstor)
			return true;
		return false;
	}
	friend bool operator>(const Disk& levi, const Disk& desni) {
		if (levi.m_velicina > desni.m_velicina)
			return true;
		if (levi.m_zauzetProstor > desni.m_zauzetProstor)
			return true;
		return false;
	}
	friend bool operator==(const Disk& levi, const Disk& desni) {
		if (levi.m_velicina == desni.m_velicina)
			return true;
		if (levi.m_zauzetProstor == desni.m_zauzetProstor)
			return true;
		return false;
	}
	friend ostream& operator<<(ostream& izlTok, const Disk& disk) {
		return izlTok << "Velicina: " << disk.m_velicina
			<< "\nZauzeto: " << disk.m_zauzetProstor
			<< "\nRaspolozivo: " << disk.slobodanProstor();
	}
	friend istream& operator>>(istream& ulTok, Disk& disk) {
		return ulTok >> disk.m_velicina >> disk.m_zauzetProstor;
	}
};

#define BRKNJIGA 5
#define BROJDISKOVA 7


int main() {

	

	try {

		ifstream inKnjige("knjige_in.txt");
		ofstream outKnjige("knjige_out.txt");

		Polica<Knjiga> policaKnjige(BRKNJIGA);

		inKnjige >> policaKnjige;

		policaKnjige.izbaci(4);

		outKnjige << policaKnjige;

		inKnjige.close(), outKnjige.close();
	
	}
	catch (exception ex) {
		cerr << "\n\t\t" << ex.what() << '\n';
	}

	/*
	try {

		ifstream inDiskovi("diskovi_in.txt");
		ofstream outDiskovi("diskovi_out.txt");


		Polica<Disk> policaDiskovi(BROJDISKOVA);

		inDiskovi >> policaDiskovi;

		policaDiskovi.izbaci(7);

		outDiskovi << policaDiskovi;

		inDiskovi.close(), outDiskovi.close();

	}
	catch (exception ex) {
		cerr << "\n\t\t" << ex.what() << '\n';
	}
	*/
	return 0;
}
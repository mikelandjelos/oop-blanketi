#include <iostream>
#include <exception>

template <class T>
class Skup {
private:
	T* m_niz;
	int m_vrh, m_kapacitet;
public:
	Skup(int kapacitet) 
		: m_vrh(0), m_kapacitet(kapacitet),
		m_niz(new T[kapacitet]) {}
	Skup(const Skup& skup) 
		: m_vrh(skup.m_vrh), m_kapacitet(skup.m_kapacitet),
		m_niz(new T[skup.m_kapacitet]) {
		for (int i = 0; i < skup.m_vrh; ++i)
			m_niz[i] = skup.m_niz[i];
	}
	virtual ~Skup() {
		if (m_niz != nullptr)
			delete[] m_niz;
	}
	bool dodaj(T& el) {
		if (m_vrh == m_kapacitet)
			throw std::exception("SKUP_ERR: \'Niz je pun!\'");
		for (int i = 0; i < m_vrh; ++i)
			if (el == m_niz[i])
				return false;
		m_niz[m_vrh++] = el;
		return true;
	}
	void ukloni(const T& el) {
		int i;
		for (i = 0; i < m_vrh; ++i)
			if (el == m_niz[i])
				break;
		for (int j = i; j < m_vrh - 1; ++j)
			m_niz[j] = m_niz[j + 1];
		m_vrh--;
	}
	template <class T>
	friend Skup<T> unija(const Skup<T>&, const Skup<T>&);
	template <class T>
	friend std::ostream& operator<<(std::ostream&, const Skup<T>&);
};

template <class T>
Skup<T> unija(const Skup<T>& levi, const Skup<T>& desni) {
	Skup<T> novi(levi.m_kapacitet + desni.m_kapacitet);
	for (int i = 0; i < levi.m_vrh; ++i)
		novi.dodaj(levi.m_niz[i]);
	for (int i = 0; i < desni.m_vrh; ++i)
		novi.dodaj(desni.m_niz[i]);
	return novi; // za ovaj tip povratne vrednosti mora da postoji copy konstruktor
}

template <class T>
std::ostream& operator<<(std::ostream& izlTok, const Skup<T>& skup) {
	for (int i = 0; i < skup.m_vrh; ++i)
		izlTok << skup.m_niz[i] << '\n';
	return izlTok;
}

class Takmicar {
private:
	int m_id;
	int m_startniBroj;
	int m_rezultat;
public:
	Takmicar() 
		: m_id(0), m_startniBroj(0), m_rezultat(0) {}
	Takmicar(int id, int startniBroj, int rezultat)
		: m_id(id), m_startniBroj(startniBroj), m_rezultat(rezultat) {}
	virtual ~Takmicar() {}
	friend bool operator==(const Takmicar& levi, const Takmicar& desni) {
		if (levi.m_id == desni.m_id)
			return true;
		return false;
	}
	friend std::ostream& operator<<(std::ostream& izlTok, const Takmicar& takm) {
		return izlTok << "Id: " << takm.m_id
			<< "\nStartni broj: " << takm.m_startniBroj
			<< "\nRezultat: " << takm.m_rezultat;
	}
};

#define BREL 7

int main() {

	try {

		Skup<Takmicar> skupTakmicara1(4);
		Skup<Takmicar> skupTakmicara2(3);
		Takmicar* niz = new Takmicar[BREL]{
			Takmicar(1, 5, 76),
			Takmicar(2, 3, 99),
			Takmicar(3, 5, 76),
			Takmicar(1, 5, 100),
			Takmicar(4, 13, 58),
			Takmicar(5, 12, 41),
			Takmicar(6, 19, 12)
		};

		for (int i = 0; i < BREL - 3; ++i)
			skupTakmicara1.dodaj(niz[i]);

		for (int i = 4; i < BREL; ++i)
			skupTakmicara2.dodaj(niz[i]);

		std::cout << skupTakmicara1 << '\n';

		skupTakmicara1.ukloni(Takmicar(1, 5, 76));

		std::cout << skupTakmicara1 << '\n';

		std::cout << skupTakmicara2 << '\n';

		Skup<Takmicar> zbir = unija(skupTakmicara1, skupTakmicara2);

		std::cout << zbir << '\n';
	
	}
	catch (std::exception ex) {
		std::cerr << ex.what() << '\n';
	}

	return 0;
}
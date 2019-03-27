#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;



class Zadania {
private:
	int nr_zadania;
	int nr_maszyny;
	int czas_wykonania;
	int suma_czasu;
public:
	Zadania(int nr_zadania, int nr_maszyny, int czas_wykonania, int suma_czasu)
	{
		this->nr_zadania = nr_zadania;
		this->nr_maszyny = nr_maszyny;
		this->czas_wykonania = czas_wykonania;
		this->suma_czasu = suma_czasu;
	}
	
	int wez_zadanie() { return nr_zadania; }
	int wez_maszyne() { return nr_maszyny; }
	int wez_czas() { return czas_wykonania; }
	int wez_suma_czasu() const { return suma_czasu; }	//Const potrzebny do przeladowania operatora
	int ustaw_zadanie(int nr_zadania) { this->nr_zadania = nr_zadania; }
	int ustaw_czas(int czas_wykonania) { this->czas_wykonania = czas_wykonania; }
	void ustaw_suma_czasu(int suma_czasu) { this->suma_czasu = suma_czasu; }
	void pokaz_zadanie() {
		cout << "Nr zadania: " << nr_zadania
			<< " czas wykonania: " << czas_wykonania
			<< " nr maszyny: " << nr_maszyny
			<< " suma czasow: " << suma_czasu << endl;
	}
};

bool operator<(const Zadania &z1, const Zadania &z2)		//https://www.walletfox.com/course/sortvectorofcustomobjects.php
{
	return z1.wez_suma_czasu() > z2.wez_suma_czasu();
}

vector<Zadania> zaladuj_zadania() {
	int il_zadan, il_maszyn, k;
	int suma = 0;

	vector<Zadania> kontener_zadan;
	ifstream plik;
	plik.open("dane.txt");
	plik >> il_zadan >> il_maszyn;

	//Dla kazdego wiersza (zadanie)
	for (int i = 0; i < il_zadan; i++)
	{
			//Dla kazdej kolumny (maszyny)
			for (int j = 0; j < il_maszyn; ++j)
			{
				plik >> k;											//Wczytuj czas wykonywania sie zadania
				kontener_zadan.push_back(Zadania(i + 1, j + 1, k, 0)); //I odpowiednio zappisz wszystkie wartosci
			}
	}

	//For ktory liczy sume czasu wykonania calego zadania, i przypisuja ta wartosc kazdemu obiektowi wykonujacemu to zadanie
	for (int i = 0; i < il_zadan*il_maszyn; ++i)
	{
		suma = suma + kontener_zadan[i].wez_czas();

		if ((i + 1) % il_maszyn == 0 && i != 0) //Warunek ktory sprawdza czy wszystkie czasy dla kazdego zadania sie wykonaly 
		{
			int temp = kontener_zadan[i].wez_zadanie(); //Dla konkretnego zadania
			for (int j = 0; j < il_zadan*il_maszyn; j++)//Poszukujemy po wszystkich wektorach
			{
				if (kontener_zadan[j].wez_zadanie() == temp)//Jezeli operacja ma taki sam numer zadania, jak wyliczona dla zadania suma wszystkich operacji
					kontener_zadan[j].ustaw_suma_czasu(suma);	//Przypisujemy wartosc polu suma_czasu, sume wykonywania tego zadania
			}
			suma = 0;
		}
	}
	plik.close();

	return kontener_zadan;
}

vector<Zadania> NehAlgorithm(vector<Zadania> kontener_zadan)
{
	vector<Zadania> posortowane_zadania = kontener_zadan;
	vector<Zadania> kontener = kontener_zadan;
	
	sort(posortowane_zadania.begin(), posortowane_zadania.end());

	
	
	


	return posortowane_zadania;
}




int main()
{
	vector<Zadania> kontener_zadan = zaladuj_zadania();
	vector<Zadania> posortowane = zaladuj_zadania();


	cout << "Breakpoint" << endl;

	system("pause");
	return 0;
}


//Zasada dzia³ania NEH:
//1. Wyznaczamy w(j) (j - nr zadania), czyli sume czasow wszystkich operacji na wszystkich maszynach dla danego zadania
//   np. w(1)= 9, w(2)=10, w(3)=6, w(4)=9 
//2. Posortowac w(j) nierosn¹co, tak aby zadania o najwiekszej sumie czasow wykonania byly na poczatku -> 2 1 4 3
//3. Nastepnie w w.w kolejnosci wsadzamy zadania na kazdej mozliwej pozycji, i wybieramy to o najmniejszej 
//   wartosci Cmax, zaczynaj¹c od zadania o najwiekszej sumie czasów.
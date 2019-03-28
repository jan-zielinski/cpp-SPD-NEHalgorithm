#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <time.h>

using namespace std;
int il_maszyn;
int il_zadan;


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
	int  k;
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
			plik >> k;
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




vector<vector<Zadania>> podziel_na_maszyny(vector<Zadania> kontener) 
{
	vector<vector<Zadania>> podzielone(il_maszyn);

	for (int j = 0; j < il_maszyn; j++) {
		for (int i = 0; i < kontener.size(); i++) {
			if (kontener[i].wez_maszyne() == j + 1)
				podzielone[j].push_back(kontener[i]);
		}

	}

	return  podzielone;
}

void wypisz_maszyny(vector<vector<Zadania>> podzielone) 
{
	for (int i = 0; i < podzielone.size(); i++) {
		cout << endl << "MASZYNA NR:" << i + 1 << endl;
		for (int j = 0; j < podzielone[i].size(); j++) {
			podzielone[i][j].pokaz_zadanie();
		}

	}

}


int policz_Cmax(vector<vector<Zadania>> maszyna, vector<int>kolejnosc) 
{
	int time1 = 0, time2 = 0;
	vector<int> czas_maszyny(maszyna.size(), 0);




	//Petla ustawia /kolejnosc/ wykonywania zadan na maszynach
	for (int i = 0; i < maszyna.size(); i++) {

		for (int j = 0; j < kolejnosc.size(); j++) {
			for (int k = 0; k < maszyna[i].size(); k++) {
				if (maszyna[i][k].wez_zadanie() == kolejnosc[j])
				{
					Zadania tmp = maszyna[i][k];
					maszyna[i].erase(maszyna[i].begin() + k);
					maszyna[i].insert(maszyna[i].end(), tmp);
				}
			}
		}
	}
	

	//Obliczamy Cmax

	for (int j = 0; j < maszyna[0].size() ; j++) 
	{

		czas_maszyny[0] += maszyna[0][j].wez_czas();
		if (j == 0) 
		{
			for (int k = 1; k < maszyna.size(); k++) 
			{
				if (j == 0) 
				{
					czas_maszyny[k] += czas_maszyny[k - 1] + maszyna[k][j].wez_czas();
				}
			}
		}

		for (int h = 0; h < maszyna.size() - 1; h++) 
		{
			if (j != 0 && czas_maszyny[h] <= czas_maszyny[h + 1]) 
			{
				czas_maszyny[h + 1] = czas_maszyny[h + 1] + maszyna[h + 1][j].wez_czas();
			}
			else if (j != 0 && czas_maszyny[h] > czas_maszyny[h + 1]) 
			{
				czas_maszyny[h + 1] += (czas_maszyny[h] - czas_maszyny[h + 1]) + maszyna[h + 1][j].wez_czas();
			}
		}
	}



	for (int i = 0; i < czas_maszyny.size(); i++)
	{
		//cout << "CZAS NA MASZYNIE NR: " << i + 1 << " = " << czas_maszyny[i] << " " << endl;
		if (i == czas_maszyny.size() - 1)
		{
			time2 = czas_maszyny[i];
		}
	}



	// wypisz_maszyny(maszyna);
	return time2;
}

vector<Zadania> NehAlgorithm(vector<Zadania> kontener_zadan)
{
	vector <int> kolejnosc;
	vector <Zadania> posortowane_zadania = kontener_zadan;
	sort(posortowane_zadania.begin(), posortowane_zadania.end());

	for (int i = 0; i < posortowane_zadania.size(); i++)
	{
		kolejnosc.push_back(posortowane_zadania[i].wez_zadanie());
		i += il_maszyn - 1;
	}

	cout << "||KOLEJNSOC ROZMIAR: " << kolejnosc.size() << endl;
	auto cont = true;
	vector <int> kolejnoscPetla;
	vector <Zadania> kontenerPetla;
	vector <Zadania> najlepszyKontener;
	vector <int> najlepszaKolejnosc;
	int iterator = 0;
	int cmax;
	int best = 99999;
	while (iterator != kolejnosc.size())	// Kolejnosc 2 1 4 3
	{
		{		
			for (int i = 0; i <= kolejnoscPetla.size(); i++)
			{
				kolejnoscPetla.insert(kolejnoscPetla.begin() + i, kolejnosc[iterator]);		//Pozniej usun z tego miejsca, i wstaw w kolejne	

				for (int j = 0; j < posortowane_zadania.size(); j++)
				{
					if (posortowane_zadania[j].wez_zadanie() == kolejnosc[iterator])
					{
						kontenerPetla.insert(kontenerPetla.begin() + (il_maszyn*i), posortowane_zadania[j]);
					}
				}

				cmax = policz_Cmax(podziel_na_maszyny(kontenerPetla), kolejnoscPetla);

				if (i == 0 || cmax < best)
				{
					best = cmax;
					najlepszaKolejnosc = kolejnoscPetla;
					najlepszyKontener = kontenerPetla;
				}
				kolejnoscPetla.erase(kolejnoscPetla.begin() + 1 * i);
				kontenerPetla.erase(kontenerPetla.begin() + (il_maszyn * i), kontenerPetla.begin() + (il_maszyn * i) + il_maszyn);
			}
			kolejnoscPetla = najlepszaKolejnosc;
			kontenerPetla = najlepszyKontener;
			iterator += 1;				
		}
	}

	cout << "Najlepszy wynik: " << best << " dla kolejnosci : ";
	for (int k = 0; k < najlepszaKolejnosc.size(); k++)
		cout << najlepszaKolejnosc[k] << " ";
	cout << "\n" << best << endl;
	cout << "breakpoint";


	return posortowane_zadania;
}

int main()
{
	clock_t zegar;
	double czas_trwania;
	vector<Zadania> kontener_zadan = zaladuj_zadania();
	zegar = clock();
	vector<Zadania> posortowane = NehAlgorithm(kontener_zadan);
	czas_trwania = (clock() - zegar) / (double)CLOCKS_PER_SEC;
	cout << "\nCzas trwania: " << czas_trwania << endl;

	

	system("pause");

	return 0;
}


//Zasada dzia³ania NEH:
//1. Wyznaczamy w(j) (j - nr zadania), czyli sume czasow wszystkich operacji na wszystkich maszynach dla danego zadania
//   np. w(1)= 9, w(2)=10, w(3)=6, w(4)=9
//2. Posortowac w(j) nierosnšco, tak aby zadania o najwiekszej sumie czasow wykonania byly na poczatku -> 2 1 4 3
//3. Nastepnie w w.w kolejnosci wsadzamy zadania na kazdej mozliwej pozycji, i wybieramy to o najmniejszej
//   wartosci Cmax, zaczynajšc od zadania o najwiekszej sumie czasów.
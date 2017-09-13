// Maze.cpp: Definiuje punkt wejścia dla aplikacji konsolowej.
//

#include "stdafx.h"
#include "cls.h"

using namespace std;

HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

int menu(int game)
{
	char wybor, nick_wynik[12];
	int poziomy_wynik, punkty_wynik;
	cls(hStdout);
	cout << "-----------------\n";
	cout << "1.Nowa gra\n";
	cout << "2.Pomoc\n";
	cout << "3.Najlepszy wynik\n";
	cout << "-----------------\n";
	cout << "ESC aby wyjsc\n";
	wybor = _getch();
	switch (wybor)
	{
	case 49: {game = 1; }; break;       ///// 49 to kod ASCII 1
	case 50:                       ///// 50 to kod ASCII 2
	{
		cls(hStdout);
		cout << "1.OPIS-----------------------------------------------------------.\n";
		cout << "Celem tej prostej gry jest przejscie postacia przez labirynt.\n";
		cout << "Zadanie to jest o tyle proste, ze w grze nie istnieje zaden limit\n";
		cout << "czasowy czy ruchowy, ani zadni przeciwnicy.\n";
		cout << "2.LEGENDA--------------------------------------------------------.\n";
		cout << "H - postac ktora porusza gracz.\n";
		cout << ". - przestrzen po ktorej postac moze sie poruszac.\n";
		cout << "# - sciana.\n";
		cout << "O - wyjscie z labiryntu.\n";
		cout << "-----------------------------------------------------------------.\n";
		_getch();
		menu(game);
	}; break;
	case 51:                    ///// 
	{
		ifstream wczytaj_wynik("Tabela_wynikow.txt");
		wczytaj_wynik >> poziomy_wynik;
		wczytaj_wynik >> punkty_wynik;
		wczytaj_wynik >> nick_wynik;
		wczytaj_wynik.close();
		cout << "Ilosc poziomow: " << poziomy_wynik << " Ilosc punktow: " << punkty_wynik << " Nick:" << nick_wynik;
		_getch();
		menu(game);
	}; break;
	case 27:
	{
		game = 0;
		cls(hStdout);
		cout << "Przerwano\n";
		system("PAUSE");
	}; break;
	}
	cls(hStdout);
	// cout << game;
	// getch();
	return(game = false);
}



int main()
{
	char tab[20][30][60], i = 1, klawisz, pom, imie[12], imie_wynik[12];
	int game = 1;    ///Zmienna prawda-falsz - jesli game=false to program glowny nie wczyta menu wyboru poziomu trudnosci
	int x = 0, y = 0, z = 0, ilosc_ruchow = 0, klawisz_pom = '0', punktacja = 0, ilosc_poziomow = 0, poziom = 0, wym1 = 0, wym2 = 0, punktacja_wynik = 0, ilosc_poziomow_wynik = 0; ///x,y,z-pomocnicze do zapisywania pozycji "postaci"; ilosc_ruchow=na jej podstawie obliczana jest punktacja; ilosc_poziomow=zlicza ile poziomow jest w pliku, wym1,wym2-zaleznie od poziomu trudnosci dostosowuja petle do wyswietlania odpowiednich wielkosci map
	system("chcp 1252");
	menu(game);
	//cout << game;
	//getch();
	if (game != 0)      //////Jesli w procedurze menu nie nacisnieto klawisz ESC menu wyboru poziomu trudnosci otworzy sie
	{
		cout << "Wybierz poziom trudnosci: \n";
		cout << "1.Latwy (10x20)\n";
		cout << "2.Trudny (20x40)\n";
		//cout << "3.Trudny (30x60)\n";
		poziom = _getch();
		switch (poziom)
		{
		case 49:   /////49 to kod ASCII 1  
		{
			wym1 = 10;
			wym2 = 20;
		}; break;
		case 50:     /////50 to kod ASCII 2
		{
			wym1 = 20;
			wym2 = 40;
		}; break;
		/*case 51:   /////51 to kod ASCII 3
		{
		wym1=30;
		wym2=60;
		}; break;*/
		}
	}
	else
	{
		system("PAUSE");
		return EXIT_SUCCESS;
	}
	/////////////////////////////////////////////////////////////////////////////Wczytanie mapy z pliku do tablicy tab 
	cls(hStdout);
	ifstream wejscie_easy("Level_easy.txt");
	if (!wejscie_easy)
	{
		cout << "Nie mozna otworzyc pliku\n";
		_getch();
		return 1;
	}
	ifstream wejscie_medium("Level_medium.txt");
	if (!wejscie_medium)
	{
		cout << "Nie mozna otworzyc pliku\n";
		_getch();
		return 1;
	}
	ifstream wejscie_hard("Level_hard.txt");
	if (!wejscie_hard)
	{
		cout << "Nie mozna otworzyc pliku\n";
		_getch();
		return 1;
	}
	////////////////////////////////////////////////////////////////////////Wrzucanie mapy z pliku do tablicy tab
	for (i = 0; i<20; i++)
	{
		for (int j = 0; j<wym1; j++)
		{
			for (int k = 0; k<wym2; k++)
			{
				if (wym1 == 10)
				{
					wejscie_easy >> tab[i][j][k];
				}
				else if (wym1 == 20)
				{
					wejscie_medium >> tab[i][j][k];
				}
				else
				{
					wejscie_hard >> tab[i][j][k];
				}
			}
		}
		if (tab[i][0][2] == '#')			///Liczenie ile poziomow znajduje sie w pliku (element [i][0][2] ZAWSZE jest czescia obramowania poziomu czyli MUSI to byc # - jezeli nie to poziom nie istnieje)
		{
			ilosc_poziomow = ilosc_poziomow + 1; /*cout << "Zwiekszono ilosc pozimow do " << ilosc_poziomow << "\n"; getch();*/
		}
	}
	wejscie_easy.close();
	wejscie_medium.close();
	wejscie_hard.close();
	i = 0;
	///////////////////////////////////////////////////////////////////////wyswietlanie mapy
	do
	{
		do
		{
			ilosc_ruchow = ilosc_ruchow + 1;
			cls(hStdout);
			if (i == ilosc_poziomow) break; ///Po ukonczeniu ostatniego dostepnego w pliku poziomu program konczy wychodzi z petli (bez wyswietlania pustej mapy jak bez tego warunku)
			cout << "Ruch: " << ilosc_ruchow << "\n" << "\n";
			for (int j = 0; j<wym1; j++)
			{
				for (int k = 0; k<wym2; k++)
				{
					if (k%wym2 != 0)
					{
						cout << tab[i][j][k];
					}
					else
						cout << tab[i][j][k] << "\n";
				}
			}
			cout << "#";
			///////////////////////////////////////////////////////////////////////Wyszukiwanie "postaci" w tablicy
			for (int j = 0; j<wym1; j++)
			{
				for (int k = 0; k<wym2; k++)
				{
					if (wym1 == 10)
					{
						wejscie_easy >> tab[i][j][k];
						if (tab[i][j][k] == 'H')
						{
							x = i;
							y = j;
							z = k;
						}
					}
					else if (wym1 == 20)
					{
						wejscie_medium >> tab[i][j][k];
						if (tab[i][j][k] == 'H')
						{
							x = i;
							y = j;
							z = k;
						}
					}
					else
					{
						wejscie_hard >> tab[i][j][k];
						if (tab[i][j][k] == 'H')
						{
							x = i;
							y = j;
							z = k;
						}
					}
				} // koniec petli for sprawdzajacej wartosc k
			} // koniec petli for sprawdzajacej wartosc j
			  ///////////////////////////////////////////////////////////////////////Sterowanie_"postacia"
			klawisz = _getch();
			switch ((int)klawisz)
			{
			case 97:                      ///////////////////////////////////////// 97 to kod ASCII dla 'a', przesuniecie "postaci" o jeden element w lewo
			{
				//cout << (int)klawisz;
				if (tab[x][y][z - 1] != '#')
				{
					pom = tab[x][y][z - 1];
					tab[x][y][z - 1] = tab[x][y][z];
					tab[x][y][z] = pom;
				}
				else
				{
					cout << "\n" << "Ruch niedozwolony!\n";
					ilosc_ruchow = ilosc_ruchow - 1;
					_getch();
				}
			}; break;
			case 100:                     //////////////////////////////////////// 100 to kod ASCII dla 'd', przesuniecie "postaci" o jeden element w prawo
			{
				if (tab[x][y][z + 1] != '#')
				{
					pom = tab[x][y][z + 1];
					//std::cout << "<<<<" << tab[x][y][z] << " tab[" << x << "][" << y << "][" << z << "] ... " << tab[x][y][z + 1] << " tab[" << x << "][" << y << "][" << z + 1 << "]>>>>";
					tab[x][y][z + 1] = tab[x][y][z];
					tab[x][y][z] = pom;
					//std::cout << "<<<<" << pom << " tab[" << x << "][" << y << "][" << z << "]>>>>";
					//_getch();
				}
				else
				{
					cout << "\n" << "Ruch niedozwolony!\n";
					ilosc_ruchow = ilosc_ruchow - 1;
					_getch();
				}
			}; break;
			case 115:                     //////////////////////////////////////// 115 to kod ASCII dla 's', przesuniecie "postaci" o jeden element w dol
			{
				if (tab[x][y + 1][z] != '#')
				{
					pom = tab[x][y + 1][z];
					tab[x][y + 1][z] = tab[x][y][z];
					tab[x][y][z] = pom;
				}
				else
				{
					cout << "\n" << "Ruch niedozwolony!\n";
					ilosc_ruchow = ilosc_ruchow - 1;
					_getch();
				}
			}; break;
			case 119:                     //////////////////////////////////////// 119 to kod ASCII dla 'w', przesuniecie "postaci" o jeden element w gore
			{
				if (tab[x][y - 1][z] != '#')
				{
					pom = tab[x][y - 1][z];
					tab[x][y - 1][z] = tab[x][y][z];
					tab[x][y][z] = pom;
				}
				else
				{
					cout << "\n" << "Ruch niedozwolony!\n";
					ilosc_ruchow = ilosc_ruchow - 1;
					_getch();
				}
			}; break;
			case 27:                   ////////////////////////////////////////// 27 to kod ASCII dla 'ESC', wyjscie z programu
			{
				cls(hStdout);
				cout << "Przerwano\n";
				cout << "Wykonano " << ilosc_ruchow << " ruchow\n";
				system("PAUSE");
				return EXIT_SUCCESS;
			}; break;
			}   // koniec switcha (przesuwanie "postaci")
		} while (tab[i][1][2] == 'O'); // koniec instrukcji do .. while poziom
		i++; ////zwiekszanie i powoduje przejscie do nastepnego poziomu
	}///////////////////////koniec pętli do while, w której jest główna czesc gry
	while (i - 1 != ilosc_poziomow);
	if (x != 0)
	{
		cls(hStdout);
		cout << "Ilosc poziomow: " << ilosc_poziomow;
		punktacja = punktacja + ilosc_ruchow;
		cout << "\nZdobyles " << punktacja << " punktow.\n";
		cout << "Podaj swoj nick (max 12 znakow): ";
		cin >> imie;
		/////////////////////////////////////////////////////////////////////////////////// Wczytanie najlepszego wyniku
		ifstream wczytaj_wynik("Tabela_wynikow.txt");
		if (!wczytaj_wynik)
		{
			ofstream zapis("Tabela_wynikow.txt");             ////////////////////////////// Zapis NOWEGO najlepszego wyniku
			zapis << ilosc_poziomow << "\n" << punktacja << "\n" << imie;
			zapis.close();
		};
		wczytaj_wynik >> ilosc_poziomow_wynik;
		wczytaj_wynik >> punktacja_wynik;
		wczytaj_wynik >> imie_wynik;
		wczytaj_wynik.close();
		if (ilosc_poziomow_wynik >= ilosc_poziomow)
		{
			if (punktacja_wynik > punktacja)
			{
				cout << "!!!!! Gratulacje! Twoj wynik byl najlepszy !!!!!";
				ofstream zapis("Tabela_wynikow.txt");             ////////////////////////////// Zapis NOWEGO najlepszego wyniku
				zapis << ilosc_poziomow << "\n" << punktacja << "\n" << imie;
				zapis.close();
				_getch();
			}
			else
			{
				cls(hStdout);
				cout << "Najlepszy wynik: \n";
				cout << "Ilosc poziomow: " << ilosc_poziomow_wynik << " Ilosc punktow: " << punktacja_wynik << " Nick:" << imie_wynik;
				_getch();
				system("PAUSE");
				return EXIT_SUCCESS;
			}
		}
		else if (punktacja_wynik > punktacja)
		{
			cout << "!!!!! Gratulacje! Twoj wynik byl najlepszy !!!!!";
			ofstream zapis("Tabela_wynikow.txt");             ////////////////////////////// Zapis NOWEGO najlepszego wyniku
			zapis << ilosc_poziomow << "\n" << punktacja << "\n" << imie;
			zapis.close();
			_getch();
		}
	}
}    //koniec main

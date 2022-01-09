#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include<stdarg.h>
#include <conio.h>
typedef struct Card
{
	char Bez[10];
	int Dmg;
	int HP;
	double Spd;
	struct Card* pNext;
} struCard;

int GetAnzahlCard(struCard* c) {
	int count = 0;
	for (c; c != NULL; c =  c->pNext) count++;
	return count;
}

void back(const char* pFormat = NULL, ...)
{
	char OutText[255];
	va_list args;

	va_start(args, pFormat);

	if (pFormat != NULL) {
		vsprintf_s(OutText, pFormat, args);
		printf("%s\n", OutText);
	}
	printf("Druecke eine beliebige Taste um zurueck ins Hauptmenu zu kommen.");
	system("pause > nul");
}
struCard* CreateCard(const char* bez, int Dmg, int HP, double Spd) {
	//Erstellt Liste
	struCard* pNew = (struCard*)malloc(sizeof(struCard));
	if (pNew == NULL) exit(-1);

	pNew->pNext = NULL;

	//Festlegung Bezeichnung
	strcpy_s(pNew->Bez, bez);

	//Festlegung Stats(Dmg,HP,Spd) 
	pNew->Dmg = Dmg;
	pNew->HP = HP;
	pNew->Spd = Spd;

	return pNew;
}

void AddCard(struCard* first, struCard* toAdd) {
	for (first; first->pNext != NULL; first = first->pNext);

	first->pNext = toAdd;
}

//Funktion zum neumischen der Spielerkarten
void MixCard(struCard** Main, struCard** Player1, struCard** Player2) {
	//Mischen
	srand((unsigned)time(NULL));
	const int Anz = 10;
	int Rand1[Anz];
	int Rand2[Anz];

	for (int i = 0; i < Anz; i++) {
		Rand1[i] = -1;
		Rand2[i] = -1;
	}

	int Rand1SpaceIndex = 0;
	int Rand2SpaceIndex = 0;
	do {
		// Random-Zahl bestimmen
		int RandomZahl = rand() % (2 * Anz);
		// Suchen ob Zahl bereits verwendet
		bool Gefunden = false;
		for (int i = 0; i < Anz && !Gefunden; i++) Gefunden = Rand1[i] == RandomZahl || Rand2[i] == RandomZahl;

		// In welchem Array hat es noch Platz
		for (Rand1SpaceIndex = 0; Rand1SpaceIndex < Anz && Rand1[Rand1SpaceIndex] != -1; Rand1SpaceIndex++);
		for (Rand2SpaceIndex = 0; Rand2SpaceIndex < Anz && Rand2[Rand2SpaceIndex] != -1; Rand2SpaceIndex++);

		// Wenn nicht verwendet, Zahl eintragen
		if (!Gefunden) {
			// Random Zahl in Array eintragen
			if (Rand1SpaceIndex < Anz) Rand1[Rand1SpaceIndex] = RandomZahl;
			else if (Rand2SpaceIndex < Anz) Rand2[Rand2SpaceIndex] = RandomZahl;
		}
	} while (Rand1SpaceIndex < Anz || Rand2SpaceIndex < Anz);

	//Speicherreservierung
	*Player1 = (struCard*)malloc(sizeof(struCard));
	*Player2 = (struCard*)malloc(sizeof(struCard));
	struCard* tPlayer1 = *Player1;
	struCard* tPlayer2 = *Player2;
	
	//Karte an richtiger Stelle anhenken
	int i = 0;
	struCard* tMain = *Main;
	do {
		tPlayer1 = *Player1;
		tPlayer2 = *Player2;
		//Zur Stelle im Spielerstapel springen (und Speicher für pNext reservieren)
		for (int j = i; j != 0; tPlayer1 = tPlayer1->pNext, j--) if (tPlayer1->pNext == NULL) tPlayer1->pNext = (struCard*)malloc(sizeof(struCard));
		for (int j = i; j != 0; tPlayer2 = tPlayer2->pNext, j--) if (tPlayer2->pNext == NULL) tPlayer2->pNext = (struCard*)malloc(sizeof(struCard));

		//Karten aus Hauptstapel holen und Pointer an richtige Stelle setzten
		tMain = *Main;
		for (int j = Rand1[i]; j != 0; tMain = tMain->pNext, j--);
		*tPlayer1 = *tMain;
		tPlayer1->pNext = NULL;
		
		tMain = *Main;
		for (int j = Rand2[i]; j != 0; tMain = tMain->pNext, j--);
		*tPlayer2 = *tMain;
		tPlayer2->pNext = NULL;

		i++;

	} while (i < Anz);
}

//Entfernt die forderste Karte vom Spieler und hängt sie beim hinteren Spieler an, vorderste Karte vom Gewinner wird auch hinten angesetzt
void ExchangeCard(bool won, struCard** p1, struCard** p2) {
	if (won) {
		//Übertragung Karte
		struCard* temp = *p2;
		*p2 = (*p2)->pNext;
		temp->pNext = NULL;
		AddCard(*p1, temp);
		//Vorderste Karte nach hinten
		temp = *p1;
		*p1 = (*p1)->pNext;
		temp->pNext = NULL;
		AddCard(*p1, temp);
	}
	else {
		//Übertragung Karte
		struCard* temp = *p1;
		*p1 = (*p1)->pNext;
		temp->pNext = NULL;
		AddCard(*p2, temp);
		//Vorderste Karte nach hinten
		temp = *p2;
		*p2 = (*p2)->pNext;
		temp->pNext = NULL;
		AddCard(*p2, temp);
	}
	
}

void PrintCard(struCard* main) {
	printf("Bez, DMG, HP, SPD\n");
	for (struCard* i = main; i != NULL; i = i->pNext) printf("%s,  %i,  %i, %lf\n", i->Bez, i->Dmg, i->HP, i->Spd);
}

//Funktion zum vergleichen der Werte, falls Spieler gewinnt, wird "true" zurückgegeben
bool CompareValue(struCard p1, struCard p2, short i) {
	switch (i) {
		case 1: {
			return (p1.Dmg > p2.Dmg);
		}
		case 2: {
			return (p1.HP > p2.HP);
		}
		case 3: {
			return (p1.Spd > p2.Spd);
		}
	}
}


void StartGame(struCard* Main, struCard* Player1, struCard* Player2) {
	bool end = false;
	bool restart = true;
	short Selection = 0;

	if (Player1 == NULL, Player2 == NULL) exit(-1);
	printf("Spiel Erfolgreich gestartet!\n\n");

	do {
		struCard* m = Main;
		struCard* p1 = Player1;
		struCard* p2 = Player2;
		MixCard(&m, &p1, &p2);
		do {
			system("CLS");
			printf("Anzahl Karten: %i\nKarte auf der Hand: \t%s, \t%i, \t%i, \t%lf\n\n Waelen Sie den Wert aus, mit dem Sie spielen wollen:\n\t1) DMG\n\t2) HP\n\t3) SPD\n", GetAnzahlCard(p1), p1->Bez, p1->Dmg, p1->HP, p1->Spd);
			Selection = _getche() - 48;
			if (Selection <= 0 || Selection > 3) back("Ungueltige Auswahl, bitte waehle eine Zahl zwischen 1 bis 3.");
			else ExchangeCard((CompareValue(*p1, *p2, Selection)), &p1, &p2);
			if (p1 == NULL || p2 == NULL) end = true;
		} while (!end);
		if (Player2 == NULL) back("\nHerlichen Glueckwunsch, Sie haben diese Runde gewonnen!\n Noch ein Spiel?\n\t1) Ja\n\t2) Nein");
		else back("\nSchade, Sie haben diese Runde leider verloren.\n Noch ein Spiel?\n\t1) Ja\n\t2) Nein");
		Selection = _getch() - 48;
		if (Selection > 2 || Selection >= 0) {
			back("Aufwahl ungueltig, bitte waehle 1 oder 2.");
			system("CLS");
		}
		else {
			if (Selection == 1) restart = true;
			else restart = false;
		}
	} while (restart);
}

int main() {  
	struCard* main = CreateCard("Golem", 2, 10, 3.5);		//1  Karte
	AddCard(main, CreateCard("Assasin", 8, 2, 7.4));		//2	 Karte
	AddCard(main, CreateCard("Archer", 7, 4, 6.2));			//3  Karte
	AddCard(main, CreateCard("Fighter", 6, 6, 4.4));		//4  Karte
	AddCard(main, CreateCard("Wizard", 10, 3, 5.5));		//5	 Karte
	AddCard(main, CreateCard("Monk", 7, 5, 7.9));			//6	 Karte
	AddCard(main, CreateCard("Thief", 0, 4, 10.1));			//7	 Karte
	AddCard(main, CreateCard("Farmer", 3, 2, 5.5));			//8	 Karte
	AddCard(main, CreateCard("King", 7, 8, 5.5));			//9	 Karte
	AddCard(main, CreateCard("Smith", 4, 6, 3));			//10 Karte
	AddCard(main, CreateCard("Smith", 4, 6, 3));			//11 Karte
	AddCard(main, CreateCard("Smith", 4, 6, 3));			//12 Karte
	AddCard(main, CreateCard("Smith", 4, 6, 3));			//13 Karte
	AddCard(main, CreateCard("Smith", 4, 6, 3));			//14 Karte
	AddCard(main, CreateCard("Smith", 4, 6, 3));			//15 Karte
	AddCard(main, CreateCard("Smith", 4, 6, 3));			//16 Karte
	AddCard(main, CreateCard("Smith", 4, 6, 3));			//17 Karte
	AddCard(main, CreateCard("Smith", 4, 6, 3));			//18 Karte
	AddCard(main, CreateCard("Smith", 4, 6, 3));			//19 Karte
	AddCard(main, CreateCard("Smith", 4, 6, 3));			//10 Karte

	struCard* Player1 = NULL;
	struCard* Player2 = NULL;
	bool Success = false;
	short Selection;
	do {
		printf("******************** Quartett ********************\nFolgende Optionen stehen zur Verfuegung: \n\t1) Spiel Starten \n\t2) Alle Karten zeigen \n\t3) Programm beenden\n\nWaehlen Sie einer der aufgelisteten Optionen auf, indem Sie die zugewiesenen Zahlen eingeben.\n***************************************************\n\n");
		Selection = _getche() - 48;
		switch (Selection) {
			case 1: {
					StartGame(main, Player1, Player2);
			}
			case 2: {
				PrintCard(main);
				Success = true;
			}
			case 3 : {
				MixCard(&main, &Player1, &Player2);
					Success = true;
			}
		}
	} while (!Success);
	printf("Bez, DMG, HP, SPD\n");
	printf("%s,  %i,  %i, %lf\n", Player1->Bez, Player1->Dmg, Player1->HP, Player1->Spd);
	printf("%s,  %i,  %i, %lf\n", Player1->pNext->Bez, Player1->pNext->Dmg, Player1->pNext->HP, Player1->pNext->Spd);
	printf("%s,  %i,  %i, %lf\n", Player1->pNext->pNext->Bez, Player1->pNext->pNext->Dmg, Player1->pNext->pNext->HP, Player1->pNext->pNext->Spd);
	printf("%s,  %i,  %i, %lf\n", Player1->pNext->pNext->pNext->Bez, Player1->pNext->pNext->pNext->Dmg, Player1->pNext->pNext->pNext->HP, Player1->pNext->pNext->pNext->Spd);
	printf("%s,  %i,  %i, %lf\n", Player1->pNext->pNext->pNext->pNext->Bez, Player1->pNext->pNext->pNext->pNext->Dmg, Player1->pNext->pNext->pNext->pNext->HP, Player1->pNext->pNext->pNext->pNext->Spd);
	printf("Spieler2\n\n");
	printf("%s,  %i,  %i, %lf\n", Player2->Bez, Player2->Dmg, Player2->HP, Player2->Spd);
	printf("%s,  %i,  %i, %lf\n", Player2->pNext->Bez, Player2->pNext->Dmg, Player2->pNext->HP, Player2->pNext->Spd);
	printf("%s,  %i,  %i, %lf\n", Player2->pNext->pNext->Bez, Player2->pNext->pNext->Dmg, Player2->pNext->pNext->HP, Player2->pNext->pNext->Spd);
	printf("%s,  %i,  %i, %lf\n", Player2->pNext->pNext->pNext->Bez, Player2->pNext->pNext->pNext->Dmg, Player2->pNext->pNext->pNext->HP, Player2->pNext->pNext->pNext->Spd);
	printf("%s,  %i,  %i, %lf\n", Player2->pNext->pNext->pNext->pNext->Bez, Player2->pNext->pNext->pNext->pNext->Dmg, Player2->pNext->pNext->pNext->pNext->HP, Player2->pNext->pNext->pNext->pNext->Spd);
	printf("\n");

	printf("%s,  %i,  %i, %lf\n", main->Bez, main->Dmg, main->HP, main->Spd);
}

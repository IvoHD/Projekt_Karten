#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
//struCard** List[9];
typedef struct Card
{
	char Bez[10];
	int Dmg;
	int HP;
	double Spd;
	struct Card* pNext;
} struCard;

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
	const int Anz = 5;
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

//Entfernt die forderste Karte vom Spieler und hängt sie beim hinteren Spieler an
struCard* RemoveCard(struCard* looser, struCard* winner) {
	struCard* temp = looser;
	temp->pNext = NULL;

	AddCard(winner, temp);

	return looser->pNext;
}



int main() {

	struCard* main = CreateCard("Golem", 2, 10, 3.5);		//1
	AddCard(main, CreateCard("Assasin", 8, 2, 7.4));		//2
	AddCard(main, CreateCard("Archer", 7, 4, 6.2));			//3
	AddCard(main, CreateCard("Fighter", 6, 6, 4.4));		//4
	AddCard(main, CreateCard("Wizard", 10, 3, 5.5));		//5
	AddCard(main, CreateCard("Monk", 7, 5, 7.9));			//6
	AddCard(main, CreateCard("Thief", 0, 4, 10.1));			//7
	AddCard(main, CreateCard("Farmer", 3, 2, 5.5));			//8
	AddCard(main, CreateCard("King", 7, 8, 5.5));			//9
	AddCard(main, CreateCard("Smith", 4, 6, 3));			//10

	struCard* Player1 = NULL;
	struCard* Player2 = NULL;

	MixCard(&main, &Player1, &Player2);
	system("PAUSE");
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
}

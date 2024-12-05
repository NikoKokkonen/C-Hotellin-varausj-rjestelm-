// Niko Kokkonen
// Hotelli varausj�rjestelm� C++ Kurssi

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

struct Huone {
    int numero;
    bool varattu;
    string varaajanNimi;
    int varausnumero;
    int tyyppi; // 1 = yhden hengen huone, 2 = kahden hengen huone
    float hinta;
};

// Funktio validin kokonaisluvun sy�tt�miseen
int validinumero() {
    int numero;
    while (true) {
        cin >> numero;
        if (cin.fail()) {
            cin.clear(); // Palautetaan cin virhetilasta
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Poistetaan virheellinen sy�te
            cout << "Virheellinen sy�te! Sy�t� numero." << endl;
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Poistetaan ylim��r�iset merkit
            return numero;
        }
    }
}

// Funktio varmistaen, ett� sy�te on kokonaisluku tietyll� alueella
int validinumerovalilta(int min, int max) {
    int numero;
    while (true) {
        numero = validinumero();
        if (numero >= min && numero <= max) {
            return numero;
        }
        cout << "Virheellinen sy�te! Sy�t� numero v�lilt� " << min << "-" << max << "." << endl;
    }
}

// Satunnaisen varausnumeron arpominen
int arvoVarausnumero() {
    return rand() % 90000 + 10000; // Satunnainen numero v�lill� 10000�99999
}

// Tarkista, onko huone varattu
bool tarkistaHuoneenVaraus(const vector<Huone>& huoneet, int numero) {
    for (const auto& huone : huoneet) {
        if (huone.numero == numero) {
            return huone.varattu;
        }
    }
    return false; // Jos huonetta ei l�ydy
}

// Varaa huone ja laske kokonaiskustannus
void varaaHuone(vector<Huone>& huoneet, int numero) {
    for (auto& huone : huoneet) {
        if (huone.numero == numero && !huone.varattu) {
            cout << "Anna varaajan nimi: ";
            string nimi;
            cin >> nimi;

            int varausnumero = arvoVarausnumero();
            huone.varattu = true;
            huone.varaajanNimi = nimi;
            huone.varausnumero = varausnumero;

            cout << "Kuinka monta y�t� haluat varata? ";
            int oidenMaara = validinumerovalilta(1, 365);

            // Arvotaan alennus
            int alennusProsentti = (rand() % 3) * 10; // 0 %, 10 % tai 20 %
            float kokonaishinta = huone.hinta * oidenMaara;
            float alennus = kokonaishinta * alennusProsentti / 100;
            float lopullinenHinta = kokonaishinta - alennus;

            cout << "\nHuone " << numero << " varattu onnistuneesti nimell�: " << nimi << endl;
            cout << "Varausnumero: " << varausnumero << endl;
            cout << "�iden m��r�: " << oidenMaara << endl;
            cout << "Huoneen hinta/y�: " << huone.hinta << " euroa" << endl;
            cout << "Kokonaishinta ilman alennusta: " << kokonaishinta << " euroa" << endl;
            cout << "Alennus: " << alennusProsentti << "%, eli -" << alennus << " euroa" << endl;
            cout << "Lopullinen hinta: " << lopullinenHinta << " euroa\n" << endl;

            return;
        }
    }
    cout << "Virhe! Huonetta ei voida varata." << endl;
}

// Hae varausta varausnumerolla tai varaajan nimell�
void haeVaraus(const vector<Huone>& huoneet) {
    cout << "Hae varausta (1: varausnumerolla, 2: varaajan nimell�): ";
    int valinta = validinumerovalilta(1, 2);

    if (valinta == 1) {
        cout << "Anna varausnumero: ";
        int varausnumero = validinumero();

        for (const auto& huone : huoneet) {
            if (huone.varausnumero == varausnumero) {
                cout << "Varaus l�ytyi! Varaaja: " << huone.varaajanNimi
                    << ", Huone: " << huone.numero << endl;
                return;
            }
        }
        cout << "Varausta ei l�ytynyt." << endl;
    }
    else if (valinta == 2) {
        cout << "Anna varaajan nimi: ";
        string nimi;
        cin >> nimi;

        for (const auto& huone : huoneet) {
            if (huone.varaajanNimi == nimi) {
                cout << "Varaus l�ytyi! Varaajan nimi: " << nimi
                    << ", Huone: " << huone.numero << endl;
                return;
            }
        }
        cout << "Varausta ei l�ytynyt." << endl;
    }
}

// Tulosta valikko
void tulostaValikko() {
    cout << "\n=== Hotellin varausj�rjestelm� ===" << endl;
    cout << "1. N�yt� vapaat huoneet" << endl;
    cout << "2. Tee varaus" << endl;
    cout << "3. Hae varaus" << endl;
    cout << "4. Poistu" << endl;
    cout << "Valitse toiminto: ";
}

// N�yt� vapaat huoneet
void naytaVapaatHuoneet(const vector<Huone>& huoneet) {
    cout << "\nVapaat huoneet:" << endl;
    for (const auto& huone : huoneet) {
        if (!huone.varattu) {
            cout << "Huone " << huone.numero << " (" << (huone.tyyppi == 1 ? "1hh" : "2hh")
                << "), Hinta: " << huone.hinta << " euroa/y�" << endl;
        }
    }
}

int main() {
    srand(time(0));
    setlocale(LC_ALL, "");   // Ota k�ytt��n ��kk�set
    cout.imbue(locale(""));  // UTF-8-lokalisointi tulostukseen
    cin.imbue(locale(""));   // UTF-8-lokalisointi sy�tteeseen

    // Luodaan huoneet
    vector<Huone> huoneet;
    int maara = rand() % 131 + 40; // Arvotaan huonem��r� v�lill� 40�170 (parillinen)
    if (maara % 2 != 0) maara++;  // Varmistetaan, ett� m��r� on parillinen

    for (int i = 1; i <= maara; i++) {
        Huone huone;
        huone.numero = i;
        huone.varattu = false;
        huone.varausnumero = 0;
        if (i <= maara / 2) {
            huone.tyyppi = 1; // Yhden hengen huone
            huone.hinta = 100;
        }
        else {
            huone.tyyppi = 2; // Kahden hengen huone
            huone.hinta = 150;
        }
        huoneet.push_back(huone);
    }

    // P��valikko
    while (true) {
        tulostaValikko();
        int valinta = validinumerovalilta(1, 4);

        switch (valinta) {
        case 1:
            naytaVapaatHuoneet(huoneet);
            break;
        case 2: {
            cout << "Anna varattavan huoneen numero: ";
            int numero = validinumerovalilta(1, maara);
            if (tarkistaHuoneenVaraus(huoneet, numero)) {
                cout << "Huone " << numero << " on jo varattu!" << endl;
            }
            else {
                varaaHuone(huoneet, numero);
            }
            break;
        }
        case 3:
            haeVaraus(huoneet);
            break;
        case 4:
            cout << "Kiitos ja n�kemiin!" << endl;
            return 0;
        default:
            cout << "Virheellinen valinta!" << endl;
        }
    }
}


// Mielest�ni ty� on suoritettu 3 arvosanan arvoisesti. Pieni� puutteita ja voisi olla omasta mielest�ni paremminkin tehty.
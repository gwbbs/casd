#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

/* 
Nota bene!! 
In Curses prima vengono piazzate le y e poi le x, l'inverso di come si usa normalmente
*/

// definisco area di gioco
#define LARGHEZZA 39 
#define ALTEZZA 18

// variabili globali
int vite = 1;
int monete = 10000;
int nemici = 6;
int punteggio = 0;

// posizione proiettile
int xProiettile; 
int yProiettile;

// posizione giocatore
int xGiocatore = LARGHEZZA / 2;
int yGiocatore = ALTEZZA - 1;

void inizializza() {
    clear(); // pulisce tutto il terminale
    initscr(); // inizia il terminale di curses
    curs_set(0); // non mostra il cursore nel terminale
    // stampa il riquadro di gioco
    for (int sopra = 0; sopra < LARGHEZZA; sopra++) {
        mvprintw(0, sopra, "-");
    }

    for (int sotto = 0; sotto < LARGHEZZA; sotto++) {
        mvprintw(ALTEZZA, sotto, "-");
    }

    for (int sx = 0; sx <= ALTEZZA; sx++) {
        mvprintw(sx, 0, "|");
    }

    for (int dx = 0; dx <= ALTEZZA; dx++) {
        mvprintw(dx, LARGHEZZA, "|");
    }
    mvprintw(20,0,"%d", punteggio);
}

void spawnNemici() { // stampa i nemici (ancora in fase di test poi verranno spostati in matrice bidimensionale)
    for (int i = 0; i < nemici; i += 2) {
        mvprintw(3, LARGHEZZA / 2 - i, "N");
    }
}

void sparaProiettile(int xProiettile, int yProiettile) {
    for (int i = yProiettile - 1; i >= 1; i--) { // il proiettile si sposta verso l'alto con un for e viene generato 
        usleep(10000); // il proiettile viene spostato in 10ms
        mvaddch(i, xProiettile, 'O');      // il proiettile viene spostato nella posizione (i, xProiettile)   
        refresh();
        mvaddch(i, xProiettile, ' ');   // cancella la scia che il proiettile ha "stampato"
        if (mvwinch(stdscr, i, xProiettile) =='N') { // se il proiettile in standardscreen (finestra corrente) "passa per" N allora aggiunge:
            punteggio += 10;
            monete += 5;
            refresh(); // aggiorna il terminale di curses
            break;
        }
    }
}

void apriNegozio() {
    char negoziochoose = '\0';

    while (true) {
        clear();
        printw("Monete: %d\n\n", monete);
        printw("Potenziamenti:\n\n");

        printw("\t1 ~Velocita' Proiettili (100)\n");
        printw("\tAumenta la velocita' del proiettile di +1.\n\n");

        printw("\t2 ~Vita Extra (400)\n");
        printw("\tAumenta le vite del giocatore.\n\n");

        printw("\t3 ~Boost Velocita' (400)\n");
        printw("\tAumenta la velocita' del player.\n\n");

        printw("Premi 'ESC' per tornare al menu principale.");

        negoziochoose = getch();

        if (negoziochoose == '1' && monete >= 100) {
            monete -= 100; // fase di test
        } else if (negoziochoose == '2' && monete >= 400) {
            monete -= 400; // sottrae 400 da "monete"
            vite += 1; // aggiunge 1 alla vita del giocatore
        } else if (negoziochoose == '3' && monete >= 500) {
            monete -= 500; // fase di test
        } else if (negoziochoose == 27) { // codice ascii di ESC
            break;
        }
    }
}

void reset() {
    punteggio = 0;
    xGiocatore = LARGHEZZA / 2;
    yGiocatore = ALTEZZA - 1;
}

void gioca() {
    bool fineGioco = false;
    char tasto;
    mvprintw(yGiocatore, xGiocatore, "A");
    spawnNemici();

    // Abilita la modalità non bloccante per la tastiera
    nodelay(stdscr, TRUE);
    // Disabilita l'echo nel terminale dei caratteri inseriti
    noecho();

    while (!fineGioco) {
        tasto = getch();
        if (tasto == 'a' && xGiocatore > 1) {
            mvprintw(yGiocatore, xGiocatore, " ");
            xGiocatore--;
            mvprintw(yGiocatore, xGiocatore, "A");
        } else if (tasto == 'd' && xGiocatore < LARGHEZZA - 1) {
            mvprintw(yGiocatore, xGiocatore, " ");
            xGiocatore++;
            mvprintw(yGiocatore, xGiocatore, "A");
        } else if (tasto == 'w') {
            sparaProiettile(xGiocatore, yGiocatore);
        }

        if (tasto == 'c'){
            reset();
            break;
        }
    }
}

void menu() {
    char scelta;

    while (true) {
        clear();
        printw("Benvenuto nel menu:\n");
        printw("1. Avvia la partita\n");
        printw("2. Apri il negozio\n");
        printw("3. Esci\n");

        refresh();

        scelta = getch();

        switch (scelta) {
            case '1':
                inizializza(); // starta
                gioca();
                break;
            case '2':
                apriNegozio(); // apre il negozio
                break;
            case '3':
                endwin(); // Chiudi la finestra curses prima di uscire
                exit(0);
                break;
            default:
                break;
        }
    }
}

int main() {
    initscr(); // Inizializza curses una sola volta
    while (true) {
        menu();
    }

    endwin(); // chiude la finestra di curses 
    return 0;
}

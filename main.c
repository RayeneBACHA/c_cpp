#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CMAX 10

int wahl, x;

typedef struct Date {
    int jour;
    int mois;
    int annee;
} Date;

typedef struct SClient {
    int Code_cli;
    char Nom[CMAX];
    char Prenom[CMAX];
} SClient;

typedef struct SCompte {
    int Code_cpt;
    int Code_cli;
    int Saldo;
    struct Date d_compte;
} SCompte;

SClient clients[20];
SCompte comptes[20];

void kund_verwalten(int x);
void konto_verwalten(int x);
void operationen_verwalten(int x);
void sous_menu(int x);
void menu();

int ReadClients() {
    int nbClients = 0;
    FILE *file;
    struct SClient client;
    file = fopen("Kunden.txt", "rb");
    if (file == NULL) {
        return 0;
    }
    while (fread(&client, sizeof(client), 1, file)) {
        clients[nbClients] = client;
        nbClients++;
    }
    fclose(file);
    return nbClients;
}

int ReadAccounts() {
    int nbComptes = 0;
    FILE *file;
    SCompte compte;
    file = fopen("kontos.txt", "rb");
    if (file == NULL) {
        return 0;
    }
    while (fread(&compte, sizeof(SCompte), 1, file)) {
        comptes[nbComptes] = compte;
        nbComptes++;
    }
    fclose(file);
    return nbComptes;
}

int compare(const void *a, const void *b) {
    const SClient *c1 = (const SClient*)a;
    const SClient *c2 = (const SClient*)b;
    return strcmp(c1->Nom, c2->Nom);
}

int kunden_anzeigen() {
    int nbClients = ReadClients();
    if (nbClients == 0) {
        printf("Keine Kunden vorhanden!\n");
        return 0;
    }
    qsort(clients, nbClients, sizeof(SClient), compare);
    for (int i = 0; i < nbClients; i++) {
        printf("\nCode Client : %d", clients[i].Code_cli);
        printf("\nNom : %s %s", clients[i].Nom, clients[i].Prenom);
        printf("\n");
    }
    printf("\n");
    return nbClients;
}

int kontos_anzeigen() {
    int nbComptes = ReadAccounts();
    if (nbComptes == 0) {
        printf("Keine Konten vorhanden!\n");
        return 0;
    }
    for (int i = 0; i < nbComptes; i++) {
        printf("Kund_ID : %d | Konto_nummer : %d erstellt am %d/%d/%d", 
               comptes[i].Code_cli, comptes[i].Code_cpt, 
               comptes[i].d_compte.jour, comptes[i].d_compte.mois, comptes[i].d_compte.annee);
        printf(":\nSaldo : %d\n", comptes[i].Saldo);
        printf("\n");
    }
    printf("\n");
    return nbComptes;
}

void kund_verwalten(int x) {
    FILE *file;
    if (x == 1) { 
        SClient client;
        printf("Code client : ");
        scanf("%d", &client.Code_cli);
        
        while (getchar() != '\n');

        int nbClients = ReadClients();
        for (int i = 0; i < nbClients; i++) {
            if (clients[i].Code_cli == client.Code_cli) {
                printf("Client code existiert bereits!\n\n");
                return;
            }
        }

        printf("Nom : ");
        scanf("%9s", client.Nom); 
        
        printf("Prenom : ");
        scanf("%9s", client.Prenom); 

        file = fopen("Kunden.txt", "ab");
        if (file == NULL) {
            printf("Fehler beim Öffnen der Datei!\n");
            return;
        }

        size_t written = fwrite(&client, sizeof(client), 1, file);
        fclose(file);
        
        if (written == 1) {
            printf("Kund erfolgreich hinzugefügt.\n\n");
        } else {
            printf("Fehler beim Schreiben!\n");
        }

    } else if (x == 2) { 
        int nbClients = kunden_anzeigen();
        if (nbClients == 0) return;

        int code;
        printf("Code des zu löschenden Clients : ");
        scanf("%d", &code);

        int clientExists = 0;
        for (int i = 0; i < nbClients; i++) {
            if (clients[i].Code_cli == code) {
                clientExists = 1;
                break;
            }
        }

        if (!clientExists) {
            printf("Client mit Code %d existiert nicht!\n\n", code);
            return;
        }

        remove("Kunden.txt");
        FILE *nvfile = fopen("Kunden.txt", "ab");
        if (nvfile == NULL) {
            printf("Fehler beim Öffnen der Datei!\n");
            return;
        }

        int deleted = 0;
        for (int i = 0; i < nbClients; i++) {
            if (clients[i].Code_cli != code) {
                fwrite(&clients[i], sizeof(clients[i]), 1, nvfile);
            } else {
                deleted = 1;
            }
        }
        fclose(nvfile);

        printf(deleted ? "Client gelöscht.\n\n" : "Löschen fehlgeschlagen!\n\n");

    } else if (x == 3) {
        kunden_anzeigen();

    } else if (x == 4) { 
        int nbClient = kunden_anzeigen();
        if (nbClient == 0) return;

        int code;
        printf("Code des zu ändernden Clients : ");
        scanf("%d", &code);

        int clientIndex = -1;
        for (int i = 0; i < nbClient; i++) {
            if (clients[i].Code_cli == code) {
                clientIndex = i;
                break;
            }
        }

        if (clientIndex == -1) {
            printf("Client existiert nicht!\n\n");
            return;
        }

        char nvNom[CMAX], nvPrenom[CMAX];
        printf("Neuer Nachname : ");
        scanf("%9s", nvNom);
        printf("Neuer Vorname : ");
        scanf("%9s", nvPrenom);

        remove("Kunden.txt");
        FILE *nv = fopen("Kunden.txt", "wb");
        if (nv == NULL) {
            printf("Dateifehler!\n");
            return;
        }

        for (int i = 0; i < nbClient; i++) {
            if (i == clientIndex) {
                strncpy(clients[i].Nom, nvNom, CMAX-1);
                strncpy(clients[i].Prenom, nvPrenom, CMAX-1);
                clients[i].Nom[CMAX-1] = '\0';  // Ensure null-termination
                clients[i].Prenom[CMAX-1] = '\0';
            }
            fwrite(&clients[i], sizeof(clients[i]), 1, nv);
        }
        fclose(nv);
        printf("Daten aktualisiert.\n\n");
    }
}

void konto_verwalten(int x) {
    FILE *file;
    SCompte compte, compte0;
    int found = 0;

    if (x == 1) {
        printf("Geben Sie die Kontonummer ein : ");
        scanf("%d", &compte.Code_cpt);

        file = fopen("kontos.txt", "rb");
        if (file != NULL) {
            while (fread(&compte0, sizeof(SCompte), 1, file)) {
                if (compte0.Code_cpt == compte.Code_cpt) {
                    found = 1;
                    break;
                }
            }
            fclose(file);
        }

        if (found == 1) {
            printf("Konto existiert bereits!\n\n");
            return;
        }

        printf("Geben Sie die Kund_ID ein : ");
        scanf("%d", &compte.Code_cli);
        int nbClients = ReadClients();
        int validClient = 0;
        for (int i = 0; i < nbClients; i++) {
            if (clients[i].Code_cli == compte.Code_cli) {
                validClient = 1;
                break;
            }
        }

        if (!validClient) {
            printf("Es gibt keinen Kund mit diesem Code!\n\n");
            return;
        }

        int saldo;
        printf("Geben Sie den anfänglichen Kontosaldo ein (mindestens 250$) : ");
        scanf("%d", &saldo);
        if (saldo < 250) {
            printf("Saldo zu niedrig!\n\n");
            return;
        }
        compte.Saldo = saldo;

        int day, month, year;
        do {
            printf("Geben Sie das Eröffnungsdatum (DD MM YYYY): ");
            scanf("%d %d %d", &day, &month, &year);
        } while (!(day >= 1 && day <= 30 && month >= 1 && month <= 12 && year >= 1950 && year <= 2025));
        compte.d_compte.jour = day;
        compte.d_compte.mois = month;
        compte.d_compte.annee = year;

        file = fopen("kontos.txt", "ab");
        if (file == NULL) {
            printf("Fehler beim Öffnen der Datei!\n");
            return;
        }
        fwrite(&compte, sizeof(SCompte), 1, file);
        fclose(file);
        printf("Konto wurde erfolgreich hinzugefügt.\n\n");
    } else if (x == 2) {
        printf("Geben Sie die Kontonummer ein : ");
        scanf("%d", &compte.Code_cpt);
        file = fopen("kontos.txt", "rb");
        if (file == NULL) {
            printf("Kontoliste leer!\nÖffnen Sie ein Konto bitte!\n");
            return;
        }
        
        while (fread(&compte0, sizeof(SCompte), 1, file)) {
            if (compte0.Code_cpt == compte.Code_cpt) {
                found = 1;
                compte = compte0;
                break;
            }
        }
        fclose(file);
        
        if (found == 0) {
            printf("Konto nicht gefunden!\n");
            return;
        }
        
        int nbClients = ReadClients();
        for (int i = 0; i < nbClients; i++) {
            if (clients[i].Code_cli == compte.Code_cli) {
                printf("\nNAME : %s %s\n", clients[i].Nom, clients[i].Prenom);
                printf("Kund_ID : %d | Konto_nummer : %d erstellt am %d/%d/%d", 
                       compte.Code_cli, compte.Code_cpt, 
                       compte.d_compte.jour, compte.d_compte.mois, compte.d_compte.annee);
                printf(":\nSaldo : %d\n", compte.Saldo);
                break;
            }
        }
    } else if (x == 3) {
        kontos_anzeigen();
    }else if (x == 4) { 
        int nbcomptes = kontos_anzeigen();
        if (nbcomptes == 0) return;
        
        int code;
        printf("Geben Sie den Code des zu schließenden Konto ein : ");
        scanf("%d", &code);
        
        int accountExists = 0;
        for (int i = 0; i < nbcomptes; i++) {
            if (comptes[i].Code_cpt == code) {
                accountExists = 1;
                break;
            }
        }
        
        if (!accountExists) {
            printf("Konto mit Code %d existiert nicht!\n\n", code);
            return;
        }

        remove("kontos.txt");
        FILE *nvfile = fopen("kontos.txt", "ab");
        if (nvfile == NULL) {
            printf("Fehler beim Öffnen der Datei!\n");
            return;
        }
        
        int deleted = 0;
        for (int i = 0; i < nbcomptes; i++) {
            if (comptes[i].Code_cpt != code) {
                fwrite(&comptes[i], sizeof(SCompte), 1, nvfile);
            } else {
                deleted = 1;
            }
        }
        fclose(nvfile);
        
        if (deleted) {
            printf("Konto wurde erfolgreich gelöscht.\n\n");
        } else {
            printf("Fehler beim Löschen des Kontos!\n\n");
        }
    }
}

void operationen_verwalten(int x) {
    FILE *file;
    int code;
    int nbComptes = ReadAccounts();
    
    if (nbComptes == 0) {
        printf("Kontoliste leer!\n");
        return;
    }
    
    kontos_anzeigen();
    printf("Kontonummer : ");
    scanf("%d", &code);
    
    int accountIndex = -1;
    for (int i = 0; i < nbComptes; i++) {
        if (comptes[i].Code_cpt == code) {
            accountIndex = i;
            break;
        }
    }
    
    if (accountIndex == -1) {
        printf("Konto nicht gefunden!\n");
        return;
    }
    
    if (x == 1) {
        int betrag;
        printf("wie viel Geld würden Sie auszahlen (maximal 500$): ");
        scanf("%d", &betrag);
        if (betrag <= 500 && betrag > 0 && betrag <= comptes[accountIndex].Saldo) {
            remove("kontos.txt");
            file = fopen("kontos.txt", "ab");
            if (file == NULL) {
                printf("Fehler beim Öffnen der Datei!\n");
                return;
            }
            comptes[accountIndex].Saldo -= betrag;
            printf("%d wurde abgebucht.\nNeue Kontosaldo : %d\n", betrag, comptes[accountIndex].Saldo);
            for (int i = 0; i < nbComptes; i++) {
                fwrite(&comptes[i], sizeof(SCompte), 1, file);
            }
            fclose(file);
        } else {
            printf("Ungültiger Betrag!\n");
        }
    } else if (x == 2) {
        int betrag;
        printf("wie viel Geld würden Sie einzahlen : ");
        scanf("%d", &betrag);
        if (betrag > 0) {
            remove("kontos.txt");
            file = fopen("kontos.txt", "ab");
            if (file == NULL) {
                printf("Fehler beim Öffnen der Datei!\n");
                return;
            }
            comptes[accountIndex].Saldo += betrag;
            printf("%d wurde eingezahlt!\nNeue Kontosaldo : %d\n", betrag, comptes[accountIndex].Saldo);
            for (int i = 0; i < nbComptes; i++) {
                fwrite(&comptes[i], sizeof(SCompte), 1, file);
            }
            fclose(file);
        } else {
            printf("Ungültiger Betrag!\n");
        }
    } else if (x == 3) {
        return;
    } else {
        printf("Schlechte Wahl!\n");
    }
}

void menu() {
    while (1) {
        printf("\n1- Kundenverwaltung\n");
        printf("2- Kontoverwaltung\n");
        printf("3- Operationenverwaltung\n");
        printf("4- Programm beenden\n\n");
        printf("Wählen Sie das Menü : ");
        scanf("%d", &wahl);
        if (wahl == 4) break;
        if (wahl < 1 || wahl > 4) {
            printf("Ungültige Wahl!\n");
            continue;
        }
        sous_menu(wahl);
    }
}

void sous_menu(int x) {
    int choice;
    if (x == 1) {
        printf("\n1- Hinzufügen\n");
        printf("2- Löschen\n");
        printf("3- Anzeigen\n");
        printf("4- Verändern\n");
        printf("5- Zurück\n\n");
        printf("Wählen Sie das Untermenü : ");
        scanf("%d", &choice);
        if (choice == 5) return;
        if (choice < 1 || choice > 5) {
            printf("Ungültige Wahl!\n");
            return;
        }
        kund_verwalten(choice);
    } else if (x == 2) {
        printf("\n1- Hinzufügen\n");
        printf("2- Suchen\n");
        printf("3- Liste anzeigen\n");
        printf("4- Löschen\n");
        printf("5- Zurück\n\n");
        printf("Wählen Sie das Untermenü : ");
        scanf("%d", &choice);
        if (choice == 5) return;
        if (choice < 1 || choice > 5) {
            printf("Ungültige Wahl!\n");
            return;
        }
        konto_verwalten(choice);
    } else if (x == 3) {
        printf("\n1- Auszahlung\n");
        printf("2- Einzahlung\n");
        printf("3- Zurück\n\n");
        printf("Wählen Sie das Untermenü : ");
        scanf("%d", &choice);
        if (choice == 3) return;
        if (choice < 1 || choice > 3) {
            printf("Ungültige Wahl!\n");
            return;
        }
        operationen_verwalten(choice);
    } else {
        printf("Ungültige Wahl!\n");
    }
}

int main() {
    menu();
    return 0;
}
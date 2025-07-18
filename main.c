#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CMAX 10


int wahl, x;

typedef struct Date {
   int jour;
    int mois;
    int annee;
}Date;

typedef struct SClient {
    int Code_cli;
    char Nom[CMAX];
    char Prenom[CMAX];
}SClient;

typedef struct SCompte {
    int Code_cpt;
    int Code_cli;
    struct Date d_compte;
}SCompte;



SClient clients[20];
SCompte comptes[20];










int ReadData() {
    int nbClients = 0;
    FILE *file;
    struct SClient client;
    file = fopen("Kunden.txt", "rb");
    if (file == NULL) {
        perror("Fehler beim Öffnen der Datei!");
        return 0;
    }
    while (fread(&client, sizeof(client), 1, file))
    {
        clients[nbClients] = client;
        nbClients++;
    }
    fclose(file);
    return nbClients;
}


void kunden_anzeigen(){
    int nbClients = ReadData();
    for (int i = 0; i < nbClients; i++)
    {
        printf("\nCode Client : %d", clients[i].Code_cli);
        printf("\nNom : %s %s", clients[i].Nom, clients[i].Prenom);
        printf("\n");
    }
    printf("\n");
}
    
void kund_verwalten(int x) {
    struct SClient client;
    FILE *file;
    if (x == 1)
    {
        printf("code client : ");
        scanf("%d", &client.Code_cli);
        printf("Nom : ");
        scanf("%s", &client.Nom);
        printf("Prenom : ");
        scanf("%s", &client.Prenom);
        file = fopen("Kunden.txt", "ab");
        size_t written = fwrite(&client, sizeof(client), 1, file);
        fclose(file);
        if (written == 1)
        {
            printf("Kund wurde erfolgreich hinzugefügt.\n\n");
        }else
        {
            printf("Fehler beim schreieben in die Datei!\n");
        }    
    }else if (x == 2)
    {
        kunden_anzeigen();
        int nbClients = ReadData();
        remove("Kunden.txt");
        int code;
        printf("Geben Sie den Code des zu löschenden Clients ein : ");
        scanf("%d", &code);
        FILE *nvfile;
        nvfile = fopen("Kunden.txt", "ab");
        for (int i = 0; i < nbClients; i++)
        {
            if (clients[i].Code_cli != code)
            {
                fwrite(&clients[i], sizeof(clients[i]), 1, nvfile);
            }
        }
        fclose(nvfile);
        printf("Kund wurde erfolgreich gelöscht.\n");
        printf("\n");
    }
    
    else if (x == 3)
    {
        kunden_anzeigen();
    }else if (x == 4)
    {
        char nvNom[10];
        char nvPrenom[10];
        int code;
        kunden_anzeigen();
        int nbClient = ReadData();
        remove("Kunden.txt");
        FILE *nv;
        nv = fopen("Kunden.txt", "wb");
        printf("Geben Sie den Code des zu ändern Clients ein : ");
        scanf("%d", &code);
        printf("neue Familienname : ");
        scanf("%s", &nvNom);
        printf("neue Name : ");
        scanf("%s", &nvPrenom);
        int i = 0;
        while ((i != nbClient))
        {
            if (code == clients[i].Code_cli)
            {
                strcpy(clients[i].Nom, nvNom);
                strcpy(clients[i].Prenom, nvPrenom);
                break;
            }
            i++;
        }
        for (int i = 0; i < nbClient; i++)
        {
            size_t written = fwrite(&clients[i], sizeof(clients[i]), 1, nv);
            if (written != 1)
            {
                perror("Error!");
            }
        }
        fclose(nv);
    }
}


void menu() {
    while (1){
        printf("1- Kundenverwaltung\n");
        printf("2- Kontoverwaltung\n");
        printf("3- Operationenverwaltung\n");
        printf("4- Programm beenden\n");
        printf("\n");
        printf("Wählen Sie das Menü : ");
        scanf("%d", &wahl);
        if (wahl == 4) break;
        sous_menu(wahl);
    }
    
}

void sous_menu(int wahl){
    if (wahl == 1)
    {
        printf("1- Hinzufügen\n");
        printf("2- Löschen\n");
        printf("3- Anzeigen\n");
        printf("4- Verändern\n");
        printf("5- Zurück\n");
        printf("\n");
        printf("Wählen Sie das Unter_menü : ");
        scanf("%d", &x);
        if (x == 5) return;
           kund_verwalten(x);
            
    
        }
        else if (wahl == 2)
        {
            printf("1- Hinzufügen\n");
            printf("2- Suchen\n");
            printf("3- Liste anzeigen\n");
            printf("4- Löschen\n");
            printf("5- Zurück\n");
            printf("\n");
            printf("Wählen Sie das Unter_menü : ");
            scanf("%d", &x);
            if (x == 5) return;
            // konto_verwalten(x);
        }
        else if (wahl == 3)
        {
            printf("1- Auszahlung\n");
            printf("2- Einzahlung\n");
            printf("3- Zurück\n");
            printf("Wählen Sie ein Untermenü:");
            scanf("%d", &x);
            if (x == 3) return;
            // operation_verwalten(x);
        }else{
            printf("Schlechte Wahl");
        }
}       



        
// void konto_verwalten(int x);
int main()
{
    menu();
    return 0;
}

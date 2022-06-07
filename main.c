#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Hauteur_Cb 200
#define Large_Cb 8

char Code_39_ASCII[1000]={'\0'};
int  Barre_nb_element=0;


/* Code39 */
/* https://fr.wikipedia.org/wiki/Code_39 */

/* A a Z */
int Lettre_C39[26]={0x109,0x49,0x148,0x19,0x118,0x58,0xD,0x10C,0x4C,0x1C,0x103,0x43,0x142,0x13,0x112,0x52,0x7,0x106,0x46,0x16,0x181,0xC1,0x1C0,0x91,0x190,0xD0};
/* 0 à 9*/
int Chiffre_C39[10]={0x34,0x121,0x61,0x160,0x31,0x130,0x70,0x25,0x124,0x64};
/* espace 0, - 1 , $ 2 , % 3 , . 4 , / 5 , + 6 , * 7  */
int Symbole_C39[8]={0xC4,0x85,0xA8,0x2A,0x184,0xA2,0x8A,0x94};

int StateBit(int valeur,int bit)
{
    if ((valeur & (1<<bit))!=0) return 1;
    else return 0;
}

int Valide_C39(char *chaine)
{
    int compteur;
    int valide=0; /*faux au depart */

    for (compteur=0; compteur < (int) strlen(chaine);compteur++)
    {
        if ((chaine[compteur]>='A') && (chaine[compteur]<='Z')) valide=1;
        if ((chaine[compteur]>='0') && (chaine[compteur]<'9')) valide=1;
        if (chaine[compteur]==' ') valide=1;
        if (chaine[compteur]=='-') valide=1;
        if (chaine[compteur]=='$') valide=1;
        if (chaine[compteur]=='%') valide=1;
        if (chaine[compteur]=='.') valide=1;
        if (chaine[compteur]=='/') valide=1;
        if (chaine[compteur]=='+') valide=1;
        if (chaine[compteur]=='*') valide=1;
    }
    return valide;
}

void Test_C39(char *chaine)
{
    if (!Valide_C39(chaine))
    {
        printf("La chaine n'est pas conforme à C39\n");
        exit(EXIT_FAILURE);
    }
    return;
}


int Ascii2C39(char car)
{
    int CodeC39=Symbole_C39[4]; /* le point */

    if ((car>='A') && (car<='Z'))
    {
        CodeC39=Lettre_C39[car-'A'];
        return CodeC39;
    }

    if ((car>='0') && (car<='9'))
    {
        CodeC39=Chiffre_C39[car-'0'];
        return CodeC39;
    }
    /* espace 0, - 1 , $ 2 , % 3 , . 4 , / 5 , + 6 , * 7  */
    if (car==' ')
    {
        CodeC39=Symbole_C39[0];
        return CodeC39;
    }

    if (car=='-')
    {
        CodeC39=Symbole_C39[1];
        return CodeC39;
    }


    if (car=='$')
    {
        CodeC39=Symbole_C39[2];
        return CodeC39;
    }

    if (car=='%')
    {
        CodeC39=Symbole_C39[3];
        return CodeC39;
    }

    if (car=='.')
    {
        CodeC39=Symbole_C39[4];
        return CodeC39;
    }
    if (car=='/')
    {
        CodeC39=Symbole_C39[5];
        return CodeC39;
    }

    if (car=='+')
    {
        CodeC39=Symbole_C39[3];
        return CodeC39;
    }

    if (car=='*')
    {
        CodeC39=Symbole_C39[3];
        return CodeC39;
    }

    return CodeC39;
}

void Barre_1_Noire(void)
{
    //printf("111");
    Code_39_ASCII[Barre_nb_element++]='0';
    Code_39_ASCII[Barre_nb_element++]='0';
    Code_39_ASCII[Barre_nb_element++]='0';
}

void Barre_0_Noire(void)
{
    //printf("1");
    Code_39_ASCII[Barre_nb_element++]='0';
}

void Barre_1_blanche(void)
{
    //printf("000");
    Code_39_ASCII[Barre_nb_element++]='1';
    Code_39_ASCII[Barre_nb_element++]='1';
    Code_39_ASCII[Barre_nb_element++]='1';
}

void Barre_0_blanche(void)
{
    //printf("0");
    Code_39_ASCII[Barre_nb_element++]='1';
}

/* 1 grosse barre (3) 0 petite barre (1) */
void BarreC39(int CodeC39)
{
    int compteur;
    int bw=1;
    for (compteur=8; compteur>=0;compteur--)
    {
        if (bw)
        {   /* barre noire */
            if (StateBit(CodeC39,compteur))  Barre_1_Noire();
            else Barre_0_Noire();
        }
        else
        {    /* barre blanche */
            if (StateBit(CodeC39,compteur))  Barre_1_blanche();
            else Barre_0_blanche();
        }
    bw^=1;
    }

    return;
}

void fichier(void)
{
    int Lg_chaine;
    int cpt;
    int hauteur=0;
    int cpt_larg=0;
    FILE * fichier;

    //Lg_chaine=(int)strlen(Code_39_ASCII);
    Lg_chaine=Barre_nb_element;
    printf("Fichier !\n");
    fichier =fopen ("CodeBarre39.pbm","w+");
    if (fichier == NULL)
    {
        exit(EXIT_FAILURE);
    }
    fprintf(fichier,"P1\n");
    fprintf(fichier,"%d %d\n",Lg_chaine*Large_Cb,Hauteur_Cb);
    for (hauteur=0;hauteur<Hauteur_Cb;hauteur++)
    for (cpt=0;cpt < Lg_chaine; cpt++)
    {
        for (cpt_larg=0; cpt_larg < Large_Cb;cpt_larg++)
        fprintf(fichier, "%c\n",Code_39_ASCII[cpt]);
    }
fclose(fichier);


}

void Affiche_C39(char *chaine)
{
    int CodeC39;
    int long_chaine;
    int compteur;

    long_chaine=strlen(chaine);

    Test_C39(chaine);
    /*  code de depart * */
    BarreC39(Ascii2C39('*'));
    for (compteur=0; compteur<long_chaine;compteur++)
    {
    CodeC39=Ascii2C39(chaine[compteur]);
    BarreC39(CodeC39);
    }
    /*  code de fin * */
    BarreC39(Ascii2C39('*'));
}


int main(int argc,char ** argv)
{
    printf("Code 39 V1\n");
    if (argc!=2)
    {
        printf("Veuillez me donner la chaine à placer dans le code C39\n");
        return EXIT_FAILURE;
    }



    printf("chaine a convertir %s\n",argv[1]);
//    Affiche_C39(argv[1]);
    Affiche_C39('A');
    Code_39_ASCII[Barre_nb_element]='\0'; /* fin de chaine */

    printf("\n%s\n",Code_39_ASCII);
    fichier();
    return EXIT_SUCCESS;
}

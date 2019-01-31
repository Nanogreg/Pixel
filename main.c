/*---
    CREATED BY GREGORY CUYLE IN 2010-2011
    cuyleg@protonmail.com
---*/

#include <stdio.h>
#include <windows.h>
#include <myconio.h>
#include <ctype.h>
#include <time.h>

#define CLRSCR textbackground(BLACK);MYclrwin(1,1,80,25)
#define BIP printf("\a");
#define FLECHE "%c",16
#define POINT  "%c",4


/*---Const---*/
#define HAUT 72
#define BAS 80
#define GAUCHE 75
#define DROITE 77

/*---Structure Score---*/
typedef struct
{
    char nom[15];
    short score;
    char difficulte[15];
}Score;

/*---Prototypes---*/
void acceuil (void);
short menu (short*);
short saisie_couleur(short*son);
short difficulte (short*son);
void instructions (void);
void affiche_lvl (short);
void depart (short);
void affiche_obstacles(short,short obstacle[81][26],short son);
void check_lvl( short score, short *lvl, short *vitesse,short*x,short*y,char*touche,short*aleat,short obstacle[81][26],short son);
void fin(short,short,short,short,char);
void clr_obstacle(short obstacle[81][26]);
void affiche_score (void);
void affiche_top3(void);
void calcul_aleat(int*,int*,short score,short obstacle[81][26]);
void affiche_cur_score (short score, short couleur);
void displayDG(char *chaine, short debut, short fin, short timing, short color);

/*---The Main Code---*/
int main(void)
{
    char touche,tn,toucheprec;
    unsigned int randx,randy,rand2x,rand2y;
    short obstacle[81][26]={};
    short enfer,vitesse_max,son,lvl_prec,triche,aleat,x,y,continuer,couleur,i,j,score,niveau,lvl,vitesse,partie;

    CLRSCR;
    /*---Hide cursor---*/
    CONSOLE_CURSOR_INFO ci = {1, FALSE};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);

    /*---Welcome screen---*/
    acceuil();

    do
    {
    /*---Initit variables---*/
    x=35;y=12;
    score=vitesse=0;
    rand2x=rand2y=0;
    vitesse_max=85;
    touche=DROITE;
    lvl_prec=lvl=1;
    aleat=FALSE;
    triche=FALSE;
    continuer=TRUE;
    son=TRUE;

    partie=menu(&son);

    /*---IF Custom Game---*/
    if(partie==2)
    {
        /*---Color choice---*/
        couleur=saisie_couleur(&son);

        /*---Difficulty---*/
        niveau=difficulte(&son);

        /*---Instructions---*/
        instructions();
        gotoxy(4,18);printf("Make the best score, Good luck :D");
        gotoxy(16,20);printf("<Hit a Key to stat the Game>");getch();
    }

    /*---Fast Game---*/
    else {couleur=WHITE;niveau=1;}

    /*---IF lvl 6---*/
    if(partie==6)
    {
        score=75;triche=TRUE;
        randx=randy=6;
        check_lvl(score, &lvl, &vitesse, &x, &y, &touche, &aleat,obstacle,son);
    }
    else
    {
        /*---Show LVL---*/
        affiche_lvl(lvl);
        /*---Start---*/
        depart(son);
        /*---show obstacles---*/
        clr_obstacle(obstacle);
        affiche_obstacles(lvl,obstacle,son);
        /*---show score---*/
        affiche_cur_score (score,WHITE);
    }

    /*---find first point position---*/
    calcul_aleat(&randx, &randy, score, obstacle );

    /*-----------------------------------PRINCIPAL : The Game-----------------------------------*/
    while(continuer==TRUE)
    {
        /*-------------------If you touch a point-------------------*/
        if( randx==x && randy==y )
        {
            score++;
            /*---show new score---*/
            affiche_cur_score (score,WHITE);


            //  Sound when point is catched
            //  If 2 points hits, only sound 2
            if(son==TRUE && randx!=rand2x && randy!=rand2y ) BIP

            /*  If we change lvl , whitout this fonction, sound is not done,
                force sound play if score = 15 */
            if( son==TRUE && randx==rand2x && randy==rand2y && (score%15)==0 ) BIP


            if(vitesse<=vitesse_max){vitesse+=6;} // increse speed

            lvl_prec=lvl;
            /*---Check if lvl need to be upped---*/
            check_lvl(score, &lvl, &vitesse, &x, &y, &touche, &aleat,obstacle, son);

            /*---Random point is calculated---*/
            calcul_aleat(&randx, &randy, score, obstacle);
        }

        /*-------------------------Second point calc -----------------*/
        if(aleat==FALSE && score==(rand()%200)+1)   //nobody (I think) will do more than 200
        {
            aleat=TRUE;

            calcul_aleat(&rand2x, &rand2y, score, obstacle);

            /*---Show rdm pt NO 2---*/
            textbackground(BLACK);
            if(rand2x==randx && rand2y==randy) textcolor(YELLOW);
            else textcolor(MAGENTA);
            gotoxy(rand2x,rand2y);printf(POINT);
        }
        /*---If we hit the pt N2, make it disapear---*/
        if(aleat==TRUE)
        {
            if (rand2x==x && rand2y==y)
            {if(son==TRUE) BIP
            aleat=FALSE;
            rand2x=rand2y=0;

            score++;
            /*---Show new score---*/
            affiche_cur_score (score,WHITE);

            lvl_prec=lvl;
            /*---Check if lvl need to be upped---*/
            check_lvl(score, &lvl, &vitesse, &x, &y, &touche, &aleat,obstacle,son);

            if(lvl_prec!=lvl)calcul_aleat(&randx, &randy, score, obstacle);
            }
            else
            {
                /*---Affiche point al�atoire NO 2---*/
                textbackground(BLACK);
                if(rand2x==randx && rand2y==randy) textcolor(YELLOW);
                else textcolor(MAGENTA);
                gotoxy(rand2x,rand2y);printf(POINT);
            }
        }

        /*---Display pt NO 1---*/
        textbackground(BLACK);
        if(rand2x==randx && rand2y==randy) textcolor(YELLOW);
        else textcolor(LIGHTRED);
        gotoxy(randx,randy);printf(POINT);

        /*-----------------------------Functions IF you hit a key-----------------------------*/
        if(kbhit())
        {
            tn=(tolower(getch()));
            if(kbhit()) touche=getch();

            /*---IF Pause---*/
            if( tn=='p' )
            {
                textbackground(BLACK);
                gotoxy(47,1);textcolor(WHITE);printf("Resume Game : <p>");
                gotoxy(15,1);textcolor(YELLOW);printf("%c%cPAUSE%c%c",4,4,4,4);

                do              //Necessaire car 'p'=code ASCII 80 et fleche du bas du clavier = code ASCII �tendu 80.
                {
                    tn=(tolower(getch()));
                    if(kbhit()) tn=getch();
                }while( tn!='p' );

                /* Display normal(unpause) screen */
                textbackground(BLACK);MYclrwin(47,1,70,1);
                gotoxy(55,1);textcolor(WHITE);printf("Pause : <p>");
                textbackground(BLACK);MYclrwin(15,1,34,1);
                gotoxy(19,1);printf("Level : %hd",lvl);
            }
            /*--- If you want to go back to Menu ---*/
            else if( lvl==1 && tn=='m' )
            {
                /*--- Confirm Box ---*/
                textcolor(WHITE);
                /*---Sides---*/
                gotoxy(28,8);printf("%c",201);
                gotoxy(53,12);printf("%c",188);
                gotoxy(53,8);printf("%c",187);
                gotoxy(28,12);printf("%c",200);

                /*---sticks---*/
                for(i=0;i<24;i++)
                {
                    gotoxy(29+i,8);printf("%c",205);
                    gotoxy(29+i,12);printf("%c",205);
                }
                gotoxy(28,9);printf("%c",186);
                gotoxy(28,10);printf("%c",186);
                gotoxy(28,11);printf("%c",186);
                gotoxy(53,9);printf("%c",186);
                gotoxy(53,10);printf("%c",186);
                gotoxy(53,11);printf("%c",186);
                fflush(stdin);
                gotoxy(30,10);printf("Back to Menu ? <y/n>");
                do
                {
                    while(kbhit())getch();
                    tn= tolower(getch());
                }while( tn!='y' && tn!='n');

                if( tn=='y' ) {continuer=FALSE;tn='m';} //Back at lvl 1.
                else { textbackground(BLACK);MYclrwin(28,8,53,12);}
            }
            /*--- Sound on/off with green or red color---*/
            else if( tn=='s')
            {
                if(son==TRUE){ son=FALSE; textcolor(LIGHTRED); }
                else { son=TRUE; textcolor(LIGHTGREEN); }
                gotoxy(72,1);printf("Sound");
            }

            /*---Keep same direction if unknow key pressed---*/
            if(touche!=HAUT && touche!=BAS && touche!=DROITE && touche!=GAUCHE)
                touche=toucheprec;
        }

        textbackground(BLACK);MYclrwin(x,y,x,y);    //Erase old position

        /*-------------------Direction change if arrow key pression-------------------*/
        switch(touche)
        {
            case HAUT :  toucheprec=HAUT; y--;break;
            case BAS :  toucheprec=BAS; y++;break;
            case GAUCHE :  toucheprec=GAUCHE; x--;break;
            case DROITE :  toucheprec=DROITE; x++;break;
        }

        /*--- Handle "Wall teleportation"---*/
        if(x>80)x=1;
        if(x<1)x=80;
        if(y<2)y=25;
        if(y>25)y=2;

        /* You loose if you hit an obtacle */
        for(i=1;i<=80;i++)
            for(j=2;j<=25;j++) if(obstacle[x][y]==1) continuer=FALSE;

        /* User define Color for cursor, exept for lvl 6*/
        if(lvl==6)  textbackground(LIGHTRED);
        else        textbackground(couleur);

        MYclrwin(x,y,x,y); // Display new cursor position

        /* Display "Cursed" if random nb = 666 in lvl 6*/
        if(lvl==6)
        {
            srand(time(NULL));
            enfer= rand()%700 ;

            if(enfer==666)
            {
                textbackground(BLACK);
                textcolor(LIGHTRED);gotoxy(23,3);printf("You are Cursed ! Seep Incease ! .");
                vitesse+=12;
            }
        }

        /*---Seep setup ( with points and lvl ) ---*/
        switch(niveau)
        {
            case 3 :Sleep(100-vitesse);break;
            case 2 :Sleep(120-vitesse);break;
            case 1 :Sleep(140-vitesse);
        }
    }
    //END OF MAIN

    /*---Display end of game---*/
    textbackground(LIGHTRED);MYclrwin(x,y,x,y); //red point if obstacle hit
    fin(score,niveau,triche,son,tn);

    }while(1);
  return 0;
}
/*------------------------------------------------Functions------------------------------------------------*/
/*---Welcome---*/
void acceuil (void)
{
    short i,passer=FALSE;

    textcolor(WHITE);gotoxy(56,1);printf("<Enter> : Skip Intro");

    for(i=0;passer==FALSE && i<49;i++)
    {
        while(kbhit())getch();

        Sleep(50);
        textbackground(MAGENTA);MYclrwin(32+i,12,33+i,12);
        textbackground(BLACK);MYclrwin(32+i,12,32+i,12);
        switch(i)
        {
            case 5 : textcolor(13);gotoxy(32+i,12);printf("P");break;
            case 6 : textcolor(12);gotoxy(32+i,12);printf("I");break;
            case 7 : textcolor(14);gotoxy(32+i,12);printf("X");break;
            case 8 : textcolor(10);gotoxy(32+i,12);printf("E");break;
            case 9 : textcolor(11);gotoxy(32+i,12);printf("L");break;
            case 11 : textcolor(WHITE);gotoxy(32+i,12);printf("b");break;
            case 12 : gotoxy(32+i,12);printf("e");break;
            case 13 : gotoxy(32+i,12);printf("t");break;
            case 14 : gotoxy(32+i,12);printf("a");
        }
        if(kbhit()) if(getch()==13) passer=TRUE;
    }
    while(kbhit())getch();

    /* Pause  */
    if(kbhit()) if(getch()==13) passer=TRUE;

    if(passer==FALSE){textcolor(MAGENTA);gotoxy(37,14);printf("THE GAME !");Sleep(1000);}
}
                /*---MENU---*/
short menu (short *son)
{
    short touche,x,partie;

    do
    {
        x=touche=0;

        CLRSCR;
        gotoxy(33,5);textcolor(13);printf("---M");;
                     textcolor(12);printf("E");
                     textcolor(14);printf("N");
                     textcolor(11);printf("U---");
                     textcolor(WHITE);
        gotoxy(30,7);  printf("Quick Game");
        gotoxy(30,8);  printf("Custom Game");
        gotoxy(30,9);  printf("Scores");
        gotoxy(30,10); printf("Instructions");
        gotoxy(30,11); printf("Quit");

        /*---Help for---*/
        gotoxy(72,1);textcolor(LIGHTGREEN);printf("Sound");
        textcolor(WHITE);printf(" : <s>");

        /* top 3 scores below the menu */
        affiche_top3();

        /*---Menu selection with arrows---*/
        gotoxy(28,7+x);printf(FLECHE);

        /* Canter Enter hit before Menu display */
        Sleep(100);
        while(kbhit())getch();

        /*---------- Navigation in Menu ----------*/
        while(touche!=13)
        {
            textcolor(WHITE);
            touche=getch();

            /* Sound on/off */
            if(tolower(touche)=='s')
            {
                if(*son==TRUE){ *son=FALSE;gotoxy(72,1);textcolor(LIGHTRED);printf("Sound");}
                else { *son=TRUE;gotoxy(72,1);textcolor(LIGHTGREEN);printf("Sound");}
            }

            /* Arrow hit */
            textcolor(WHITE);
            if(touche!=13)
            {
                switch(touche)
                {
                    case HAUT : if(x>0){gotoxy(28,7+x);printf(" ");x--;}break;
                    case BAS :  if(x<5){gotoxy(28,7+x);printf(" ");x++;}break;
                }
                gotoxy(28,7+x);printf(FLECHE);
            }
            else partie=x+1;

            /* Direct lvl 6 */
            if(x==5)
            {
                textcolor(YELLOW);gotoxy(30,12);printf("Level 6 ! ");
                textcolor(WHITE);printf("<Warning : Impossible to save Score in this mode>");
            }
            else {textbackground(BLACK);MYclrwin(30,12,80,12);}
        }

        if(partie==5)       //we quit game
        {
            CLRSCR;
            gotoxy(15,7);printf("Pixel in an open source game.");

            gotoxy(15,13);printf("Developed by Gr%cgory in 2011 for personal learning",130);

            textcolor(13);printf("C");
            textcolor(12);printf("u");
            textcolor(14);printf("y");
            textcolor(11);printf("le");

            gotoxy(1,17);
            displayDG("See you soon for a Better Score !",2,79,2,YELLOW);
            getch();
            exit(0);
        }
        else if(partie==3)     //Score
            affiche_score();

        else if(partie==4)  //Insctructions
        {
            instructions();
            gotoxy(16,20);printf("<Hit any Key to go back to Menu>");getch();
        }

    }while(partie==3 || partie==4);

    return partie;
}
                /*---Color Choice---*/
short saisie_couleur(short*son)
{
    short i,couleur,x,touche;

    x=touche=0;

    CLRSCR;

    /* Display Sound : <s> */
    if(*son==TRUE)textcolor(LIGHTGREEN);
    else textcolor(LIGHTRED);
    gotoxy(72,1);printf("Sound");
    textcolor(WHITE);printf(" : <s>");

    do
    {
        for(i=10;i<16;i++){gotoxy(36,1+i);textcolor(i);printf("Color %hd",i-9);}
        gotoxy(18,8);printf("Choose your cursor Color");

        /*---Selection with arrow keys---*/
        gotoxy(34,11+x);printf(FLECHE);
        while(touche!=13)
        {
            touche=getch();

            /* Sound  */
            if(tolower(touche)=='s')
            {
                if(*son==TRUE){ *son=FALSE;gotoxy(72,1);textcolor(LIGHTRED);printf("Sound");}
                else { *son=TRUE;gotoxy(72,1);textcolor(LIGHTGREEN);printf("Sound");}
            }
            else
            {
                if(touche!=13)
                {
                    switch(touche)
                    {
                        case HAUT : if(x>0){gotoxy(34,11+x);printf(" ");x--;}break;
                        case BAS : if(x<5){gotoxy(34,11+x);printf(" ");x++;}break;
                    }
                    textcolor(WHITE);gotoxy(34,11+x);printf(FLECHE);
                }
                else couleur=x+10;
            }
        }
        CLRSCR;
    }while(couleur>15 || couleur<10);
    return couleur;
}
                /*---DIFFICULTY Select---*/
short difficulte (short*son)
{
    short niveau;
    short x,touche;

    x=touche=0;

    /* Sound : <s> */
    if(*son==TRUE)textcolor(LIGHTGREEN);
    else textcolor(LIGHTRED);
    gotoxy(72,1);printf("Sound");
    textcolor(WHITE);printf(" : <s>");

    gotoxy(36,10);textcolor(WHITE);printf("Normal");
    gotoxy(36,11);textcolor(LIGHTGRAY);printf("Hard");
    gotoxy(36,12);textcolor(DARKGRAY);printf("Very Hard");
    gotoxy(27,8);textcolor(WHITE);printf("Choose your Difficulty");

    gotoxy(34,10+x);printf(FLECHE);
    while(touche!=13)
    {
        touche=getch();
        /* Sound */
        if(tolower(touche)=='s')
        {
            if(*son==TRUE){ *son=FALSE;gotoxy(72,1);textcolor(LIGHTRED);printf("Sound");}
            else { *son=TRUE;gotoxy(72,1);textcolor(LIGHTGREEN);printf("Sound");}
        }
        else
        {
            if(touche!=13)
            {
                switch(touche)
                {
                    case HAUT : if(x>0){gotoxy(34,10+x);printf(" ");x--;}break;
                    case BAS : if(x<2){gotoxy(34,10+x);printf(" ");x++;}break;
                }
                textcolor(WHITE);gotoxy(34,10+x);printf(FLECHE);
            }
            else niveau=x+1;
        }
    }
    return niveau;
}
            /*---INSTRUCTIONS---*/
void instructions (void)
{
    CLRSCR;
    textcolor(WHITE);
    gotoxy(34,8);printf("Instructions :");
    gotoxy(3,10);printf("Move your Cursor with arrow keys, don't touch any obtacles or Game Over");
    gotoxy(3,11);printf("You can go throught Walls, at lvl 1, there is no obtacles");
    gotoxy(3,12);printf("Spped inscreese every time you take a normal point");
    gotoxy(3,14);printf("Yellow points are normal points and a temporary point at the same position");
    gotoxy(4,15);printf("They give 2 points but only 1 speed increase");
}
                /*---Start---*/
void depart(short son)
{
    CLRSCR;
    gotoxy(41,10);textcolor(LIGHTRED);printf("3");if(son==TRUE) BIP Sleep(650);clreol();
    gotoxy(41,10);textcolor(LIGHTRED);printf("2");if(son==TRUE) BIP Sleep(650);clreol();
    gotoxy(41,10);textcolor(LIGHTRED);printf("1");if(son==TRUE) BIP Sleep(650);clreol();
    gotoxy(37,10);textcolor(LIGHTGREEN);printf("GO !!!"); Sleep(550);
    textcolor(WHITE);
}
                /*---Changing lvl handeling---*/
void check_lvl( short score, short*lvl, short *vitesse,short*x,short*y,char*touche,short*aleat,short obstacle[81][26],short son)
{
    if(score==15 || score==30 || score==45 ||score==60 ||score==75 || score==90)
    {
        /* Display green score before lvl change */
        affiche_cur_score (score,LIGHTGREEN);
        Sleep(1000);

        /*---Parameters for each lvls---*/
        if( score==15 )  //Niveau 2
        {
            *lvl=2;
            *x=35;*y=5;
            *touche=DROITE;
        }
        else if( score==30 )  //Niveau 3
        {
            *lvl=3;
            *x=35;*y=12;
            *touche=DROITE;
        }
        else if( score==45 )  //Niveau 4
        {
            *lvl=4;
            *x=35;*y=5;
            *touche=DROITE;
        }
        else if( score==60 )  //Niveau 5
        {
            *lvl=5;
            *x=25;*y=5;
            *touche=GAUCHE;
        }
        else if( score==75 )  //Niveau 6
        {
            *lvl=6;
            *x=3;*y=4;
            *touche=BAS;
        }

        else if( score==90 )  //Niveau 7
        {
            *lvl=7;
            *x=3;*y=4;
            *touche=DROITE;
        }

        /*---Params for every lvl change---*/
        *vitesse=0;
        *aleat=FALSE;

        /* Display new lvl */
        affiche_lvl(*lvl);
        affiche_obstacles(*lvl,obstacle,son);

        /*---Display green cursor in new lvl + score---*/
        affiche_cur_score (score,WHITE);
        textbackground(LIGHTGREEN);MYclrwin(*x,*y,*x,*y);
        Sleep(1000);

        /* Cancel key pressed before */
        while(kbhit())getch();
    }
}
                /*---Lvl Screen---*/
void affiche_lvl (short lvl)
{
    short i;

    if(lvl>1)Sleep(1000);
    CLRSCR;
    textcolor(WHITE);
    /*---Sides---*/
    gotoxy(33,8);printf("%c",201);
    gotoxy(47,12);printf("%c",188);
    gotoxy(47,8);printf("%c",187);
    gotoxy(33,12);printf("%c",200);

    /*---Barres---*/
    for(i=0;i<13;i++)
    {
        gotoxy(34+i,8);printf("%c",205);
        gotoxy(34+i,12);printf("%c",205);
    }
    gotoxy(33,9);printf("%c",186);
    gotoxy(33,10);printf("%c",186);
    gotoxy(33,11);printf("%c",186);
    gotoxy(47,9);printf("%c",186);
    gotoxy(47,10);printf("%c",186);
    gotoxy(47,11);printf("%c",186);

    /*---The lvl---*/
    for(i=0;i<9;i++)
    {
        Sleep(100);
        switch(i)
        {
            case 0 : gotoxy(35+i,10);printf("L");break;
            case 1 : gotoxy(35+i,10);printf("E");break;
            case 2 : gotoxy(35+i,10);printf("v");break;
            case 3 : gotoxy(35+i,10);printf("e");break;
            case 4 : gotoxy(35+i,10);printf("L");break;
            case 5 : gotoxy(35+i,10);printf(" ");break;
            case 6 : gotoxy(35+i,10);printf(":");break;
            case 7 : gotoxy(35+i,10);printf(" ");break;
            case 8 : gotoxy(35+i,10);printf("%hd",lvl);break;
        }
    }
    /* lvl 6 => Display instructions */
    if(lvl==6)
    {
        textcolor(LIGHTRED);gotoxy(33,14);printf("Level from Hell !");Sleep(2000);
        textcolor(WHITE);gotoxy(10,16);printf("Special Level, Special Rules");Sleep(2000);
        gotoxy(10,17);printf("Each time you make a move",130,130);
        gotoxy(10,18);printf("A random number between 0 and 700 is calculated");Sleep(2000);
        gotoxy(10,20);printf("If this random number = 666, The Curse will be displayed !");Sleep(2000);
        gotoxy(10,21);printf("Good Luck !");Sleep(2000);
        gotoxy(15,3);   printf("[TIP] : Don't touch ");textcolor(LIGHTRED);printf("the devil's fork");textcolor(WHITE);
        gotoxy(14,4);displayDG("[INFO]   : Level 6 begins in 6 seconds.",2,79,2,WHITE);
        gotoxy(10,23);printf("Are you Ready ?");Sleep(1000);
        textcolor(LIGHTGREEN);
        gotoxy(54,4);printf("5");Sleep(1000);
        gotoxy(54,4);printf("4");Sleep(1000);
        gotoxy(54,4);printf("3");Sleep(1000);
        gotoxy(54,4);printf("2");Sleep(1000);
        gotoxy(54,4);printf("1");
    }
    else if(lvl==1)
    {gotoxy(12,4);displayDG("[INFO] : Hit < M > in Game to go back to Menu",2,79,1,WHITE);Sleep(1000);}
    Sleep(1000);
}
                /*---Display OBSTACLES---*/
void affiche_obstacles(short lvl,short obstacle[81][26],short son)
{
    short i,j;
    CLRSCR;
    textcolor(WHITE);

    clr_obstacle(obstacle);

    /*---Display sides, + fill obtacles table---*/
    if(lvl==3 || lvl==4)
    {
        gotoxy(1,2);printf("%c",201);
        gotoxy(80,2);printf("%c",187);
        gotoxy(1,25);printf("%c",200);
        gotoxy(80,25);printf("%c",188);

        for(i=1;i<79;i++)
        {
            if(i<=37 || i>=43)  //3 space hole
            {
                obstacle[1+i][2]=1;
                gotoxy(1+i,2);printf("%c",205);

                obstacle[1+i][25]=1;
                gotoxy(1+i,25);printf("%c",205);
            }
        }
        for(i=1;i<23;i++)
        {
            if(i<=8 || i>=12)   //3 space hole
            {
                obstacle[1][2+i]=1;
                gotoxy(1,2+i);printf("%c",186);

                obstacle[80][2+i]=1;
                gotoxy(80,2+i);printf("%c",186);
            }
        }
    }
    /*---Display Obstacles + fill obst tb---*/
    if(lvl==2 || lvl==4)
    {
        for(i=0;i<10;i++)
        {
            obstacle[10+i][12]=1;
            gotoxy(10+i,12);printf("%c",205);

            obstacle[60+i][12]=1;
            gotoxy(60+i,12);printf("%c",205);

            obstacle[41][7+i]=1;
            gotoxy(41,7+i);printf("%c",186);
        }
    }
    if(lvl==5)
    {
        for(i=0;i<24;i++)   //Barre verticale
        {
            obstacle[41][2+i]=1;
            gotoxy(41,2+i);printf("%c",186);
        }
        for(i=0;i<80;i++)   //Barre horizontale
        {
            obstacle[1+i][12]=1;
            gotoxy(1+i,12);printf("%c",205);
        }

        gotoxy(41,12);printf("%c",206); //Millieu
    }

    if(lvl==6)
    {
        /* 1st box */
        /* Walls */
        for(i=6;i<=21;i++)
        {
            if(i<11 || i>15)
            {
                obstacle[7][i]=1;
                gotoxy(7,i);printf("%c",186);

                obstacle[74][i]=1;
                gotoxy(74,i);printf("%c",186);
            }
        }
        for(i=7;i<=74;i++)
        {
            if(i!=39 && i!=40 && i!=41)
            {
                obstacle[i][6]=1;
                gotoxy(i,6);printf("%c",205);

                obstacle[i][21]=1;
                gotoxy(i,21);printf("%c",205);
            }
        }
        /* sides walls */
        gotoxy(7,6);printf("%c",201);
        gotoxy(74,6);printf("%c",187);
        gotoxy(7,21);printf("%c",200);
        gotoxy(74,21);printf("%c",188);

        /* second box */
        /* Walls */
        for(i=10;i<=17;i++)
        {
            obstacle[16][i]=1;
            gotoxy(16,i);printf("%c",186);

            obstacle[65][i]=1;
            gotoxy(65,i);printf("%c",186);
        }
        for(i=16;i<=64;i++)
        {
            if(i<35 || i>45)
            {
                obstacle[i][10]=1;
                gotoxy(i,10);printf("%c",205);

                obstacle[i][17]=1;
                gotoxy(i,17);printf("%c",205);
            }
        }
        /* Sides walls */
        gotoxy(16,10);printf("%c",201);
        gotoxy(65,10);printf("%c",187);
        gotoxy(16,17);printf("%c",200);
        gotoxy(65,17);printf("%c",188);

        /* The Devil's Fork */
        gotoxy(39,13);textcolor(LIGHTRED);printf("%c",192);
        gotoxy(40,13);textcolor(LIGHTRED);printf("%c",197);
        gotoxy(41,13);textcolor(LIGHTRED);printf("%c",217);
        obstacle[39][13]=1;
        obstacle[40][13]=1;
        obstacle[41][13]=1;

        gotoxy(40,14);textcolor(LIGHTRED);printf("%c",179);
        gotoxy(40,15);textcolor(LIGHTRED);printf("%c",179);
        obstacle[40][14]=1;
        obstacle[40][15]=1;

        gotoxy(39,12);textcolor(LIGHTRED);printf("%c",179);
        gotoxy(40,12);textcolor(LIGHTRED);printf("%c",179);
        gotoxy(41,12);textcolor(LIGHTRED);printf("%c",179);
        obstacle[39][12]=1;
        obstacle[40][12]=1;
        obstacle[41][12]=1;
    }

    if (lvl==7)
    {
        for(i=3;i<=25;i+=4)
        {
            for(j=2;j<=80;j+=4)
            {
                obstacle[j][i]=1;
                gotoxy(j,i);printf("%c",15);
            }
        }
    }

    /*---Display Titel---*/
    gotoxy(36,1);   textcolor(13);printf("---P");;
                    textcolor(12);printf("I");
                    textcolor(14);printf("X");
                    textcolor(10);printf("E");
                    textcolor(11);printf("L---");

    /*---Display score---*/
    gotoxy(1,1);textcolor(WHITE);printf("Score: ");
    gotoxy(12,1);textcolor(LIGHTRED);printf(POINT);

    /*---Pause Help---*/
    gotoxy(55,1);textcolor(WHITE);printf("Pause : <p>");

    /*---New lvl ---*/
    gotoxy(19,1);printf("Level : %hd",lvl);

    /*---Sound---*/
    if(son==TRUE)textcolor(LIGHTGREEN);
    else textcolor(LIGHTRED);
    gotoxy(72,1);printf("Sound");
    textcolor(WHITE);printf(" : <s>");
}

                /*---END SCORE + SCORE RECORD---*/
void fin(short score, short niveau,short triche,short son,char touche)
{
    char choix,nom[15],difficulte[15];
    Score cur_score;
    FILE *pfile=NULL;

    if(son==TRUE) {BIP BIP BIP}  //3 ssound when you loose
    Sleep(1000);
    CLRSCR;
    if( touche=='m' )
    {
        textcolor(WHITE);gotoxy(27,7);printf("Game Over");
                         gotoxy(27,8);printf("-------------------");
    }
    else {textcolor(RED);gotoxy(27,7);printf("Obstacle Hit !");}
    textcolor(WHITE);
    gotoxy(27,9);printf("Your Score : %hd ",score);textcolor(LIGHTRED);printf(POINT);
    Sleep(1500);
    if(score!=0){
    /*---Score saving---*/
    fflush(stdin);
    do
    {
        textbackground(BLACK);MYclrwin(27,11,80,11);
        textcolor(WHITE);gotoxy(27,11);printf("Save your score ? <y/n> ");
        while(kbhit()){getch();}
        fflush(stdin);scanf("%c",&choix);
    }while(toupper(choix)!='Y' && toupper(choix)!='N');

    if(toupper(choix)=='Y' && triche==FALSE)
    {
        /*---We save level difficulty---*/
        switch(niveau)
        {
            case 1 : strcpy(difficulte,"Normal");break;
            case 2 : strcpy(difficulte,"Hard");break;
            case 3 : strcpy(difficulte,"Very Hard");
        }
        difficulte[14]='\0';

        /* Text Save */
        pfile=fopen("scores.txt","a");
        if(!pfile)
        {
            gotoxy(19,15);printf("Text File   : [");textcolor(LIGHTRED);printf("ERROR");
            textcolor(WHITE);printf("]");
        }
        else
        {
            gotoxy(27,13);printf("Your Name : ");scanf("%s",nom);
            fprintf(pfile,"Name : %-15s  Score : %-4hd  Difficulty : %s\n",nom,score,difficulte);
            fclose(pfile);
            gotoxy(19,15);printf("Text File   : [");textcolor(LIGHTGREEN);printf("OK");
            textcolor(WHITE);printf("]");
        }

        /* Enregistrement mode binaire */
        pfile=fopen("scores.pixel","ab");
        if(!pfile)
        {
            textcolor(WHITE);gotoxy(19,16);printf("Pixel Score : [");
            textcolor(LIGHTRED);printf("ERROR");
        }
        else
        {
            strcpy(cur_score.nom,nom);
            cur_score.score=score;
            strcpy(cur_score.difficulte,difficulte);
            fwrite(&cur_score,sizeof(Score),1,pfile);
            fclose(pfile);
            textcolor(WHITE);gotoxy(19,16);printf("Pixel Score : [");
            textcolor(LIGHTGREEN);printf("OK");
            textcolor(WHITE);printf("]");
        }//Fin enregistrement biaire
        gotoxy(20,18);printf("<Hit any Key to go back to Menu>");
        getch();

    }
    else if(triche==TRUE && toupper(choix)=='Y')
    {
        textcolor(WHITE);gotoxy(32,13);printf("<Cheat Activated>",130);
        textcolor(WHITE);gotoxy(19,15);printf("Text File   : [");
        textcolor(LIGHTRED);printf("ERROR");textcolor(WHITE);printf("]");
        textcolor(WHITE);gotoxy(19,16);printf("Pixel Score : [");
        textcolor(LIGHTRED);printf("ERROR");textcolor(WHITE);printf("]");
        gotoxy(19,18);printf("<Hit any Key to go back to Menu>");
        getch();
    }
    else {gotoxy(20,15);printf("<Hit any Key to go back to Menu>"); getch();}
    }
    else    //If score = 0 no save
    {
        gotoxy(27,11);printf("Back to Menu ...");
        Sleep(5000);
    }
}
                /* Reset obstacle to 0 */
void clr_obstacle(short obstacle[81][26])
{
    short i,j;

    for(i=0;i<=80;i++)
        for(j=0;j<=25;j++) obstacle[i][j]=0;
}
                /* Display scores (in score.pixel) list */
void affiche_score (void)
{
    FILE *p_file;
    Score cur_score;

    CLRSCR;
    if(!(p_file=fopen("scores.pixel","rb")))
    {
        gotoxy(20,10);textcolor(WHITE);printf("[");
        textcolor(LIGHTRED);printf("ERROR");
        textcolor(WHITE);printf("] : Cannot Open \"scores.pixel\"");
        gotoxy(20,12);
    }
    else
    {
        gotoxy(10,2);
        textcolor(13);printf("S");;
        textcolor(12);printf("c");
        textcolor(14);printf("o");
        textcolor(10);printf("r");
        textcolor(11);printf("e");
        textcolor(12);printf("s");

        textcolor(5);gotoxy(10,3);printf("-------\n\n");
        textcolor(WHITE);
        while(fread(&cur_score,sizeof(Score),1,p_file) && !feof(p_file))
        {
            gotoxy(10,wherey());printf("Name : %-15s ",cur_score.nom);
            gotoxy(31,wherey());printf("Score : %-3hd ",cur_score.score);
            gotoxy(45,wherey());printf("Difficulty : %-15s \n",cur_score.difficulte);
            printf("\n");
        }
        fclose(p_file);
    }
    gotoxy(18,wherey()+1);printf("<Hit any Key to go back to Menu>");
    getch();
}
                /* TOP3 IN MENU */
void affiche_top3(void)
{
    Score cur_score,top1,top2,top3;
    FILE *pfile=NULL;

    short nb_score;

    gotoxy(33,15);printf("---Top 3---");
    if( !(pfile=fopen("scores.pixel","rb")) ){gotoxy(28,17);printf("No Scores");}
    else
    {
        /*--- Top 1 ---*/
        top1.score=nb_score=0;

        while(fread(&cur_score,sizeof(Score),1,pfile) && !feof(pfile))
        {
            if(cur_score.score>top1.score)
            {
                top1.score=cur_score.score;
                strcpy(top1.nom,cur_score.nom);
                strcpy(top1.difficulte,cur_score.difficulte);
            }
            nb_score++;
        }

        /*--- Top 2 ---*/
        rewind(pfile);
        top2.score=0;

        while(fread(&cur_score,sizeof(Score),1,pfile) && !feof(pfile))
        {
            if(cur_score.score>top2.score && cur_score.score<top1.score )
            {
                top2.score=cur_score.score;
                strcpy(top2.nom,cur_score.nom);
                strcpy(top2.difficulte,cur_score.difficulte);
            }
        }

        /*--- Top 3 ---*/
        rewind(pfile);
        top3.score=0;

        while(fread(&cur_score,sizeof(Score),1,pfile) && !feof(pfile))
        {
            if(cur_score.score<top2.score && cur_score.score<top1.score && cur_score.score>top3.score)
            {
                top3.score=cur_score.score;
                strcpy(top3.nom,cur_score.nom);
                strcpy(top3.difficulte,cur_score.difficulte);
            }
        }
        fclose(pfile);
        /* 1 */
        gotoxy(5,17);printf("1] Name : %-15s Score : %-3hd Difficulty : %-15s\n",top1.nom,top1.score,top1.difficulte);
        /* 2 */
        if(nb_score>=2)
        {gotoxy(5,18);printf("2] Name : %-15s Score : %-3hd Difficulty : %-15s\n",top2.nom,top2.score,top2.difficulte);}
        /* 3 */
        if(nb_score>=3)
        {gotoxy(5,19);printf("3] Name : %-15s Score : %-3hd Difficulty : %-15s\n",top3.nom,top3.score,top3.difficulte);}
    }
}
                    /* Random calculation with Obts and score params */
void calcul_aleat(int*randx,int*randy,short score,short obstacle[81][26])
{
    short again,i,j;

    /*---Random nb---*/
    srand(time(NULL));
    /* lvl 3 and 4 with walls */
    if(score>=30 && score<=59)
    {
        *randx=(rand()%77)+2;
        *randy=(rand()%21)+3;
    }
    /* others */
    else
    {
        *randx=(rand()%78)+1;
        *randy=(rand()%22)+2;
    }

    /* Check if random point is not inside OBST */
    again=TRUE;
    for(i=1;again==TRUE && i<=80;i++)
    {
        for(j=2;again==TRUE && j<=25;j++)
        {
            if(again==TRUE && obstacle[*randx][*randy]==1) again=FALSE;
        }
    }
    /* 5/5 pos if random point is inside obstacle */
    if(again==FALSE)*randx=*randy=5;
}
                    /* Display current Score */
void affiche_cur_score (short score, short couleur)
{
        textbackground(BLACK);
        textcolor(couleur);gotoxy(8,1);printf("%3hd",score);
}

void displayDG(char *chaine, short debut, short fin, short timing, short color)
{
  short i, j, cpt, arret, ln_chaine;

     textcolor(color);
     ln_chaine=strlen(chaine);
     arret=((debut+fin)/2)-(ln_chaine/2);
     i=fin;

     cpt=1;
     while(arret<=i)
     {
       gotoxy(i,wherey());
       if(cpt<ln_chaine)
       {
          for(j=0;j<cpt;j++) putchar(chaine[j]);
          cpt++;
       }
       else
       {
          MYclrwin(debut,wherey(),fin,wherey());
          gotoxy(i,wherey());
          printf("%s",chaine);
       }
       i--;
       Sleep(timing*10);
     }
}

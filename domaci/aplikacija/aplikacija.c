#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

FILE *fp
char *str

void menu()
{    //ispis jednostavnih poruka na stdout
      puts("1: Pročitaj trenutno stanje stringa");
      puts("2: Upiši novi string"):
      puts("3: Konkataniraj string na trenutni");
      puts("4: Izbriši čitav string");
      puts("5: Izbriši vodeće i prateće space karaktere");
      puts("6: Izbriši izraz iz stringa");
      puts("7: Izbriši poslednjih n karaktera iz stringa");
      puts("Q: aplikacija se terminira");
}

void ffopenread()
{
  fp = fopen("/dev/ppp", "r");  
		if(fp == NULL)
		{
			printf("Problem pri otvaranju /dev/ppp");  
			return -1;                     
		}
}

void ffopenwrite()
{
  fp = fopen("/dev/ppp", "w");  
		if(fp == NULL)
		{
			printf("Problem pri otvaranju /dev/ppp");  
			return -1;                     
		}
}

void ffclose()
{
                if(fclose(fp))  //posto je zavrseno sa citanjem fajl se zatvara
		{
			puts("Problem pri zatvaranju /dev/ppp");
			return -1;
		}
}

int main()
{
     char opcija;
     char string[100];
     int i;
      while(1)
      {
        menu();
        scanf("%s", string);
        opcija=string[0];
        
          switch(opcija)
          {
            case '1': ffopenread();
                      fscanf(fp, "%s", string);
                      printf("^s\n", string);
                      ffclose();
                      break;
            case '2': puts("Upiši novi string");
                      scanf("%s", string);
                      ffopenwrite();
                      fprintf(fp, "string=%s\n", string);
                      ffclose();
                      break;
            case '3': puts("Unesi string koji treba da se konkatanira");
                      scanf("%s",string);
                      ffopenwrite();
                      fprintf(fp, "append=%s\n", string);
                      ffclose();
                      break;
            case '4': ffopenwrite();
                      fprintf(fp, "clear\n");
                      ffclose();
                      break;
            case '5': ffopenwrite();
                      fprintf(fp, "shrink\n");
                      ffclose();
                      break;
            case '6': puts("Unesi string koji treba da se ukloni");
                      scanf("%s",string);
                      ffopenwrite();
                      fprintf(fp, "remove=%s\n", string);
                      ffclose();
                      break;
            case '7': puts("Koliko karaktera ima string?");
                      scanf("%s",string);
                      i=atoi(string);
                      ffopenwrite();
                      fprintf(fp, "truncate=%d\n", i);
                      ffclose();
                      break;
					  */
            case 'Q': puts("Game over");
                        exit(0);
                        break;
            default: puts("Try again");
          }
     }
}
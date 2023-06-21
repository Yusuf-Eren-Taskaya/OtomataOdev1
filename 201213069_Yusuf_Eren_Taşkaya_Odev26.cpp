#include<stdio.h>
#include <stdlib.h>
#include<string.h>


#define MAX_SAT_UZ 100


int durumSayisi=0;
int sutunSayisi=0;

int** gecisTablosu;
char* basliklar;
char girisStringi[MAX_SAT_UZ];


int CharSay(char* s,char c)
{
	int sy=0;
	int i=0;
	while(s[i]!='\0')
	{
		if(s[i]==c)sy++;
		i++;
	}
	return sy;
}

void DosyaOku()
{
    /***************************************
    GECISTABLOSU.TXT gecis tablosunu tasir.
    ilk satir basliklari tasir ve tek karakterden olusur
        Q   a   b   ...     Op
        Q       -> Sonlu durumlar kumesi (bu sutunun altinda durum indisleri bulunur)
        a,b,... -> Girdi alfabesi (sembolleri)(sutunlar altinda gecis yapacagi durum indisi bulunur)
        C      -> Son sutun, ciktilar (cikti alfabesi bu sutundaki  sembollerden olusur)
        her sutun tek bir tab ile ayrilir.

    ****************************************/
	char satir[MAX_SAT_UZ];
	int temp=0;
	FILE *fp;
	if ((fp=fopen("GECISTABLOSU.TXT", "r")) == NULL) {
      printf("GECISTABLOSU.TXT Dosyasi acilamadi!\n");
      exit(1);
	}

	if(fgets(satir, MAX_SAT_UZ, fp)==NULL)
    {
        printf("Dosyada uygun veri yok.!\n");
        exit(1);
    }
    else
    {
        // sutun sayisi bulunuyor
        sutunSayisi=CharSay(satir,'\t')+1;
        if(sutunSayisi<3)
        {
            printf("Dosyada en az bir durum, bir giris, bir cikis verisi bulunmali.!\n");
            exit(1);
        }
        basliklar =(char*)malloc(sutunSayisi * sizeof(char));// baslik dizisi olusturuluyor
        for(int i=0;i<strlen(satir);i++)
        {
            if(satir[i]!='\t')
            {
                basliklar[temp++]=satir[i];   // basliklar diziye ataniyor
            }
        }

    }

    while (fgets(satir, MAX_SAT_UZ, fp)!=NULL)
    {
        //satirlar sayilarak durum sayisi bulunuyor
        durumSayisi++;
    }
    fclose (fp);


    gecisTablosu =(int**)malloc(durumSayisi * sizeof(int*));   //2 boyutlu gecis tablosu icin yer ayriliyor
    for(int i=0;i<durumSayisi;i++)
    {
        gecisTablosu[i]=(int*)malloc(sutunSayisi * sizeof(int));
    }

    fp=fopen("GECISTABLOSU.TXT", "r");
    fgets(satir, MAX_SAT_UZ, fp);// baslik satiri geciliyor

    for(int i=0;i<durumSayisi;i++)
    {
        for(int j=0;j<sutunSayisi;j++)
        {
            fscanf(fp,"%d",&gecisTablosu[i][j]);
        }
    }
    fclose(fp);
}

void TabloyuYaz()
{
    int temp=0;
    printf("             Durum Kumesi :{");
    for(int i=0;i<durumSayisi;i++)printf("%c%d,",basliklar[0],i);
    printf("\b}\n");
    printf("    Giris sembol Alfabesi :{");
    for(int i=1;i<sutunSayisi-1;i++)printf("%c,",basliklar[i]);
    printf("\b}\n");
    printf("Cikti sembolleri Alfabesi :{");
    for(int i=0;i<durumSayisi;i++)
    {
        temp=0;
        for(int j=0;j<i;j++)
        {
            if(gecisTablosu[i][sutunSayisi-1]==gecisTablosu[j][sutunSayisi-1])
            {
                temp=1;
                break;
            }
        }
        if(temp!=1)printf("%d,",gecisTablosu[i][sutunSayisi-1]);

    }
    printf("\b}\n\n");

    for(int i=0;i<sutunSayisi;i++)printf("  %c  |",basliklar[i]);
    printf("\n");
    for(int i=0;i<sutunSayisi;i++)printf("------");
    printf("\n");
    for(int i=0;i<durumSayisi;i++)
    {
        for(int j=0;j<sutunSayisi;j++)
        {
            if(j==0)
            printf(" Q%d  |",gecisTablosu[i][j]);
            else if(j<sutunSayisi-1)
                printf("  Q%d ",gecisTablosu[i][j]);
            else
                printf(" |  %d  ",gecisTablosu[i][j]);


        }
        printf("\n");
    }
}

int GirisSembolInisi(char s)
{
    //Giris karakterini tablodaki index numarasini dondurur
    int ndx=0;
    for(int i=1;i<sutunSayisi-1;i++)
    {
        if(basliklar[i]==s)
        {
            ndx=i;
            break;
        }
    }
    ndx--;
    if(ndx<0 || ndx>=sutunSayisi-1)
    {
        printf("\n'%c' karakteri giris alfabesine uygun degil.!\n",s);
        exit(1);
    }
    return ndx;
}
void DurumlarArasiGecis(char * giris)
{
    int drm=0,drm_old=0;

    printf("Mevcut           Gecilen  \n");
    printf("Durum   Giris    Durum    Cikis\n");
    printf("-----   -----    -----    -----\n");
    printf("  Q0      -        Q0      %d\n",gecisTablosu[drm][sutunSayisi-1]);
    for(int i=0;i<strlen(giris);i++)
    {
        drm_old=drm;
        drm=gecisTablosu[drm][GirisSembolInisi(giris[i])+1];
        printf("  Q%d      %c        Q%d      %d\n",drm_old,giris[i],drm,gecisTablosu[drm][sutunSayisi-1]);
    }
}
int main()
{
    DosyaOku();
    TabloyuYaz();
    printf("\nGiris stringini gir (arada bosluk birakmadan) :");
    scanf("%s",girisStringi);
    printf("\n");
    DurumlarArasiGecis(girisStringi);
}


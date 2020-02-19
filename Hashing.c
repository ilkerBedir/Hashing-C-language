#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<math.h>
typedef struct{ //hash tablosunu olusturcak dosyalar�n� yap�s� 1-dosya ismi,2-icindekiler
	char dosya_ismi[50];
	char icindekiler[1000];
}hash;
void dosya_olusturma(FILE *sample,FILE *samples);//dosya eklemek icin olsuturdugum fonksiyon
void hash_tablosu_olusturma(FILE *sample,FILE *samples);//verilen dosyalara g�re hash talosu yapma
int deger_hesapla(char *icerik,int j);//burda ald�g�m kelimeye g�re key hesaplama
int hash_function1(unsigned long int key,int uz);//key degeri el ile belirledigim fonksiyona g�re mod al 
int hash_function2(unsigned long key,int uz);//yine ayn� sekilde ama 1 ekliyorum h2
int main(int argc, char *argv[]) {
	FILE *sample;//dosyan�n iceri bulundugu dosya
    FILE *samples;//dosyalar�n isimlerinin bulundugu dosya
	int secim;//men� icin secim
	while(secim!=3){//burada men� yapt�m dosya ekleme ve hash tablosu diye 2 secenek var
	printf("1-)Dosya ekle\n2-)Hash tablosu olustur/goruntule\n3-)exit\n");
    scanf("%d",&secim);
    switch(secim)
    {
       case 1: dosya_olusturma(sample,samples); break;  //dosya ekleme
       case 2: hash_tablosu_olusturma(sample,samples); break;//hash tablosu olu�turma
       case 3 : printf("CIKIS");break;//��k��
       default : printf("Gecersiz secim"); break; 
    }
    } 
    getch();      
	return 0;
}
void hash_tablosu_olusturma(FILE *sample,FILE *samples){//burda dosyalar� okuyacag�m icin dosyalar� parametre g�sterdim
    unsigned long int key;//key degerim buyuk c�kt�g� i�in unsigned long key kulland�m
    char sample_name[50];//dosya ismi
	char *icerik;//icerigi dinamik tan�mlad�m
	char *kelime;//icerikte her bir kelime i�in ayr� key istedigi i�in kelime dizisi dinamik tan�mlad�m
	int c,m,k,j;//c=icerikteki karakter say�s�,m=hash2 fonksiyonu i�in indis;k dizi degiskeni,j dizi degiskeni
	char er,ch;//dosyalardan teker karakter almak i�in 
	hash *hash_table;//hash tablom
	hash_table=(hash*)calloc(11,sizeof(hash));//burada 11 yi el ile hesaplad�m 
	int indis;//hash tablosu i�in indis
	samples=fopen("samples.txt","r");//isimlerin buldugu dosyay� ac�p okuyorum
	if(samples==NULL){
		printf("dosya yoktur..\n");
	}
	else{
		while(!feof(samples)){
			fscanf(samples,"%s",&sample_name);
			sample=fopen(sample_name,"r");//icerik bulundugu dosyay� ac�yorum
			if(sample==NULL){
				printf("dosyanin icerigi yoktur\n");
			}
			else{
				c=0;
				er=getc(sample);
				c++;
				while(er!=EOF){//dinamik tasarlamak i�in b�yle yapt�m
					er=getc(sample);
					c++;
				}
				if(c==1){//do�ruysa karakter okumas� ald�m
						printf("dosyanin icerigi yoktur\n");
				}
				icerik=(char*)malloc(c*sizeof(char));
				rewind(sample);//sat�r ba��na geldik
				while((ch=getc(sample))!=EOF){
				rewind(sample);//sat�r ba��na geldik
				fgets(icerik,c,sample);//dosyadan icerigi al�yorum
				ch=getc(sample);	//burada t�m karakterleri almas� i�in b�yle yapt�m
				key=0;//key ba�ta 0
				k=0;
				indis=0;//ba�ta 0;
				kelime=(char*)malloc(1*sizeof(char));
			    for(k=0;k<c;k++){// for d�ng�s�nde icerigin sonuna gelindiginde ve kactane kelime varsa ayr� her kelimenin key degerini hesaplay�p indisi toplama yapt�m. 
			    	j=0;
			    	while(icerik[k]!=' ' && icerik[k]!='\0'){//icerik bosluk gelene kadar ve icerigin bitimine kadar kelime dizisine kopyal�yor karakter
			    		kelime[j]=icerik[k];
			    		kelime=(char*)realloc(kelime,j+2);//boyut att�rma
			    		k++;
			    		j++;
					}
					if(icerik[k]==' '){//eger bo�luk varsa bo�lu�u da kelimeye eklyip hesapl�yorum
						kelime[j]=icerik[k];
						j++;
				    }
					key=deger_hesapla(kelime,j);//key hesaplama
					indis+=hash_function1(key,11);//indis hesaplama
					indis=indis%11;//eger indis 11 den b�y�kse mod al�p yazma
				}
			
				if(hash_table[indis].dosya_ismi[0]=='\0'){//burda daha �nce buldugum indisi hash tablosunda yeri varm� kontrolu eger bo� ise yazd�rma
					strcpy(hash_table[indis].dosya_ismi,sample_name);
					strcpy(hash_table[indis].icindekiler,icerik);
					printf("%s isimli dosya %d indisindedir\n",sample_name,indis);
				}
				else{//bo� degilse eger dosya farkl� ise 2.hash fonksiyonumu bo� yer bulana kadar cal�st�r�yorum. 
					m=1;
					while(  strcmp(hash_table[indis].icindekiler,icerik)!=0 && hash_table[indis].dosya_ismi[0]!='\0' ){
						indis=hash_function1((hash_function1(key,11)+hash_function2(key,10)*m),11);
						m++;
						
					}
					if( strcmp(hash_table[indis].icindekiler,icerik)==0 ){
					printf("HATA,ayni icerikli dosya mevcut...\n");//eger dosya ayn� ise islem tamamlanm�st�r
					}
					else{//degilse yazd�rma
					strcpy(hash_table[indis].dosya_ismi,sample_name);
					strcpy(hash_table[indis].icindekiler,icerik);
					printf("%s isimli dosya %d indisindedir\n",sample_name,indis);
					}
				}				
				}
			}
		}
		fclose(sample);
	}
	fclose(samples);
}
    int deger_hesapla(char *icerik,int j){//kelimele ve uzunluklar�n� kullanarak verilen form�le g�re hash olusturma 
	int r=31;//asal say� olmal�d�r genelde 31 say�s� kullan�l�r
	int i;
	unsigned long int key;
	key=0;
	for(i=0;i<j;i++){
		key=key+(icerik[i] * pow(r,j-i-1));
	}
	return key;
}
void dosya_olusturma(FILE *sample,FILE *samples){//dosya yazd�rma olusturma fonksiyonu
	char sample_name[50];//dosya isim
	char *icerik;//dosya icerigi
	char ch;
	int c;
	c=0;
	samples=fopen("samples.txt","a+");//dosyalar�n isimleri bulundugu dosyay� acma
	printf("dosya adini giriniz(.txt formatinda)\n");//dosyay� al�yorum kullan�c�dan
	scanf("%s",&sample_name);
	fprintf(samples,"\n %s",sample_name);//dosya ismini okuyorum
	sample=fopen(sample_name,"w");//ac�yorum ve yazd�r�yorum
	printf("dosyanin icini yaziniz\n");
	gets(icerik);
	while( (ch=getchar())!='\n')
    {
        putc(ch,sample);
		c++; 
    }
    icerik=(char*)calloc(c,sizeof(char));
	fprintf(sample,"%s",icerik);//icerik dosyaya yazd�r�yorum
	fclose(sample);
	fclose(samples);
}
int hash_function1(unsigned long int key,int uz){//hash1 fonksiyonum burada uz yi 11 olarak hesapl�yorum el ile
	int indis;
	indis=(key%uz);
	return indis;
}
int hash_function2(unsigned long int key,int uz){//hash2 fonksiyonum burada uz yi 10 olarak hesapl�yorum el ile
	int indis;
	indis=1+(key%uz);
	return indis;
}



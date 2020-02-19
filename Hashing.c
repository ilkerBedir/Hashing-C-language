#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<math.h>
typedef struct{ //hash tablosunu olusturcak dosyalarýný yapýsý 1-dosya ismi,2-icindekiler
	char dosya_ismi[50];
	char icindekiler[1000];
}hash;
void dosya_olusturma(FILE *sample,FILE *samples);//dosya eklemek icin olsuturdugum fonksiyon
void hash_tablosu_olusturma(FILE *sample,FILE *samples);//verilen dosyalara göre hash talosu yapma
int deger_hesapla(char *icerik,int j);//burda aldýgým kelimeye göre key hesaplama
int hash_function1(unsigned long int key,int uz);//key degeri el ile belirledigim fonksiyona göre mod al 
int hash_function2(unsigned long key,int uz);//yine ayný sekilde ama 1 ekliyorum h2
int main(int argc, char *argv[]) {
	FILE *sample;//dosyanýn iceri bulundugu dosya
    FILE *samples;//dosyalarýn isimlerinin bulundugu dosya
	int secim;//menü icin secim
	while(secim!=3){//burada menü yaptým dosya ekleme ve hash tablosu diye 2 secenek var
	printf("1-)Dosya ekle\n2-)Hash tablosu olustur/goruntule\n3-)exit\n");
    scanf("%d",&secim);
    switch(secim)
    {
       case 1: dosya_olusturma(sample,samples); break;  //dosya ekleme
       case 2: hash_tablosu_olusturma(sample,samples); break;//hash tablosu oluþturma
       case 3 : printf("CIKIS");break;//çýkýþ
       default : printf("Gecersiz secim"); break; 
    }
    } 
    getch();      
	return 0;
}
void hash_tablosu_olusturma(FILE *sample,FILE *samples){//burda dosyalarý okuyacagým icin dosyalarý parametre gösterdim
    unsigned long int key;//key degerim buyuk cýktýgý için unsigned long key kullandým
    char sample_name[50];//dosya ismi
	char *icerik;//icerigi dinamik tanýmladým
	char *kelime;//icerikte her bir kelime için ayrý key istedigi için kelime dizisi dinamik tanýmladým
	int c,m,k,j;//c=icerikteki karakter sayýsý,m=hash2 fonksiyonu için indis;k dizi degiskeni,j dizi degiskeni
	char er,ch;//dosyalardan teker karakter almak için 
	hash *hash_table;//hash tablom
	hash_table=(hash*)calloc(11,sizeof(hash));//burada 11 yi el ile hesapladým 
	int indis;//hash tablosu için indis
	samples=fopen("samples.txt","r");//isimlerin buldugu dosyayý acýp okuyorum
	if(samples==NULL){
		printf("dosya yoktur..\n");
	}
	else{
		while(!feof(samples)){
			fscanf(samples,"%s",&sample_name);
			sample=fopen(sample_name,"r");//icerik bulundugu dosyayý acýyorum
			if(sample==NULL){
				printf("dosyanin icerigi yoktur\n");
			}
			else{
				c=0;
				er=getc(sample);
				c++;
				while(er!=EOF){//dinamik tasarlamak için böyle yaptým
					er=getc(sample);
					c++;
				}
				if(c==1){//doðruysa karakter okumasý aldým
						printf("dosyanin icerigi yoktur\n");
				}
				icerik=(char*)malloc(c*sizeof(char));
				rewind(sample);//satýr baþýna geldik
				while((ch=getc(sample))!=EOF){
				rewind(sample);//satýr baþýna geldik
				fgets(icerik,c,sample);//dosyadan icerigi alýyorum
				ch=getc(sample);	//burada tüm karakterleri almasý için böyle yaptým
				key=0;//key baþta 0
				k=0;
				indis=0;//baþta 0;
				kelime=(char*)malloc(1*sizeof(char));
			    for(k=0;k<c;k++){// for döngüsünde icerigin sonuna gelindiginde ve kactane kelime varsa ayrý her kelimenin key degerini hesaplayýp indisi toplama yaptým. 
			    	j=0;
			    	while(icerik[k]!=' ' && icerik[k]!='\0'){//icerik bosluk gelene kadar ve icerigin bitimine kadar kelime dizisine kopyalýyor karakter
			    		kelime[j]=icerik[k];
			    		kelime=(char*)realloc(kelime,j+2);//boyut attýrma
			    		k++;
			    		j++;
					}
					if(icerik[k]==' '){//eger boþluk varsa boþluðu da kelimeye eklyip hesaplýyorum
						kelime[j]=icerik[k];
						j++;
				    }
					key=deger_hesapla(kelime,j);//key hesaplama
					indis+=hash_function1(key,11);//indis hesaplama
					indis=indis%11;//eger indis 11 den büyükse mod alýp yazma
				}
			
				if(hash_table[indis].dosya_ismi[0]=='\0'){//burda daha önce buldugum indisi hash tablosunda yeri varmý kontrolu eger boþ ise yazdýrma
					strcpy(hash_table[indis].dosya_ismi,sample_name);
					strcpy(hash_table[indis].icindekiler,icerik);
					printf("%s isimli dosya %d indisindedir\n",sample_name,indis);
				}
				else{//boþ degilse eger dosya farklý ise 2.hash fonksiyonumu boþ yer bulana kadar calýstýrýyorum. 
					m=1;
					while(  strcmp(hash_table[indis].icindekiler,icerik)!=0 && hash_table[indis].dosya_ismi[0]!='\0' ){
						indis=hash_function1((hash_function1(key,11)+hash_function2(key,10)*m),11);
						m++;
						
					}
					if( strcmp(hash_table[indis].icindekiler,icerik)==0 ){
					printf("HATA,ayni icerikli dosya mevcut...\n");//eger dosya ayný ise islem tamamlanmýstýr
					}
					else{//degilse yazdýrma
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
    int deger_hesapla(char *icerik,int j){//kelimele ve uzunluklarýný kullanarak verilen formüle göre hash olusturma 
	int r=31;//asal sayý olmalýdýr genelde 31 sayýsý kullanýlýr
	int i;
	unsigned long int key;
	key=0;
	for(i=0;i<j;i++){
		key=key+(icerik[i] * pow(r,j-i-1));
	}
	return key;
}
void dosya_olusturma(FILE *sample,FILE *samples){//dosya yazdýrma olusturma fonksiyonu
	char sample_name[50];//dosya isim
	char *icerik;//dosya icerigi
	char ch;
	int c;
	c=0;
	samples=fopen("samples.txt","a+");//dosyalarýn isimleri bulundugu dosyayý acma
	printf("dosya adini giriniz(.txt formatinda)\n");//dosyayý alýyorum kullanýcýdan
	scanf("%s",&sample_name);
	fprintf(samples,"\n %s",sample_name);//dosya ismini okuyorum
	sample=fopen(sample_name,"w");//acýyorum ve yazdýrýyorum
	printf("dosyanin icini yaziniz\n");
	gets(icerik);
	while( (ch=getchar())!='\n')
    {
        putc(ch,sample);
		c++; 
    }
    icerik=(char*)calloc(c,sizeof(char));
	fprintf(sample,"%s",icerik);//icerik dosyaya yazdýrýyorum
	fclose(sample);
	fclose(samples);
}
int hash_function1(unsigned long int key,int uz){//hash1 fonksiyonum burada uz yi 11 olarak hesaplýyorum el ile
	int indis;
	indis=(key%uz);
	return indis;
}
int hash_function2(unsigned long int key,int uz){//hash2 fonksiyonum burada uz yi 10 olarak hesaplýyorum el ile
	int indis;
	indis=1+(key%uz);
	return indis;
}



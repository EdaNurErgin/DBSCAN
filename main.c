#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*define ifadesi,bir isim icin belirli bir deger atamasini saglayan c programa dilinde kullanilan bir
on islemci direktividir.*/
#define MAX_NOKTA 100 //bir dizi veya bir veri yapisindaki islenebilecek maksimum nokta sayisini temsil eder
#define EPSILON 2.5 //dbscan algoritmasinda iki nokta arasindaki maksimum uzaklik olarak kullanilan bir esik degeridir
#define MIN_NOKTA 4 /*bu sabit bir noktanin cekirdek nokta olarak kabuk edilebilmesi icin gereken minimum komsu
sayisini belirtir */



//Eda adinda bir yapi tanimlanir
//bu yapi sayesinde 2D uzayda bir noktanin koordinatlari bir arada tutulabilir.
typedef struct {//typedef sayesinde struct ifadesinin sadece adini kullanarak tanimlama yapabiliriz.
    double x; //bu eleman bir noktanin x-koordinatini temsil eder
    double y; //bu eleman bir noktanin y-koordinatini temsil eder.
} Eda; //struct yapisinin adi verilmis


//Nur adinda bir yapi tanimlanir
//Bu yapi bir noktanin ziyaret durumunu ve ait oldugu kume numarasini tutmak icin kullanilir
typedef struct {
    int ziyaretlenmis;//bir noktanin ziyaret edilip edilmedigini belirlemede kullanilacak
    //visited = 0 ise nokta henuz ziyaret edilmedi visited=1 ise nokta ziyaret edildi
    int kume_no;//noktanin ait oldugu kume numarasini temsil eder.
} Nur;


//bu fonksiyon iki noktanin birbirine komsu olup olmadigini kontrol eder.
int komsu_mu(Eda m1, Eda m2) {
	//pow(x,y)=x^y , sqrt=karekök alir
    double mesafe = sqrt(pow(m2.x - m1.x, 2) + pow(m2.y - m1.y, 2));/*iki nokta arasindaki mesafe hesaplanir 
	ve bu deger mesafe degiskenine atanir*/
    return mesafe <= EPSILON; //mesafe degeri EPSILON  degerinden kucuk veya esitse fonksiyon deger dondurur.
}



/*bu fonksiyon komsuluk sorgulamasini gerceklestir.Verilen bir noktanin etrafindaki komsu noktalarini bulmak icin
kullanilir ve bu komsu noktalari, belirli bir esik degeriyle (epsilon) karsilastirarak 
belirli bir kume numarasi verir */
int cevredeki_komsu_bul(Eda noktalar[], Nur durum[], int noktalar_index, int noktalar_nokta_sayisi, int kume_id) {//noktalar dizisindeki nokta sayisi=noktalar_nokta_sayisi
    int komsu_sayisi = 0;//bir noktanin baslangicta sahip oldugu komsu sayisi 0'a atanir
    int i;//dongu icin
    
    for (i = 0; i < noktalar_nokta_sayisi; i++) {//dongu, noktalar_nokta_sayisi 'a kadar devam eder ve
	// her nokta icin komsuluk kontrolu yapar
        if (i != noktalar_index && komsu_mu(noktalar[noktalar_index], noktalar[i])) {/*noktalar[noktalar_index]'in temsil ettigi nokta ile 
		//noktalar[i]'nin temsil ettigi nokta birbirine komsu ve i degeri noktalar_index 'e esit degil ise ife gir*/
		//i!=noktalar_index ->Bu kontrol, islenen noktanin kendisiyle karsilastirilmasini önlemek için eklenir.
            if (durum[i].ziyaretlenmis==0) {//durum[i].ziyaretlenmis= 0 ise yani nokta ziyaretlenmemis ise if icine gir
                durum[i].ziyaretlenmis = 1;//nokta ziyaretlenmis demektir.
                komsu_sayisi++;//komsu_sayisi ni bir arttir
                
                int yeni_komsu_sayisi = cevredeki_komsu_bul(noktalar, durum, i, noktalar_nokta_sayisi, kume_id);
                //cevredeki_komsu_bul fonksiyonu cagrilarak elde edilen deger yeni_komsu_sayisi 'a atanir
                komsu_sayisi += yeni_komsu_sayisi; //komsu_sayisi'a yeni_komsu_sayisi eklenir
            }
            
            if (durum[i].kume_no == 0) {//durum[i] noktasinin kume numarasi yoksa if icine gir
                durum[i].kume_no = kume_id; //noktaya kuma numarasi atanir.
            }
        }
    }
    
    return komsu_sayisi;
}
//bu fonksiyon dbscan algoritmasini aciklar
void dbscan(Eda noktalar[], int noktalar_nokta_sayisi) {//parametrelirimiz=dizi tanimi ,dizideki nokta sayisi
    Nur durum[MAX_NOKTA]={0}; //Nur türünde durum dizisi tanimlanir MAX_NOKTA tane eleman icerecek sekilde
    int kume_no = 1;//kume numarasi bire atanir.her bir kumeye farkli bir kume numarasi atanacaktir.
    int i;
    
    for (i = 0; i < noktalar_nokta_sayisi; i++) {//dongu 0dan baslayarak noktalar_nokta_sayisi'a kadar devam eder
    //bu sayede noktalar dizisi icinde gezecegiz
        if (durum[i].ziyaretlenmis==0) {//durum[i] noktasi ziyaretlenmemisse if icine gir
            durum[i].ziyaretlenmis = 1; //durum[i] noktasi ziyaretlendi.
            
            int komsu_sayisi = cevredeki_komsu_bul(noktalar, durum, i, noktalar_nokta_sayisi, kume_no);
            /*cevredeki_komsu_bul fonksiyonunu cagirarak ilgili noktanin komsu sayisi degeri bulunur.
			Bu deger komsu_sayisi degiskenine atanir*/
            
           if (komsu_sayisi >= MIN_NOKTA) {//komsu_sayisi MIN_NOKTA dan buyuk veya esitse if icine gir
                kume_no++;//kume numarasi bir arttirilir.
            }
        }
    }
    
    //Sonuclari yazdirma
    //burada noktalarin x - y koordinat elemanlari ve noktanin kume nosu yazdirilir.
    for (i = 0; i < noktalar_nokta_sayisi; i++) {
        printf("Nokta %d --> Koordinatlar: (%lf, %lf) - Kume ID: %d\n", i+1, noktalar[i].x, noktalar[i].y, durum[i].kume_no);
    }
}

int main() {
    Eda noktalar[MAX_NOKTA] = { //dizimizi olusturduk
       /*  {1.0, 1.0},
        {1.5, 2.0},
        {2.0, 1.5},
        {2.5, 2.0},
        {5.0, 5.0},
        {5.5, 6.0},
        {6.0, 5.5},
        {6.5, 6.0},
        {8.0, 8.0},
        {8.5, 9.0},
        {9.0, 8.5} */
        
         {1.0, 2.5},
        {8.0, 2.0},
        {7.0, 1.0}
      
    };
    
    dbscan(noktalar, 3);//dbscan fonksiyonunu cagirdik
    
    return 0;
}

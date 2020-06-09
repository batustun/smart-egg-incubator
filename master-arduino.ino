#include <Wire.h>
#include <LiquidCrystal.h> // LCD ekranı kütüphanemizden ekliyoruz.

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // LCD ekranımızın pinlerini ardiunomuzda bağladığımız bacakların numaralarını yazıyoruz.


int sensordeger; // Sensördeğer tanıtıyoruz

 int sensoryuzdesi; // sensör yüzde tanıtıyoruz

int led=10; // ledimizi tanıtıyoruz

int dcmotor=7; // motorumuzu tanıtıyoruz

int sensor = A0; // gaz sensörümüzü tanıtıyoruz

int buzzer = 13; // Buzzerımızı tanıtıyoruz
int sicaklik_degeri;
int voltaj_degeri  ;
int sensor_degeri ; // sıcaklık sensör değeri

// Ampul ve fan 220 v ile besleneceği için röle kullanmak 
// gerekir. İkisininde pinlerini tanımladı.
const int ampul_role = 8 ;    
const int fan_role = 9 ;
int dogruluk = 1 ;



void setup() {
  pinMode(buzzer, OUTPUT); // pinimizi çıkış olarak belirliyoruz
  pinMode(led, OUTPUT); // pinimizi çıkış olarak belirliyoruz
  pinMode(sensor, INPUT); // pinimizi çıkış olarak belirliyoruz
  pinMode(ampul_role, OUTPUT);
  pinMode(fan_role, OUTPUT);
  lcd.begin(16, 2); // LCD ekranımızın satır ve sütununu belirliyoruz
  Wire.begin(1);
  /* I2C haberleşmesi, haberleşme adresi 1 olan bir slave cihaz olarak başlatıldı */
  Wire.onRequest(istekGeldiginde);
  /* 
  Master olan cihaz bu Arduino'dan veri istediğinde gerçekleşecek işlem seçildi
  */
  Serial.begin(9600);

}

void loop() {

  int sensor_degeri=analogRead(A1);
  
  voltaj_degeri = ( sensor_degeri / 1023.0 ) * 5000 ;
  Serial.println(voltaj_degeri);
  sicaklik_degeri = voltaj_degeri / 30.0 ;
  
  if ( sicaklik_degeri < 30 && dogruluk == 1 )
  {
    digitalWrite(ampul_role, HIGH);
    digitalWrite(fan_role, LOW);
  }
  else 
  {
    dogruluk = 0 ;
    digitalWrite(ampul_role, LOW);
    digitalWrite(fan_role, HIGH);
  
   if( sicaklik_degeri < 15 )
    {
      dogruluk = 1 ;
    }
  }
  lcd.setCursor(0, 1); 
  lcd.print("Sicaklik ");
  lcd.print(sicaklik_degeri);
  lcd.print("C ");
  //delay(1000);
 
 sensordeger = analogRead(sensor); // Sensörümüzün yüzdesini ölçüyoruz
 sensoryuzdesi=map(sensordeger ,487,877,0,100);
  
  if(sensoryuzdesi > 20){ // eğer sensör yüzdesi 20'den büyükse motorumuzu çalıştır led'imizi yak diyoruz
    
     digitalWrite(dcmotor, HIGH);
     digitalWrite(led, HIGH);
    
    
    tone(buzzer,400); // sensör yüzdesi 400 olunca buzzerımız çalsın diyoruz.
   

  }
  
  else // eğer yüzdemiz küçük ise motorumuz dursun led'imiz sönsün ve buzzerımız sussun diyoruz
  {
    
        digitalWrite(dcmotor, LOW);
        digitalWrite(led, LOW);
    
       noTone(buzzer);
    
  }
  lcd.setCursor(0, 0);  // ekranımıza gaz yüzdesini yazdırıyoruz
  lcd.print("Gaz Yuzde ");
  lcd.print(sensoryuzdesi);
  
 // Serial.print("Gaz Yuzde "); // seri ekranımıza gaz yüzdesini yazdırıyoruz.
  Serial.println(sensoryuzdesi);
  delay(1000); // devremizin daha verimli çalışabilmesi için delay süre komutumuzu koyuyoruz. 
//lcd.clear();
  
  //Wire.beginRequest();
  //Wire.write(sensoryuzdesi);
  //Wire.write(sicaklik_degeri);
  //Wire.write(voltaj_degeri);
  //Wire.endTransmission();

}

void istekGeldiginde()
{
  /* 
  * Eğer master bu cihazdan veri istiyor ise master cihaza "Merhaba" verisi yollanılır 
  * Eğer bu bir sensör olsaydı "merhaba" yerine sıcaklık veya ivme verisi yollanıyor olacaktı
  */
   Wire.write(sensoryuzdesi); 
   //Wire.write(sicaklik_degeri);
   //Wire.write(voltaj_degeri);
}

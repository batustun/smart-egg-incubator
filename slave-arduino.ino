#include <Wire.h>
#include <Servo.h>

Servo servo_9;
Servo servo_6;
int pos = 0;
//int sensoryuzdesi;
//int sicaklik_degeri;
//int  voltaj_degeri;

/* 
 * I2C fonksiyonlarını kullanabilmek için 
 * Wire.h kütüphanesini projemize ekledik
 */
const int fan_rolesi_pini = 4 ;
boolean fan_durumu = LOW;



void setup()
{
  Wire.begin();
  /* I2C haberleşmesi master olarak başlatıldı */
  pinMode(fan_rolesi_pini, OUTPUT);
  Serial.begin(9600);
  /* Bilgisayara veri yazdırabilmek için seri haberleşme başlatıldı */
  servo_9.attach(9);
  servo_6.attach(6);
  cli();

  // Timer2 kesmesi ayarlanıyor
  TCNT2  = 1;
  TCCR2A = 1;
  TCCR2B = 1;
  OCR2A = 15624;  // 1 saniye çalışması için gerekli zaman kesmesi frekansı
  TCCR2B |= (1 << WGM22);
  TCCR2B |= (1 << CS22) | (1 << CS20);
  TIMSK2 |= (1 << OCIE2A);
  sei();
  
 
}

ISR(TIMER2_COMPA_vect){     //Her kesmeye girildiğinde otomatik çalıştırılacak fonksiyon.
 
 fan_durumu = !fan_durumu;

 digitalWrite(fan_rolesi_pini,fan_durumu);
  
}



void loop()
{
  Wire.beginTransmission(1);
  /* 1 adresine sahip Slave (köle) cihazına veri yollanacağı bildiriliyor */
  
  Wire.endTransmission();
  /* Yollanacak verilerin bittiği bildiriliyor */
 
 
  
  //yumurta haznesinin düzenli bir şekilde dönmesini sağlar
   for (pos = 0; pos <= 180; pos += 1) {
   
    servo_9.write(pos);
    
  }
   delay(1000);
 for (pos = 180; pos >= 0; pos -= 1) {
   
   servo_9.write(pos);
     
  }
  delay(1000);
  Wire.requestFrom(1,0x001);
  //Wire.requestFrom(1,0x001);
 
  /* I2C hattından gelen veriler gelenKarakter değişkenine yazdırılacak */
  
  while(Wire.available()){
    /* I2C hattında yeni veri olduğu sürece döngü devam edecek */
   int sensoryuzdesi = Wire.read();
    //sicaklik_degeri = Wire.read();
    //voltaj_degeri = Wire.read();
 
    /* I2C hattından gelen veriler okunuyor */
    Serial.print(sensoryuzdesi);
    //Serial.print(sicaklik_degeri);
    //Serial.print(voltaj_degeri);
   if(sensoryuzdesi == 0){
    
    servo_9.write(180);
    
    delay(100);
  }
    else
  {
   servo_9.write(0);
  }
   
    /* Gelen veriler ekrana yazdırılıyor */
  }
  Serial.println();
 
 
  
}

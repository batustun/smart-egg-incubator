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
  Wire.begin(1);
  /* I2C haberleşmesi master olarak başlatıldı */
  pinMode(fan_rolesi_pini, OUTPUT);
  Serial.begin(9600);
  Wire.onReceive(veriGeldiginde);
  servo_9.attach(9);
  servo_6.attach(6);
  cli();

 // Timer2 kesmesi ayarlanıyor
  TCNT0  = 0;
  TCCR0A = 0;
  TCCR0B = 0;
  OCR0A = 15624;  // 1 saniye çalışması için gerekli zaman kesmesi frekansı
  TCCR0B |= (0 << WGM02);
  TCCR0B |= (0 << CS02) | (0 << CS20);
  TIMSK0 |= (0 << OCIE0A);
  sei();  
  
 
}

ISR(TIMER0_COMPA_vect){     //Her kesmeye girildiğinde otomatik çalıştırılacak fonksiyon.
 
 fan_durumu = !fan_durumu;

 digitalWrite(fan_rolesi_pini,fan_durumu);
  
}



void loop()
{
 
 
 
  
  //yumurta haznesinin düzenli bir şekilde dönmesini sağlar
  for (pos = 0; pos <= 180; pos += 1) {
   
    servo_6.write(pos);   
   
  }
 
    delay(1000);
  
 for (pos = 180; pos >= 0; pos -= 1) {
   
   servo_6.write(pos);
  
  }
  delay(100);
  
  
  
  
    /* Gelen veriler ekrana yazdırılıyor */
  }


   void veriGeldiginde(int veri)
   {
  
  int sensoryuzdesi = Wire.read();
  Serial.println(sensoryuzdesi);
  int sicaklik_degeri = Wire.read();
  Serial.println(sicaklik_degeri);
  int voltaj_degeri = Wire.read();
  Serial.println(voltaj_degeri);
     
if(sensoryuzdesi == 0 && sicaklik_degeri== 0 && voltaj_degeri== 0 ){
    
    servo_9.write(180);
    
  
  }
    else
  {
   servo_9.write(0);
  }
 
     
}
  

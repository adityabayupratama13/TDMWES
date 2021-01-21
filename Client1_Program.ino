//Program Arduino Client 1 (LM35)

#include <SPI.h>                        //Library arduino untuk mengatifkan atau agar dapat menggunakan komunikasi Serial Periperal Interface (SPI)
#include <Ethernet.h>                   //Library arduino untuk mengatifkan atau agar dapat menggunakan Ethernet Shield
#include <EthernetUdp.h>                //Library arduino untuk mengatifkan atau agar dapat berkomunikasi ethernet menggunakan udp
#define LM35 A0

char thisChar;
int LM35Val, LM35Bit;

                                        //Mac Address yang digunakan masing-masing device harus unik
byte mac[] = {                          //Mac Address Arduino
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01
};

                                        //IP Address yang digunakan
                                        //IP Address harus berbeda setiap device yang terkoneksi dalam jaringan yang sama
IPAddress ip(192, 168, 1, 178);         //IP Address Arduino Pengirim Data

EthernetServer server(80);              //Membuka akses port 80 untuk protokol TCP

EthernetUDP Udp;                        //Menginisialisasi atau membuat variabel Udp sebagai komunikasi UDP
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];          //Variabel untuk menerima data

void setup(){                           //Program Arduino untuk melakukan setup pada pin dan berbagai hal yang diinginkan seperti baudrated  
  Ethernet.begin(mac, ip);              //Inisialisasi Ethernet dengan MAC Address dan IP Address yang didefinisikan diatas
                                        //Membuka koneksi dan menunggu koneksi dari client
  server.begin();                       //Arduino membuka koneksi port 9000 sebagai port untuk masuknya data dalam komunikasi ethernet TCP
  Serial.begin(9600);                   //Serial akan menggunakan baudrate 9600 dalam berkomunikasi serial (Berkomunikasi dengan Komputer)
  
  Udp.begin(9000);                      //Arduino membuka koneksi port 9000 sebagai port untuk masuknya data dalam komunikasi ethernet UDP

  pinMode(LM35, INPUT);
}

void loop(){                            //Program Arduino yang digunakan untuk berbagai keadaan yang ingin kita ulangi prosesnya

  LM35Val=analogRead(LM35);
  LM35Bit = LM35Val/2.0479;
  Serial.print("Nilai Suhu: ");
  Serial.print(LM35Bit);
  Serial.println(" Celcius");
  
  int packetSize = Udp.parsePacket();               //Menginisialisasi variabel packetSize agar memiliki nilai yang sama dengan Udp.persePacket()
  Serial.println(packetSize);
  if(packetSize){                                   //Jika packetSize sama nilainya dengan Udp.persePacket maka kondisi if dikerjakan
    Serial.print("Received packet of size ");       //Kalimat "Received packet of size " akan dikirimkan ke Laptop atau Komputer (dapat dilihat di Serial Monitor)
    Serial.println(packetSize);                     //Data varibel packetSize akan dikirimkan ke Laptop atau Komputer (dapat dilihat di Serial Monitor)
    Serial.print("From IP Address ");               //Kalimat "From IP Address" akan dikirimkan ke Laptop atau Komputer (dapat dilihat di Serial Monitor)
    IPAddress remote = Udp.remoteIP();              //Membaca IP yang mengirim data
    for (int i = 0; i < 4; i++){                    //Pengulangan for akan dialakukan jika nilai i lebih kecil dari 4 dan i diincrementkan
      Serial.print(remote[i], DEC);                 //Alamat IP pengirim data akan dikirimkan ke Laptop atau Komputer (dapat dilihat di Serial Monitor)
      if (i < 3){                                   //Jika i lebih kecil dari 3 maka kondisi if dikerjakan
        Serial.print(".");                          //Kalimat "." akan dikirimkan ke Laptop atau Komputer (dapat dilihat di Serial Monitor)
      }
    }
    
    Serial.print(" port ");                         //Kalimat "Port" akan dikirimkan ke Laptop atau Komputer (dapat dilihat di Serial Monitor)
    Serial.println(Udp.remotePort());               //Port yang digunakan dalam komunikasi akan dikirimkan ke Laptop atau Komputer (dapat dilihat di Serial Monitor)
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE); //Menerima data dari UDP
    Serial.print("Data Server: ");                  //Kalimat "Data: " akan dikirimkan ke Laptop atau Komputer (dapat dilihat di Serial Monitor)
    Serial.println(packetBuffer);                   //Data varibel packetBuffer akan dikirimkan ke Laptop atau Komputer (dapat dilihat di Serial Monitor)

    if (packetSize == 1){
      Udp.beginPacket("192.168.1.177",9000);
      Udp.print(LM35Bit);                   
      Udp.endPacket();                            //Mengakhiri pengiriman data Udp.beginPacket("192.168.1.177",9000); //Arduino mengirim data ke IP Address tujuan dan port tujuan
      delay(10);
    }
  }

  delay(1000);
}


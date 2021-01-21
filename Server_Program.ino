//Program Arduino Server

#include <SPI.h>                        //Library arduino untuk mengatifkan atau agar dapat menggunakan komunikasi Serial Periperal Interface (SPI)
#include <Ethernet.h>                   //Library arduino untuk mengatifkan atau agar dapat menggunakan Ethernet Shield
#include <EthernetUdp.h>                //Library arduino untuk mengatifkan atau agar dapat berkomunikasi ethernet menggunakan udp
#define BBroadcast 5
#define BRTemp 6
#define BRDist 7
#define BReset 8

String string;
char command;
                                        //Mac Address yang digunakan masing-masing device harus unik
byte mac[] = {                          //Mac Address Arduino
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

                                        //IP Address yang digunakan
                                        //IP Address harus berbeda setiap device yang terkoneksi dalam jaringan yang sama
IPAddress ip(192, 168, 1, 177);         //IP Address Arduino Pengirim Data

EthernetServer server(80);              //Membuka akses port 80 untuk protokol TCP

EthernetUDP Udp;                                    //Menginisialisasi atau membuat variabel Udp sebagai komunikasi UDP

char packetBuffer0[UDP_TX_PACKET_MAX_SIZE];          //Variabel untuk menerima data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];           //Variabel untuk menerima data
char packetBuffer1[UDP_TX_PACKET_MAX_SIZE];          //Variabel untuk menerima data
char packetBuffer3[UDP_TX_PACKET_MAX_SIZE];          //Variabel untuk menerima data

void setup(){                           //Program Arduino untuk melakukan setup pada pin dan berbagai hal yang diinginkan seperti baudrated      
  Ethernet.begin(mac, ip);              //Inisialisasi Ethernet dengan MAC Address dan IP Address yang didefinisikan diatas
                                        //Membuka koneksi dan menunggu koneksi dari client
  server.begin();                       //Arduino membuka koneksi port 9000 sebagai port untuk masuknya data dalam komunikasi ethernet TCP
  
  Udp.begin(9000);                                  //Arduino membuka koneksi port 9000 sebagai port untuk masuknya data dalam komunikasi ethernet UDP 
  Serial.begin(9600);                               //Serial akan menggunakan baudrate 9600 dalam berkomunikasi serial (Berkomunikasi dengan Komputer)

  pinMode(BRTemp, INPUT);
  pinMode(BRDist, INPUT);
  pinMode(BBroadcast, INPUT);
  pinMode(BReset, INPUT);
  
}

void loop(){                            //Program Arduino yang digunakan untuk berbagai keadaan yang ingin kita ulangi prosesnya
  
  int BRTempVal = 0;
  BRTempVal = digitalRead(BRTemp);

  int BRDistVal = 0;
  BRDistVal = digitalRead(BRDist);
  
  int BBroadcastVal = 0;
  BBroadcastVal = digitalRead(BBroadcast);

  server.print(BBroadcastVal);

  int BResetVal = 0;
  BResetVal = digitalRead(BReset);
  
  if (Serial.available() > 0) 
    {string = "";}
    
    while(Serial.available() > 0)
    {
      command = ((byte)Serial.read());
      if(command == ':')
      {
        break;
      }
      
      else
      {
        string += command;
      }
      
      delay(1);
    }
    
    
    if(string == "BC" || BBroadcastVal == 1)
    {
     
      Udp.beginPacket("192.168.1.179",9000);
      Udp.print(1);                         //Mengirimkan data acak ke ip yang dituju
      Udp.endPacket();                      //Mengakhiri pengiriman data Udp.beginPacket("192.168.1.179",9000); //Arduino mengirim data ke IP Address tujuan dan port tujuan

      //Mengirim data ke client yang telah terkoneksi
      server.print(BBroadcastVal); 

      int packetSize = Udp.parsePacket();               //Menginisialisasi variabel packetSize agar memiliki nilai yang sama dengan Udp.persePacket()
      if(packetSize){                                   //Jika packetSize sama nilainya dengan Udp.persePacket maka kondisi if dikerjakan
        IPAddress remote = Udp.remoteIP();              //Membaca IP yang mengirim data
        
        for (int i = 0; i < 4; i++){                    //Pengulangan for akan dialakukan jika nilai i lebih kecil dari 4 dan i diincrementkan
          //Serial.print(remote[i], DEC);               //Alamat IP pengirim data akan dikirimkan ke Laptop atau Komputer (dapat dilihat di Serial Monitor)
          if (i < 3){                                   //Jika i lebih kecil dari 3 maka kondisi if dikerjakan
            //Serial.print(".");                        //Kalimat "." akan dikirimkan ke Laptop atau Komputer (dapat dilihat di Serial Monitor)
          }
        }
    
        memset(packetBuffer0,0,sizeof(packetBuffer0));
        
        Udp.read(packetBuffer0, UDP_TX_PACKET_MAX_SIZE); //Menerima data dari UDP
        
      }
      
      Udp.beginPacket("192.168.1.178",9000);
      Udp.print(1);
      Udp.endPacket(); 

      server.print(BBroadcastVal);  
      
      packetSize = Udp.parsePacket();                   //Menginisialisasi variabel packetSize agar memiliki nilai yang sama dengan Udp.persePacket()
      if(packetSize){                                   //Jika packetSize sama nilainya dengan Udp.persePacket maka kondisi if dikerjakan
        IPAddress remote = Udp.remoteIP();              //Membaca IP yang mengirim data
        
        for (int i = 0; i < 4; i++){                    //Pengulangan for akan dialakukan jika nilai i lebih kecil dari 4 dan i diincrementkan
          //Serial.print(remote[i], DEC);               //Alamat IP pengirim data akan dikirimkan ke Laptop atau Komputer (dapat dilihat di Serial Monitor)
          if (i < 3){                                   //Jika i lebih kecil dari 3 maka kondisi if dikerjakan
            //Serial.print(".");                        //Kalimat "." akan dikirimkan ke Laptop atau Komputer (dapat dilihat di Serial Monitor)
          }
        }
    
        memset(packetBuffer3,0,sizeof(packetBuffer3));
        Udp.read(packetBuffer3, UDP_TX_PACKET_MAX_SIZE); //Menerima data dari UDP
      
      }

      delay(1000);
      
      if(strlen(packetBuffer3) == 0)
      {
        Serial.print(0);                   //Data varibel packetBuffer akan dikirimkan ke Laptop atau Komputer (dapat dilihat di Serial Monitor)
      }
      
      else
      {
        Serial.print(packetBuffer3);  
      }
      
      Serial.print(",");                   //Kalimat "," akan dikirimkan ke Laptop atau Komputer (dapat dilihat di Serial Monitor)
      if(strlen(packetBuffer0) == 0)
      {
        Serial.print(0);                   //Data varibel packetBuffer akan dikirimkan ke Laptop atau Komputer (dapat dilihat di Serial Monitor)
      }
      
      else
      {
        Serial.println(packetBuffer0);  
      }
    }
    
    else if(string =="RT" || BRTempVal == 1)
    {
      Udp.beginPacket("192.168.1.178",9000);
      Udp.print(1);                         //Mengirimkan data acak ke ip yang dituju
      Udp.endPacket();                      //Mengakhiri pengiriman data Udp.beginPacket("192.168.1.178",9000); //Arduino mengirim data ke IP Address tujuan dan port tujuan
      delay(1000);

      int packetSize = Udp.parsePacket();               //Menginisialisasi variabel packetSize agar memiliki nilai yang sama dengan Udp.persePacket()
   
      if(packetSize){                                   //Jika packetSize sama nilainya dengan Udp.persePacket maka kondisi if dikerjakan
        IPAddress remote = Udp.remoteIP();              //Membaca IP yang mengirim data
        for (int i = 0; i < 4; i++){                    //Pengulangan for akan dialakukan jika nilai i lebih kecil dari 4 dan i diincrementkan
          //Serial.print(remote[i], DEC);               //Alamat IP pengirim data akan dikirimkan ke Laptop atau Komputer (dapat dilihat di Serial Monitor)
          if (i < 3){                                   //Jika i lebih kecil dari 3 maka kondisi if dikerjakan
            //Serial.print(".");                        //Kalimat "." akan dikirimkan ke Laptop atau Komputer (dapat dilihat di Serial Monitor)
          }
        }
    
        memset(packetBuffer,0,sizeof(packetBuffer));
        Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE); //Menerima data dari UDP
        
        String ValTemp = String(packetBuffer);
        Serial.print(ValTemp);                   //Data varibel packetBuffer akan dikirimkan ke Laptop atau Komputer (dapat dilihat di Serial Monitor)
        Serial.print(",0");
      }
    } 

    else if(string == "RD" || BRDistVal == 1)
    {
      Udp.beginPacket("192.168.1.179",9000);
      Udp.print(1);                         //Mengirimkan data acak ke ip yang dituju
      Udp.endPacket();                      //Mengakhiri pengiriman data Udp.beginPacket("192.168.1.179",9000); //Arduino mengirim data ke IP Address tujuan dan port tujuan
      delay(1000);

      int packetSize = Udp.parsePacket();               //Menginisialisasi variabel packetSize agar memiliki nilai yang sama dengan Udp.persePacket()
     
      if(packetSize){                                   //Jika packetSize sama nilainya dengan Udp.persePacket maka kondisi if dikerjakan
        IPAddress remote = Udp.remoteIP();              //Membaca IP yang mengirim data
        for (int i = 0; i < 4; i++){                    //Pengulangan for akan dialakukan jika nilai i lebih kecil dari 4 dan i diincrementkan
          //Serial.print(remote[i], DEC);               //Alamat IP pengirim data akan dikirimkan ke Laptop atau Komputer (dapat dilihat di Serial Monitor)
          if (i < 3){                                   //Jika i lebih kecil dari 3 maka kondisi if dikerjakan
            //Serial.print(".");                        //Kalimat "." akan dikirimkan ke Laptop atau Komputer (dapat dilihat di Serial Monitor)
          }
        }
   
        memset(packetBuffer1,0,sizeof(packetBuffer1));
        Udp.read(packetBuffer1, UDP_TX_PACKET_MAX_SIZE); //Menerima data dari UDP
       
        String ValDist = String(packetBuffer1);
        Serial.print("0,");
        Serial.print(ValDist);                   //Data varibel packetBuffer akan dikirimkan ke Laptop atau Komputer (dapat dilihat di Serial Monitor)

      }
    }

    else if(BResetVal == 1)
    {
      string = "RS";
      Serial.print("0,0");
    }
    
  

  delay (1000);
}


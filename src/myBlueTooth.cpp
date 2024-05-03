#include "myBlueTooth.h"
#include "fileSystem.h"
#include "SimpleCLI.h"

extern LittleFileSystem lsFile;
extern SimpleCLI simpleCli;

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;
void MyServerCallbacks::onConnect(BLEServer* pServer) {
      deviceConnected = true;
};

void  MyServerCallbacks::onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
}
bool btDataReceived=false;
String btReceiveString="";
void MyCallbacks::onWrite(BLECharacteristic *pCharacteristic)
{
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0)
    {
        //Serial.println("*********");
        //Serial.print("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++){
            btReceiveString +=rxValue[i];
            if(rxValue[i] == '\r' || rxValue[i]== '\n'){
                btDataReceived =true;
            }
            if(btReceiveString.length()>30 )
                btDataReceived =true;
            //Serial.print(rxValue[i]);
        }
        // Serial.println();
        // Serial.printf("\n-->%s",btReceiveString.c_str());
    }
}

myBlueToothStream::myBlueToothStream(void){};
myBlueToothStream::~myBlueToothStream(void){};
size_t myBlueToothStream::write(uint8_t c){
 return write(&c, 1);
};
size_t myBlueToothStream::write(const uint8_t *buffer, size_t size){
    pTxCharacteristic->setValue((uint8_t *)buffer, size);
    return size; 
};       
int myBlueToothStream::available(void)
{
    return btDataReceived; 
    //return btReceiveString.length();
}
int myBlueToothStream::read()
{
    uint8_t c = 0;
    if(btReceiveString.length()){
        c = btReceiveString.charAt(0);
        btReceiveString.remove(0);
        if(btReceiveString.length() ==0 )  
            btDataReceived   = 0;
        return c;
    }
    return -1;
}
int myBlueToothStream::peek(void)
{
    uint8_t c;
    if(btReceiveString.length()){
        c = btReceiveString.charAt(0);
        return c;
    }
    return -1;
}

String myBlueToothStream::readString(){
    String retString = btReceiveString;
    btReceiveString="";
    btDataReceived=0;
    return retString ;
};
size_t myBlueToothStream::printf(const char *format, ...)
{
    char loc_buf[64];
    char * temp = loc_buf;
    va_list arg;
    va_list copy;
    va_start(arg, format);
    va_copy(copy, arg);
    int len = vsnprintf(temp, sizeof(loc_buf), format, copy);
    va_end(copy);
    if(len < 0) {
        va_end(arg);
        return 0;
    }
    if(len >= (int)sizeof(loc_buf)){  // comparation of same sign type for the compiler
        temp = (char*) malloc(len+1);
        if(temp == NULL) {
            va_end(arg);
            return 0;
        }
        len = vsnprintf(temp, len+1, format, arg);
    }
    va_end(arg);
    len = write((uint8_t*)temp, len);
    pTxCharacteristic->notify(true);
    if(temp != loc_buf){
        free(temp);
    }
    return len;
}

void bleSetup(){
 BLEDevice::init("UPS1P1P_IFTECH");
 //lsFile.littleFsInit(1);// 
 lsFile.littleFsInitFast(0);
 lsFile.setOutputStream(&Serial);

 simpleCli.outputStream = &Serial;
  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
										CHARACTERISTIC_UUID_TX,
										BLECharacteristic::PROPERTY_NOTIFY
									);
                      
  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
											 CHARACTERISTIC_UUID_RX,
											BLECharacteristic::PROPERTY_WRITE
										);

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}
myBlueToothStream SerialBT;
void bleCheck()
{
  String cmd;
  if (deviceConnected)
  {
    if(SerialBT.available()){
        cmd = SerialBT.readString().c_str();
        simpleCli.parse(cmd );
        Serial.printf("*** %s",cmd.c_str());
        SerialBT.printf("\nHello %s",cmd.c_str());
        SerialBT.printf("\nThis is a ");
        SerialBT.printf("\n wonderful land.....!!! ");
    }
    //pTxCharacteristic->setValue(&txValue, 1);
    // pTxCharacteristic->notify();
    // txValue++;
    // delay(10); // bluetooth stack will go into congestion, if too many packets are sent
  }


  // disconnecting
  if (!deviceConnected && oldDeviceConnected)
  {
    delay(500);                  // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
    // lsFile.setOutputStream(&Serial);
    // simpleCli.outputStream = &Serial;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected)
  {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
    // lsFile.setOutputStream(&SerialBT);
    // simpleCli.outputStream = &SerialBT;
  }
}
#include "myBlueTooth.h"
#include "fileSystem.h"
#include "SimpleCLI.h"
#include "WiFi.h"
#include "esp_gatt_common_api.h"
#include "esp_task_wdt.h"

extern LittleFileSystem lsFile;
extern SimpleCLI simpleCli;

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

myBlueToothStream mySerialBT;

void MyServerCallbacks::onConnect(BLEServer* pServer) {
      deviceConnected = true;
      Serial.println("BLE client connected");
};

void  MyServerCallbacks::onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      Serial.println("BLE client disconnected");
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
    if (pTxCharacteristic == nullptr) {
        return Serial.write(buffer, size);
    }
    const size_t chunk = 160;
    size_t sent = 0;
    while (sent < size) {
        size_t n = size - sent;
        if (n > chunk)
            n = chunk;
        pTxCharacteristic->setValue((uint8_t *)buffer + sent, n);
        pTxCharacteristic->notify(true);
        sent += n;
        if (sent < size)
            delay(8);
    }
    return sent;
}

size_t myBlueToothStream::writePaced(const uint8_t *buffer, size_t size)
{
    if (pTxCharacteristic == nullptr || pServer == nullptr)
        return write(buffer, size);

    size_t sent = 0;
    while (sent < size) {
        auto peers = pServer->getPeerDevices(false);
        if (peers.empty())
            return sent;
        uint16_t connId = peers.begin()->first;
        uint16_t mtu = pServer->getPeerMTU(connId);
        size_t chunk = 20;
        if (mtu > 23)
            chunk = (size_t)mtu - 3;
        if (chunk > 500)
            chunk = 500;

        size_t n = size - sent;
        if (n > chunk)
            n = chunk;

        bool sentThis = false;
        for (int attempt = 0; attempt < 30 && !sentThis; attempt++) {
            uint16_t room = esp_ble_get_cur_sendable_packets_num(connId);
            if (room == 0 && attempt < 20) {
                vTaskDelay(pdMS_TO_TICKS(8));
                (void)esp_task_wdt_reset();
                continue;
            }
            pTxCharacteristic->setValue((uint8_t *)buffer + sent, n);
            pTxCharacteristic->notify(true);
            sentThis = true;
            /* 버퍼가 남아 있으면 컨트롤러가 비우는 속도에 맞춘다. */
            vTaskDelay(pdMS_TO_TICKS(room > 2 ? 1 : 6));
            (void)esp_task_wdt_reset();
        }
        if (!sentThis)
            return sent;
        sent += n;
    }
    return sent;
}       
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
    if(temp != loc_buf){
        free(temp);
    }
    return len;
}
void bleSetup(){
  String bleName = "IFT_43_" + WiFi.macAddress();
  bleName.replace(":", "");
  BLEDevice::init(bleName.c_str());
  BLEDevice::setMTU(517);
  Serial.printf("BLE name %s\n", bleName.c_str());

  simpleCli.outputStream = &Serial;
  simpleCli.inputStream = &Serial;
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

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

  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void bleCheck()
{
  String cmd;
  if (deviceConnected)
  {
    mySerialBT.deviceConnected=true;
    if(mySerialBT.available()){
        lsFile.setOutputStream(&mySerialBT);
        simpleCli.outputStream = &mySerialBT;
        cmd = mySerialBT.readString().c_str();
        simpleCli.parse(cmd );
        mySerialBT.printf("*** %s",cmd.c_str());
        // mySerialBT.printf("\nHello %s",cmd.c_str());
        // mySerialBT.printf("\nThis is a ");
        // mySerialBT.printf("\n wonderful land.....!!! ");
    }
    //pTxCharacteristic->setValue(&txValue, 1);
    // pTxCharacteristic->notify();
    // txValue++;
    // delay(10); // bluetooth stack will go into congestion, if too many packets are sent
  }
  else 
    mySerialBT.deviceConnected=false;


  // disconnecting
  if (!deviceConnected && oldDeviceConnected)
  {
    delay(500);                  // give the bluetooth stack the chance to get things ready
    pServer->getAdvertising()->start(); // restart advertising
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
    // lsFile.setOutputStream(&mySerialBT);
    // simpleCli.outputStream = &mySerialBT;
  }
}
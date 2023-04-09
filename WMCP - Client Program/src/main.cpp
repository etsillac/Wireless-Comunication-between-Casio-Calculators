#include <SoftwareSerial.h>

#define rxPin 10
#define txPin 11

SoftwareSerial serialCalc =  SoftwareSerial(rxPin, txPin);

typedef struct Message {
    int userIndex;
    char message[109];
} Message;

char groupNames[][11] = {
  "Test #1",
  "It's piiie",
  "So cool!",
  "Les nuls",
  "Group #01",
  "Enfin ptn!"
};
char userNames[][3] = {
  "CG", "HC", "LB"
};

int currentGroup = -1;

Message new_messages[60];
int n = 60;
int front = -1, rear = -1;

void inqueue(Message message) {
  if (rear == n - 1) Serial.println("New messages queue overflowing");
  else {
    if (front == -1) front = 0;
    rear++;
    new_messages[rear] = message;
  }
}
Message* dequeue() {
  if (front == -1 || front > rear) { Serial.println("New messages queue underflowing"); return; }
  else {
    front++;
    return &new_messages[front-1];
  }
}

unsigned int checkSum(char* data) {
    int i;
    unsigned int checksumResult, sumResult;

    for (i = 0; i < 128; i++) {
        sumResult = checksumResult + data[i];
        checksumResult = ((unsigned char*)(&sumResult))[0] + ((unsigned char*)(&sumResult))[1];
    }
}

void sendValue(int value) {
  Serial.println("Arduino transmitting:");
  serialCalc.write(0xF0);

  char dataSent[128];
  memset(dataSent, 0, 128);
  dataSent[0] = value;
  serialCalc.write(strlen(dataSent));
  Serial.print("  Byte count: ");
  Serial.println(strlen(dataSent));
  
  for (int i = 0; i < 128; i++) {
    serialCalc.write(dataSent[i]);
  }
  Serial.print("  Data: ");
  Serial.println(dataSent[0], DEC);
  
  serialCalc.write(checkSum(dataSent));
  Serial.print("  Checksum: ");
  Serial.println(checkSum(dataSent));
}
void sendText(char* dataToSend) {
  Serial.println("Arduino transmitting:");
  serialCalc.write(0xF0);

  char dataSent[128];
  memset(dataSent, 0, 128);
  strncpy(dataSent, dataToSend, 128);
  serialCalc.write(strlen(dataSent));
  Serial.print("  Byte count: ");
  Serial.println(strlen(dataSent));
  
  for (int i = 0; i < 128; i++) {
    serialCalc.write(dataSent[i]);
  }
  Serial.print("  Data: ");
  Serial.println(dataSent);
  
  serialCalc.write(checkSum(dataSent));
  Serial.print("  Checksum: ");
  Serial.println(checkSum(dataSent));
}
void sendMessage(Message message) {
  Serial.println("Arduino transmitting:");
  serialCalc.write(0xF0);

  char dataSent[128];
  memset(dataSent, 0, 128);
  strncpy(dataSent, "New message: ", 128);
  strcat(dataSent, (const char*)message.userIndex);
  strcat(dataSent, message.message);
  serialCalc.write(strlen(dataSent));
  Serial.print("  Byte count: ");
  Serial.println(strlen(dataSent));
  
  for (int i = 0; i < 128; i++) {
    serialCalc.write(dataSent[i]);
  }
  Serial.print("  Data: ");
  Serial.println(dataSent);
  
  serialCalc.write(checkSum(dataSent));
  Serial.print("  Checksum: ");
  Serial.println(checkSum(dataSent));
}

void setup() {
  Serial.begin(9600);
  serialCalc.begin(9600);

  Message message1;
  message1.userIndex = 0;
  strcpy(message1.message, "Salut les gars, ca va ??");
  inqueue(message1);
  
  Message message2;
  message2.userIndex = 0;
  strcpy(message2.message, "Moi au top si vous demandez");
  inqueue(message2);

  Message message3;
  message3.userIndex = 2;
  strcpy(message3.message, "super bien de mon coté");
  inqueue(message3);

  Message message4;
  message4.userIndex = 1;
  strcpy(message4.message, "moi aussi !!");
  inqueue(message4);
}

void loop() {
  if(serialCalc.available()) {
    if (serialCalc.read() != 0xF0) return;
    Serial.println("Casio transmitting:");
    char byteCount = serialCalc.read();
    Serial.print("  Byte count: ");
    Serial.println(byteCount, DEC);

    char dataRead[128];
    serialCalc.readBytes(dataRead, 128);
    Serial.print("  Data: ");
    Serial.println(dataRead);

    char checksum = serialCalc.read();
    Serial.print("  Checksum (send, checked): ");
    Serial.print(checksum, DEC);
    Serial.print(", ");
    Serial.println(checkSum(dataRead), DEC);

    if (strcmp(dataRead, "WMCP Casio - initialisation") == 0) sendText("WMCP Transmition Module - ready");
    else if (strcmp(dataRead, "WMCP Casio - get group number") == 0) sendValue(sizeof(groupNames) / 11);
    else if (strncmp(dataRead, "WMCP Casio - get group name of index ", 37) == 0) {      
      int index = dataRead[37] - '0';
      sendText(groupNames[index]);
    }
    else if (strncmp(dataRead, "WMCP Casio - connect to group of index ", 39) == 0) {
      int index = dataRead[39] - '0';
      currentGroup = index;

      sendText(groupNames[index]);
    }
    else if (strcmp(dataRead, "WMCP Casio - get number of user") == 0) {
      if (currentGroup != -1) { sendValue(sizeof(userNames) / 3); }
      else { sendText("No group connected to"); }
    }
    else if (strncmp(dataRead, "WMCP Casio - get user name of index ", 36) == 0) {
      int userIndex = dataRead[36] - '0';

      if (currentGroup != -1) { sendText(userNames[userIndex]); }
      else { sendText("No group connected to"); }
    }
    else if (strcmp(dataRead, "WMCP Casio - check for new message") == 0) {
      if (currentGroup != -1) {
        Message* messageToSend = dequeue();
        if (messageToSend != nullptr) sendMessage(*messageToSend);
      }
      else sendText("No group connected to");
    }
  }
}
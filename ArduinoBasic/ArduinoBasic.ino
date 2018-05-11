typedef struct Token {
  byte type;
  char value[6];
} token;

#define MAX_LENGTH 20

char input[MAX_LENGTH];
char result[MAX_LENGTH];
Token tokens[MAX_LENGTH];
String lineString;

void setup() {
  Serial.begin(9600);
  Serial.println("ready");
}

void loop() {

  memset(input, NULL, MAX_LENGTH);
  memset(result, NULL, MAX_LENGTH);
  memset(tokens, NULL, MAX_LENGTH*sizeof(Token));

  if(Serial.available()){
    readIn();
    parseCommand();
  }
}

void sendOut(){
  Serial.println(result);
}

void readIn(){
  Serial.readString().toCharArray(input, MAX_LENGTH);
}

void parseCommand(){
  tokenizer();

}

void tokenizer(){

  byte iteration = 0;
  byte stringifying = 0;
  
  char buf[5];
  byte prev;
  byte holder;

  memcpy(buf, NULL, 6);
  lineString = "";

  for(byte i; i<MAX_LENGTH && input[i]!=NULL; i++){

    byte entry = 0;

    memcpy(buf, NULL, 6);

    holder = typeOf(input[i]);
    if(!i) prev = typeOf(input[i]);

    while((holder == prev || prev == B100 || stringifying) && input[i]!=NULL){
      if(holder == B100){
        stringifying = !stringifying;
      }
      else if(!stringifying){
        buf[entry++] = input[i];
        buf[entry] = NULL;
      }
      if(stringifying && holder != B100) lineString += input[i];
      prev = holder;
      holder = typeOf(input[++i]);
    }

    if(prev != B100 && buf[0] != NULL){
      Token temp;
      temp.type = prev;
      strcpy(temp.value, buf);

      Serial.println(temp.type);
      
      tokens[iteration] = temp;
    }
    else if(stringifying){
      Serial.println("Bad Syntax");
    }
    else{
      Serial.println("String: "+lineString);
    }

    prev = holder;
    iteration++;
    i--;
  }

  int i = 0;

  while(i<MAX_LENGTH){
    strcpy(result, tokens[i++].value);
    sendOut();
  }
  
}

byte typeOf(char val){
  (byte) val;

  /*Token types:
   * speciall = 000b
   * op = 001b
   * chr = 010b
   * num = 011b
   * str = 100b
   */

  if(val <= 0x20){return B00;}
  else if(val == 0x22){return B100;}
  else if(val <= 0x2F){return B01;}
  else if(val <= 0x39){return B11;}
  else if(val <= 0x40){return B01;}
  else if(val <= 0x5A){return B10;}
  else if(val <= 0x60){return B01;}
  else if(val <= 0x7A){return B10;}
  
  return B01;

}



typedef struct Token {
  byte type;
  char value[5];
} token;

#define MAX_LENGTH 20

char input[MAX_LENGTH];
char result[MAX_LENGTH];
Token tokens[MAX_LENGTH];

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
  
  char buf[5];
  byte prev;
  byte holder;

  for(byte i; i<MAX_LENGTH && input[i]!=NULL; i++){

    memcpy(buf, NULL, 5);

    holder = typeOf(input[i]);
    if(!i) prev = typeOf(input[i]);

    while(holder == prev && input[i]!=NULL){
      buf[i-(iteration*i)] = input[i];
      prev = holder;
      holder = typeOf(input[++i]);
    }
    
    Token temp;
    temp.type = prev;
    strcpy(temp.value, buf);
    tokens[iteration++] = temp;

    prev = holder;
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
   * speciall = 00b
   * op = 01b
   * chr = 10b
   * num = 11b
   */

  if(val <= 0x20){return B00;}
  else if(val <= 0x2F){return B01;}
  else if(val <= 0x39){return B11;}
  else if(val <= 0x40){return B01;}
  else if(val <= 0x5A){return B10;}
  else if(val <= 0x60){return B01;}
  else if(val <= 0x7A){return B10;}
  
  return B01;

}



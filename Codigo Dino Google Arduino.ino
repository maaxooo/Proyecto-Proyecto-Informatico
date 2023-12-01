
#include <LiquidCrystal.h>

#define but 2
#define ap 1
#define rw 10
#define ptc 12

#define sr1 1
#define sr2 2
#define sj 3
#define sju '.'      
#define sjl 4
#define ste ' '      
#define sts 5
#define stsr 6
#define stsl 7

#define hhp 1    

#define tw 16
#define te 0
#define tlb 1
#define tub 2

#define hpo 0       
#define hprl1 1  
#define hprl2 2  

#define hpj1 3   
#define hpj2 4   
#define hpj3 5   
#define hpj4 6       
#define hpj5 7       
#define hpj6 8       
#define hpj7 9       
#define hpj8 10      

#define hpru1 11 
#define hpru2 12 

const int rs = 10, en = 9, d4 = 8, d5 = 7, d6 = 6, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

static char tupp[tw + 1];
static char tlw[tw + 1];
static bool bpsh = false;


static byte graphics[] = {
    0b00000,
  0b00111,
  0b00111,
  0b10110,
  0b11111,
  0b01010,
  0b01010,
  0b00000,
    
    0b00000,
  0b00111,
  0b00111,
  0b10110,
  0b11111,
  0b01010,
  0b01010,
  0b00000,
    
    0b00000,
  0b00111,
  0b00111,
  0b10110,
  0b11111,
  0b01010,
  0b01010,
  0b00000,
    
    0b00000,
  0b00111,
  0b00111,
  0b10110,
  0b11111,
  0b01010,
  0b01010,
  0b00000,
    
    0b00100,
  0b00101,
  0b10101,
  0b10101,
  0b10111,
  0b11100,
  0b00100,
  0b00000,
    
   0b00100,
  0b00101,
  0b10101,
  0b10101,
  0b10111,
  0b11100,
  0b00100,
  0b00000,
    
    0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  };



void intGraphics(){

  
  int i;
  for (i = 0; i < 7; ++i) {
    lcd.createChar(i + 1, &graphics[i * 8]);
  }
  for (i = 0; i < tw; ++i) {
    tupp[i] = ste;
    tlw[i] = ste;
  }
}


void advterrain(char* terrain, byte nterr){
  for (int i = 0; i < tw; ++i) {
    char current = terrain[i];
    char next = (i == tw-1) ? nterr : terrain[i+1];
    switch (current){
      case ste:
        terrain[i] = (next == sts) ? stsr : ste;
        break;
      case sts:
        terrain[i] = (next == ste) ? stsl : sts;
        break;
      case stsr:
        terrain[i] = sts;
        break;
      case stsl:
        terrain[i] = ste;
        break;
    }
  }
}

bool drawDino(byte position, char* tupp, char* tlw, unsigned int score) {
  bool col = false;
  char us = tupp[hhp];
  char ls = tlw[hhp];
  byte upper, lower;
  switch (position) {
    case hpo:
      upper = lower = ste;
      break;
    case hprl1:
      upper = ste;
      lower = sr1;
      break;
    case hprl2:
      upper = ste;
      lower = sr2;
      break;
    case hpj1:
    case hpj8:
      upper = ste;
      lower = sj;
      break;
    case hpj2:
    case hpj7:
      upper = sju;
      lower = sjl;
      break;
    case hpj3:
    case hpj4:
    case hpj5:
    case hpj6:
      upper = sj;
      lower = ste;
      break;
    case hpru1:
      upper = sr1;
      lower = ste;
      break;
    case hpru2:
      upper = sr2;
      lower = ste;
      break;
  }
  if (upper != ' ') {
    tupp[hhp] = upper;
    col = (us == ste) ? false : true;
  }
  if (lower != ' ') {
    tlw[hhp] = lower;
    col |= (ls == ste) ? false : true;
  }
  
  byte digits = (score > 9999) ? 5 : (score > 999) ? 4 : (score > 99) ? 3 : (score > 9) ? 2 : 1;
  
  tupp[tw] = '\0';
  tlw[tw] = '\0';
  char temp = tupp[16-digits];
  tupp[16-digits] = '\0';
  lcd.setCursor(0,0);
  lcd.print(tupp);
  tupp[16-digits] = temp;  
  lcd.setCursor(0,1);
  lcd.print(tlw);
  
  lcd.setCursor(16 - digits,0);
  lcd.print(score);

  tupp[hhp] = us;
  tlw[hhp] = ls;
  return col;
}


void buttonPush() {
  bpsh = true;
}

void setup(){
  pinMode(rw, OUTPUT);
  digitalWrite(rw, LOW);
  pinMode(ptc, OUTPUT);
  digitalWrite(ptc, LOW);
  pinMode(but, INPUT);
  digitalWrite(but, HIGH);
  pinMode(ap, OUTPUT);
  digitalWrite(ap, HIGH);
  
  attachInterrupt(0, buttonPush, FALLING);
  
  intGraphics();
  
  lcd.begin(20, 4);
  
}

void loop(){
  static byte hpos = hprl1;
  static byte nterrType = te;
  static byte ntd = 1;
  static bool playing = false;
  static bool blink = false;
  static unsigned int distance = 0;
  
  if (!playing) {
    drawDino((blink) ? hpo : hpos, tupp, tlw, distance >> 3);
    if (blink) {
      lcd.setCursor(0,0);
      lcd.print("Press Space");
    }
    delay(250);
    blink = !blink;
    if (bpsh) {
      intGraphics();
      hpos = hprl1;
      playing = true;
      bpsh = false;
      distance = 0;
    }
    return;
  }

  advterrain(tlw, nterrType == tlb ? sts : ste);
  
 
  
  
  
  
  
 
  
  if (--ntd == 0) {
    if (nterrType == te) {
      nterrType = (random(3) == 0) ? tub : tlb;
      ntd = 2 + random(1);
    } else {
      nterrType = te;
      ntd = 10 + random(10);
    }
  }
    
  if (bpsh) {
    if (hpos <= hprl2) hpos = hpj1;
    bpsh = false;
  }  

  if (drawDino(hpos, tupp, tlw, distance >> 3)) 
    playing = false; 
   else 
   {
    if (hpos == hprl2 || hpos == hpj8) 
      hpos = hprl1;
     else if ((hpos >= hpj3 && hpos <= hpj5) && tlw[hhp] != ste) 
      hpos = hpru1;
     else if (hpos >= hpru1 && tlw[hhp] == ste) 
      hpos = hpj5;
    else if (hpos == hpru2) 
      hpos = hpru1;
     else 
      ++hpos;
    
    ++distance;
    
    digitalWrite(ap, tlw[hhp + 2] == ste ? HIGH : LOW);
    

  }

  
  delay(50);
}




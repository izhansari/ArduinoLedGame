
int firstled = 2; //p1's objective
int lastled = 12; //p2's objective
int p1 = A5; //down side. firstled side
int p2 = A0; //up side. lastled side
int p1Score, p2Score = 0;
int delayVal = 50;
int whoPressed = 0;
int newGameCounter = 0;
int currentLit = random(firstled, lastled); //choose a random LED pin to start from. make sure to reset after press.
int randDirection = random(0, 1); //0 means down. 1 means up. make sure to reset after press.
int player = A5; //so player one gets first turn.

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for(int i =firstled; i<lastled; i++){ //set each of leds as output pins.
    pinMode(i, OUTPUT);
    }
  pinMode(p1, INPUT);
  pinMode(p2, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);

  gameDifficulty();

  startupAnimation();
  


  Serial.println(digitalRead(p2));
  Serial.println(analogRead(p2));
}

void loop() {
  match();
}

void match(){
    while((digitalRead(player)==LOW) ){//turn based. so while person whos turn it is is not pressed aka low, continue.
      gameDifficulty();
      //point to currentLit (rand position from setup or off match reset).
      digitalWrite(currentLit, LOW);
      //iterate to next position...
      if(randDirection == 0){
        currentLit--;
        }
      else{
        currentLit++;
        }
      //check if out of bounds and get back in bounds and switch directions.
      if(currentLit < firstled){
        currentLit = firstled+1; randDirection = 1;//if at bottom of LED array, then lets set next LED to firstled+1 and switch randDirection to 1 for up.
        } 
      else if(currentLit > lastled){
        currentLit = lastled-1; randDirection = 0; //if at top of LED array, then set next LED to lastled-1 and switch randDirection to 0 for down.
        } 
      //now light it and wait to see if someone reacts. if so, the light will now stay on when this loop is broken.
      analogWrite(currentLit, 255);
      delay(delayVal);
    }
    if(player == p1){
      player = p2;
      }
    else{player = p1;}
    
    if(digitalRead(p1)==HIGH){Serial.print("P1: "); whoPressed =1;}
    else if(digitalRead(p2)==HIGH){Serial.print("P2: "); whoPressed =2;}
    pressCheck();
//    if(prevPress!=whoPressed){pressCheck();}
  }


void pressCheck(){
  if(whoPressed == 1 && digitalRead(lastled)==HIGH){
      //they score. do something like light up x amount of LEDs w how much they won  
      p1Score++;
      Serial.print("score is ");
      Serial.print(p1Score);
      for(int i = 1; i <= p1Score && p1Score<=2; i++){
          analogWrite(firstled+i, 255);
          delay(500);
        }
    }
  else if(whoPressed == 2 && digitalRead(firstled)==HIGH){
      //they score. win animation.
      p2Score++;
      Serial.print("score is ");
      Serial.print(p2Score);
      for(int i = 1; i <= p2Score && p2Score<=2; i++){
          analogWrite(lastled-i, 255);
          delay(500);
        }
    }
  else{
      //no one scores...nothing happens? or do an animation like blink light 3 times?
      Serial.print("missed");
      for(int i =0; i<3; i++){
          digitalWrite(currentLit, LOW);
          delay(delayVal);
          analogWrite(currentLit, 255);
          delay(delayVal);
        }
    }
  Serial.println();
  if(p1Score == 3 || p2Score == 3){
    Serial.print("WINNER");
    gameOver();
  }
  else{
    Serial.print("Resetting Match >>>>>>> P1 - ");
    Serial.print(p1Score);
    Serial.print("       P2- ");
    Serial.print(p2Score);
    Serial.println();
    resetForNextMatch();
    }
  }


void gameOver(){
  //first turn lights off...
    for(int i = firstled; i<=lastled; i++){
      digitalWrite(i, LOW);
    }
    //just light up each players points together (p1 1 and p2 1. p1 2 and p2 2. p1 3 and p2 3.
    for(int i = 1; i <= 3; i++){
      if(p1Score>=i){
          analogWrite(firstled+i, 255);
        }
      if(p2Score>=i){
          analogWrite(lastled-i, 255);
        }
      delay(700);
      }
    while(newGameCounter < 3 || (digitalRead(p1)==HIGH || digitalRead(p2)==HIGH)){
      //do winFlash animation on winners home light until both buttons pressed and held for 5 counts...
      if(p1Score ==3){
          winFlash(firstled);
        }
      else winFlash(lastled);
      if(digitalRead(p1)==HIGH && digitalRead(p2)==HIGH){
          newGameCounter++;
        }
      }
    cleanSlate();
  }

void winFlash(int pinValue){
    analogWrite(pinValue, 255);
    delay(150);
    digitalWrite(pinValue, LOW);
    delay(150);
  }

void resetForNextMatch(){
  //set all lights off
  for(int i = firstled; i<=lastled; i++){
      digitalWrite(i, LOW);
    }
  //choose new direction and starting spot.
  currentLit = random(firstled, lastled);
  randDirection = random(0, 1);
  }

void cleanSlate(){
  newGameCounter = 0;
  p1Score = 0;
  p2Score = 0;
  whoPressed = 0;
  resetForNextMatch();
  startupAnimation();
  }

void gameDifficulty(){
  if(analogRead(A1) == 0){
    delayVal = 50;
    }
  else if(analogRead(A2) == 0){
    delayVal = 40;
    }
  else if(analogRead(A3) == 0){
    delayVal = 30;
    }  
  else if(analogRead(A4) == 0){
    delayVal = 20;
    }  
  }

void startupAnimation(){
  for(int i = 0; i<2; i++){
   int localSpeed= 39 - (19*i);
   int counter=0;
    //GROW led at mid animation
    while(digitalRead(firstled)==LOW || digitalRead(firstled+1)==LOW){
      if(digitalRead(firstled+counter+1)==HIGH){
        if(counter == 0){
          analogWrite(firstled, 255);
          analogWrite(lastled, 255);
          }
        counter = 0;
        delay(localSpeed);
        }
      else if(firstled+counter+1 == lastled-counter-1){
        digitalWrite(firstled+counter, LOW);
        digitalWrite(lastled-counter, LOW);
        counter++;
        analogWrite(firstled+counter, 255);
        analogWrite(lastled-counter, 255);
        counter=0;
        delay(localSpeed);
        }
      else{
        digitalWrite(firstled+counter, LOW);
        digitalWrite(lastled-counter, LOW);
        counter++;
        analogWrite(firstled+counter, 255);
        analogWrite(lastled-counter, 255);
        delay(localSpeed);
        }
      }

      counter=0;
      while(digitalRead(firstled)==HIGH || digitalRead(firstled+1)==HIGH){
      
          if(digitalRead(firstled+counter+1)==LOW){
        if(counter == 0){
          analogWrite(firstled, 0);
          analogWrite(lastled, 0);
          }
        counter = 0;
        delay(localSpeed);
        }
      else if(firstled+counter+1 == lastled-counter-1){
        digitalWrite(firstled+counter, HIGH);
        digitalWrite(lastled-counter, HIGH);
        counter++;
        analogWrite(firstled+counter, 0);
        analogWrite(lastled-counter, 0);
        counter=0;
        delay(localSpeed);
        }
      else{
        digitalWrite(firstled+counter, HIGH);
        digitalWrite(lastled-counter, HIGH);
        counter++;
        analogWrite(firstled+counter, 0);
        analogWrite(lastled-counter, 0);
        delay(localSpeed);
        }
      }
    }
    int counter = 0;
    while(digitalRead(firstled)==LOW || digitalRead(firstled+1)==LOW){
      if(digitalRead(firstled+counter+1)==HIGH){
        if(counter == 0){
          analogWrite(firstled, 255);
          analogWrite(lastled, 255);
          }
        counter = 0;
        delay(30);
        }
      else if(firstled+counter+1 == lastled-counter-1){
        digitalWrite(firstled+counter, LOW);
        digitalWrite(lastled-counter, LOW);
        counter++;
        analogWrite(firstled+counter, 255);
        analogWrite(lastled-counter, 255);
        counter=0;
        delay(30);
        }
      else{
        digitalWrite(firstled+counter, LOW);
        digitalWrite(lastled-counter, LOW);
        counter++;
        analogWrite(firstled+counter, 255);
        analogWrite(lastled-counter, 255);
        delay(30);
        }
      }}

//PSEUDO


//Pick random led to start

//WHILE NO ONE PRESSES
//move up to pin 13. 
//then move back down.


//IF PRESS BREAK LOOP...
//If position is on pin 13 then p1 wins
//If position is on pin 4 then p2 wins
//else do again.

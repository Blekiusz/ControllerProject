const int playerOne = A0;
const int playerTwo = A1;
int playerOnePosition = 0;
int playerTwoPosition = 0;
const int LED = 2;
bool light = true;

int incomingByte = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  if(Serial.available() > 0)
  {
    incomingByte = Serial.read();

    if(incomingByte == 'P')
    {

      playerOnePosition = analogRead(playerOne)/2;
      playerTwoPosition = analogRead(playerTwo)/2;
      
      Serial.print(getPadded(playerOnePosition));
      Serial.print("-");
      Serial.println(getPadded(playerTwoPosition));
      
    }
    if(incomingByte == 'L'){
      light = !light;
    }
  }
  digitalWrite(LED, light);
}

String getPadded(int num)
{
  char buff[5];
  char padded[6];

  sprintf(buff, "%.4u", num); // convert to 4 character string and save it to buff
  
  for(int i = 0; i < 5; i++)
    padded[i] = buff[i];
  padded[5] = '\0';

  return String(padded);

}

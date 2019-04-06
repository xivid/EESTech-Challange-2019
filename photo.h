
void photoSetup(){
  pinMode(A0, INPUT);
  Serial.begin(115200);
}

int photoLoop(void) {
  Serial.print("photo: ");
  Serial.println(1024-analogRead(A0));
  return 1024-analogRead(A0);
}

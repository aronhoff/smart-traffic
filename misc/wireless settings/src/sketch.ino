
void setup(){
	Serial.begin(9600);
	Serial1.begin(57600);
	pinMode(13, OUTPUT);
	digitalWrite(13, 0);
	delay(1050);
	Serial.println("Hello");
}

void loop(){
	while(Serial.available()) {
		int inb = Serial.read();
		Serial1.write(inb);
	}
	while(Serial1.available()) {
		int inb = Serial1.read();
		Serial.write(inb);
	}
}

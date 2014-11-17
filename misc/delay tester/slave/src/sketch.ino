
void setup(){
	Serial.begin(57600);
	Serial.println("Hello2");
}

void serialEvent() {
	while(Serial.available()) {
		Serial.write(Serial.read());
	}
}

void loop(){
}

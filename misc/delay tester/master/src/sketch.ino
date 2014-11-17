
void setup(){
	Serial.begin(9600);
	Serial1.begin(57600);
	Serial.println("Hello");
}

bool sent = false;
bool received = false;
uint64_t t = 0;

void serialEvent1() {
	while(Serial1.available())
		if(Serial1.read() == '\n') {
			received = true;
			t = micros()-t;
		}
}

void loop(){
	if(sent == false) {
		t = micros();
		Serial1.println("12345678901234567890");
		sent = true;
	}
	if(sent && received) {
		sent = false;
		received = false;
		Serial.println((int)t);
		delay(500);
	}
}

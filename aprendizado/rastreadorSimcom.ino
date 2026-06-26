#define DEBUG Serial
#define SIMCOM Serial2
#define POWERPIN 18
#define RESETSTM 5

bool printDebug = true;
int countWait = 0;
int countError = 0;
String response = "";
bool waitResponse = false;

const char OKS[] PROGMEM = "OK";
const char ERRORS[] PROGMEM = "ERROR";
const char SEND[] PROGMEM = ">";

const char TESTE[] PROGMEM = "AT\r\n";
const char SDCARD[] PROGMEM = "AT+CPIN?\r\n";
const char SIGNAL[] PROGMEM = "AT+CSQ\r\n";
const char REGISTER[] PROGMEM = "AT+CGREG\r\n";
const char IMEI_SIMCOM[] PROGMEM = " AT+CGSN\r\n";
const char SIMCARD_NUMBER[] PROGMEM = "AT+CICCID\r\n";
const char RESET_SIMCOM[] PROGMEM = "AT+CRESET\r\n";
const char SINCK_CLOCK[] PROGMEM = "AT+CCLK?\r\n";
const char TIMEZONE[] PROGMEM = " AT+CTZU=1\r\n";

//COMANDOS APN - SIMCOM
const char APN[] PROGMEM = "AT+CGDCONT=1,\"IP\",\"iot.datatem.com.br\"\r\n";
const char AUTH[] PROGMEM = "AT+CGAUTH=1,1,\"datatem\",\"datatem\"\r\n";
const char CONNECTED[] PROGMEM = "AT+CGDCONT?\r\n";

//COMANDOS MQTT - SIMCOM
const char START_MQTT[] PROGMEM = "AT+CMQTTSTART\r\n";
const char STOP_MQTT[] PROGMEM = "AT+CMQTTSTOP\r\n";
const char CLIENT_MQTT[] PROGMEM = "AT+CMQTTACCQ=0,\"%s\"\r\n";
const char CLIENT_MQTT_CONNECTED[] PROGMEM = "AT+CMQTTACCQ?\r\n";
const char TOPIC_WILL_CONNECT[] PROGMEM = "AT+CMQTTWILLTOPIC=0,\"%d\"\r\n";
const char MSG_WILL_CONNECT[] PROGMEM = "AT+CMQTTWILLMSG=0,\"%d\",1\r\n";
const char SERVER_MQTT[] PROGMEM = "AT+CMQTTCONNECT=0,\"%s\",60,1\r\n";
const char STOP_SERVER[] PROGMEM = " AT+CMQTTSTOP\r\n";
const char DISCONNECT_SERVER_MQTT[] PROGMEM = " AT+CMQTTUNSUB=0,\"%s\",0\r\n";
const char TOPIC_SUBSCRIBE_CONNECT[] PROGMEM = "AT+CMQTTSUB=0,\"%d\",1\r\n";
const char TOPIC_PUBLISH_CONNECT[] PROGMEM = "AT+CMQTTTOPIC=0,\"%d\"\r\n";
const char PUBLISH_PAYLOAD[] PROGMEM = "AT+CMQTTPAYLOAD=0,\"%d\"\r\n";
const char PUBLISH [] PROGMEM = "AT+CMQTTPUB=0,1,60\r\n";
const char TOPIC_SUBSCRIBE[] PROGMEM = "AT+CMQTTSUBTOPIC=0,\"%d\",1\r\n";
const char SUBSCRIBE[] PROGMEM = "AT+CMQTTSUB=0\r\n";
const char UNSUBSCRIBE[] PROGMEM = " AT+CMQTTUNSUB=0,\"%d\",0\r\n";
const char REALESE_CLIENT[] PROGMEM = "AT+CMQTTREL=0\r\n";


//COMANDOS GPS-GNSS - SIMCOM
const char START_GPS[] PROGMEM = "AT+CGNSSPWR=1\r\n";
const char STOP_GPS[] PROGMEM = "AT+CGNSSPWR=0\r\n";
const char READ_GPS[] PROGMEM = "AT+CGPSINFO\r\n";
const char READ_GNSS[] PROGMEM = "AT+CGNSSINFO\r\n";
const char SET_GPS[] PROGMEM = "AT+CGPSINFO=2\r\n";
const char SET_GNSS[] PROGMEM = "AT+CGNSSINFO=2\r\n";

String idRaptor;
bool flag4G = false;

char * converterStringChar(String buffConverter);
String _atcmd(const char* ATcommand, int timeout);
bool _responseAT(String atCommand, const char* expected_answer1, const char* expected_answer2, const char* expected_answer3, const char* expected_answer4);
bool _responseAT(String atCommand, const char* expected_answer1, const char* expected_answer2, const char* expected_answer3);
bool _responseAT(String atCommand, const char* expected_answer1, const char* expected_answer2);	
bool _responseAT(String atCommand, const char* expected_answer1);

bool _waitResponse(const char* expected_answer, int timeout);	
bool _waitResponse(const char* expected_answer, const char* expected_answer2, int timeout);
bool _waitResponse(const char* expected_answer, const char* expected_answer2, const char* expected_answer3, int timeout);
bool _waitResponse(const char* expected_answer, const char* expected_answer2, const char* expected_answer3, const char* expected_answer4, int timeout);
String _waitResponseString(const char* expected_answer, int timeout);

bool powerSimcom();
bool resetSimcom();
bool setupInitial();
bool setupMqtt(const char * imei, const char * msgWill, const char * serverMqtt);
bool mqttConnected();
bool subscribeConnected();
bool unSubscribeConnected();
bool disconectedMqtt();
bool disconectedClient();
bool mqttStop();
void getImei();

bool powerOnSimcomGPS();
bool powerOffSimcomGPS();
String positionGPS();
String positionGNSS();

char* converterStringChar(String buffConverter) {
	String buffServidor = "";
	buffServidor = buffConverter;
	char* buff = new char[buffServidor.length() + 1];
	strcpy(buff, buffServidor.c_str());
	return buff;
}
String _atcmd(const char* ATcommand, int timeout) {
	countWait = 0;
	response = "";	

	SIMCOM.println(ATcommand);	

	while (!SIMCOM.available()) {		
		if (countWait > timeout) {
			break;
		}
		countWait++;
		
	}
	if (SIMCOM.available() > 0 && SIMCOM.available() != NULL) {
		
		response = (SIMCOM.readString());
		if (printDebug) {
			DEBUG.print("SIMCOM RESPONSTA : ");
			DEBUG.println(response);
		}
	}
	
	SIMCOM.flush();
	return response;
}
bool _responseAT(String atCommand, const char* expected_answer1, const char* expected_answer2, const char* expected_answer3, const char* expected_answer4) {
	
	const char* responseAction = converterStringChar(atCommand);
	
	if (strstr(responseAction, expected_answer1) != NULL) {
		SIMCOM.flush();
		return true;
	}
	if (strstr(responseAction, expected_answer2) != NULL) {		
		SIMCOM.flush();
		return true;
	}
	if (strstr(responseAction, expected_answer3) != NULL) {	
		SIMCOM.flush();
		return true;
	}
	if (strstr(responseAction, expected_answer4) != NULL) {	
		SIMCOM.flush();
		return true;
	}	
	
	return false;
	SIMCOM.flush();
	
}
bool _responseAT(String atCommand, const char* expected_answer1, const char* expected_answer2, const char* expected_answer3) {
	
	const char* responseAction = converterStringChar(atCommand);
	
	if (strstr(responseAction, expected_answer1) != NULL) {	
		SIMCOM.flush();
		return true;
	}
	if (strstr(responseAction, expected_answer2) != NULL) {	
		SIMCOM.flush();
		return true;
	}
	if (strstr(responseAction, expected_answer3) != NULL) {	
		SIMCOM.flush();
		return false;
	}
	SIMCOM.flush();
	return false;
}
bool _responseAT(String atCommand, const char* expected_answer1, const char* expected_answer2) {
	
	const char* responseAction = converterStringChar(atCommand);

	if (strstr(responseAction, expected_answer1) != NULL) {
		SIMCOM.flush();
		return true;
	}
	if (strstr(responseAction, expected_answer2) != NULL) {
		SIMCOM.flush();
		return false;
	}
	return false;
}
bool _responseAT(String atCommand, const char* expected_answer1) {
	
	const char* responseAction = converterStringChar(atCommand);

	if (strstr(responseAction, expected_answer1) != NULL) {
		SIMCOM.flush();
		return true;
	}
	return false;
}

bool _waitResponse(const char* expected_answer, int timeout) {

	int countWait = 0;

	String responseAction = "";

	while (!SIMCOM.available()) {		
		countWait++;
		if (countWait > timeout) {
			if (printDebug) {
				DEBUG.println("timeout wait");
				waitResponse = false;
				return false;
			}

		}		
		delay(1);
	}
	

	if (SIMCOM.available() > 0) {
		
		responseAction = SIMCOM.readString();
		if (printDebug) {
			DEBUG.println(responseAction);
		}
	}

	if (strstr(converterStringChar(responseAction), expected_answer) != NULL) {
		waitResponse = true;
		return true;
	}
	return false;
}
String _waitResponseString(const char* expected_answer, int timeout) {

	int countWait = 0;

	String responseAction = "";

	while (!SIMCOM.available()) {		
		countWait++;
		if (countWait > timeout) {
			if (printDebug) {
				DEBUG.println("timeout wait");
				waitResponse = false;
				return "TIMEOUT";
			}
		}
		delay(1);
	}
	

	if (SIMCOM.available() > 0) {
		
		responseAction = SIMCOM.readString();
		if (printDebug) {
			DEBUG.println(responseAction);
		}
	}

	if (strstr(converterStringChar(responseAction), expected_answer) != NULL) {
		waitResponse = true;
		return responseAction;
	}
	return "ERRORS";
}
bool _waitResponse(const char* expected_answer, const char* expected_answer2, int timeout) {

	int countWait = 0;
	String responseAction = "";
	while (!SIMCOM.available()) {		
		countWait++;
		if (countWait > timeout) {
			if (printDebug) {
				DEBUG.println("timeout wait");
				waitResponse = false;
				return false;
			}
			DEBUG.println("*");
		}
		delay(1);
	}	

	if (SIMCOM.available() > 0) {
	
		responseAction = SIMCOM.readString();
		if (printDebug) {
			DEBUG.println(responseAction);
		}
	}

	if (strstr(converterStringChar(responseAction), expected_answer) != NULL) {
		waitResponse = true;
		return true;
	}
	if (strstr(converterStringChar(responseAction), expected_answer2) != NULL) {
		waitResponse = false;
		return false;
	}
	return false;
}
bool _waitResponse(const char* expected_answer, const char* expected_answer2, const char* expected_answer3, int timeout) {

	int countWait = 0;
	String responseAction = "";
	while (!SIMCOM.available()) {		
		countWait++;
		if (countWait > timeout) {
			if (printDebug) {
				DEBUG.println("timeout wait");
				waitResponse = false;
				return false;
			}
			DEBUG.println("*");
		}
		delay(1);
	}	

	if (SIMCOM.available() > 0) {
	
		responseAction = SIMCOM.readString();
		if (printDebug) {
			DEBUG.println(responseAction);
		}
	}

	if (strstr(converterStringChar(responseAction), expected_answer) != NULL) {
		waitResponse = true;
		return true;
	}
	if (strstr(converterStringChar(responseAction), expected_answer2) != NULL) {
		waitResponse = true;
		return true;
	}
	if (strstr(converterStringChar(responseAction), expected_answer3) != NULL) {
		waitResponse = false;
		return false;
	}
	return false;
}
bool _waitResponse(const char* expected_answer, const char* expected_answer2, const char* expected_answer3, const char* expected_answer4, int timeout) {

	int countWait = 0;
	String responseAction = "";
	while (!SIMCOM.available()) {		
		countWait++;
		if (countWait > timeout) {
			if (printDebug) {
				DEBUG.println("timeout wait");
				waitResponse = false;
				return false;
			}
			DEBUG.println("*");
		}
		delay(1);
	}	

	if (SIMCOM.available() > 0) {
	
		responseAction = SIMCOM.readString();
		if (printDebug) {
			DEBUG.println(responseAction);
		}
	}

	if (strstr(converterStringChar(responseAction), expected_answer) != NULL) {
		waitResponse = true;
		return true;
	}
	if (strstr(converterStringChar(responseAction), expected_answer2) != NULL) {
		waitResponse = true;
		return true;
	}
	if (strstr(converterStringChar(responseAction), expected_answer3) != NULL) {
		waitResponse = true;
		return true;
	}
	if (strstr(converterStringChar(responseAction), expected_answer4) != NULL) {
		waitResponse = false;
		return false;
	}
	return false;
}
 
bool powerSimcom()
{	
	digitalWrite(POWERPIN, HIGH);
	delay(3000); 
	digitalWrite(POWERPIN, LOW);	
	
	if (!_waitResponse(OKS,ERRORS,30000))
	{
		return false;
	}	
	return true;	
}
bool resetSimcom()
{
	if (!_responseAT(_atcmd(RESET_SIMCOM, 30000), OKS, ERRORS))
	{
		DEBUG.println("ERROR APN SETUP");
		return false;
	}
	return true;	
}
bool setupInitial()
{
	if (!_responseAT(_atcmd(APN, 30000), OKS, ERRORS))
	{
		DEBUG.println("ERROR APN SETUP");
		return false;
	}
	
	if (!_responseAT(_atcmd(AUTH, 30000), OKS, ERRORS))
	{
		DEBUG.println("ERROR AUTH SETUP");
		return false;
	}
	
	if (!_responseAT(_atcmd(REGISTER, 30000), OKS, ERRORS))
	{
		DEBUG.println("ERROR REGISTER OPERATOR SETUP");
		return false;
	}
	
	if (!_responseAT(_atcmd(CONNECTED, 30000), OKS, ERRORS))
	{
		DEBUG.println("ERROR AUTH SETUP");
		return false;
	}
	return true;	
}
bool setupMqtt(const char * imei, const char * msgWill, const char * serverMqtt)
{
	char bufferSet[256];
	int lengthSend = sizeof(imei);
	int lengtMsgWill = sizeof(msgWill);
	String serverIp = "\"tcp://";
	serverIp.concat(serverMqtt);
	serverIp.concat(":1883\"");	
	
	if (!_responseAT(_atcmd(START_MQTT, 30000), OKS, ERRORS))
	{
		DEBUG.println("ERROR MQTT START");
		return false;
	}
	
	memset(bufferSet, '\0', 200);
	sprintf(bufferSet, CLIENT_MQTT, imei);
	
	if (!_responseAT(_atcmd(CLIENT_MQTT, 30000), OKS, ERRORS))
	{
		
		DEBUG.println("ERROR CLIENT MQTT START");
		return false;
	}		
	
	memset(bufferSet, '\0', 200);
	sprintf(bufferSet, TOPIC_WILL_CONNECT, lengthSend);
	
	if (_responseAT(_atcmd(bufferSet, 30000), SEND, ERRORS))
	{
		if (!_responseAT(_atcmd(imei, 30000), OKS, ERRORS))
		{
			DEBUG.println("ERROR CLIENT MQTT START");
			return false;
		}
	}
	else
	{
		DEBUG.println("ERROR TOPC MQTT START");
		return false;
	}
	
	
	memset(bufferSet, '\0', 200);
	sprintf(bufferSet, MSG_WILL_CONNECT, lengtMsgWill);
	
	if (_responseAT(_atcmd(bufferSet, 30000), SEND, ERRORS))
	{
		if (!_responseAT(_atcmd(msgWill, 30000), OKS, ERRORS))
		{
			DEBUG.println("ERROR CLIENT MQTT START");
			return false;
		}
	}
	else
	{
		DEBUG.println("ERROR TOPC MQTT START");
		return false;
	}
	
	memset(bufferSet, '\0', 200);
	sprintf(bufferSet, SERVER_MQTT, converterStringChar(serverIp));
	
	if (!_responseAT(_atcmd(bufferSet, 120000), OKS, ERRORS))
	{
		DEBUG.println("ERROR MQTT START");
		return false;
	}
	
	memset(bufferSet, '\0', 200);
	sprintf(bufferSet, TOPIC_SUBSCRIBE_CONNECT, lengthSend);
	
	if (_responseAT(_atcmd(bufferSet, 30000), SEND, ERRORS))
	{
		if (!_responseAT(_atcmd(imei, 30000), OKS, ERRORS))
		{
			DEBUG.println("ERROR CLIENT MQTT START");
			return false;
		}
	}
	else
	{
		DEBUG.println("ERROR TOPC MQTT START");
		return false;
	}	
	return true;	
}
bool mqttConnected()
{
	if (!_responseAT(_atcmd(CLIENT_MQTT_CONNECTED, 30000), OKS, ERRORS))
	{
		DEBUG.println("MQTT NOT CONNECTED");
		return false;
	}
	return true;
}
bool subscribeConnected(const char * imei)
{
	char bufferSet[256];
	int lengthSubscribe = sizeof(imei);
	
	memset(bufferSet, '\0', 200);
	sprintf(bufferSet, TOPIC_SUBSCRIBE, lengthSubscribe);
	
	if (_responseAT(_atcmd(bufferSet, 30000), SEND, ERRORS))
	{
		if (!_responseAT(_atcmd(imei, 30000), OKS, ERRORS))
		{
			DEBUG.println("ERROR TOPIC SUBSCRIBE");
			return false;
		}
	}
	else
	{
		DEBUG.println("ERROR TOPIC SUBSCRIBE");
		return false;
	}
	
	if (!_responseAT(_atcmd(SUBSCRIBE, 30000), OKS, ERRORS))
	{
		DEBUG.println("ERROR SUBSCRIBE");
		return false;
	}	
	return true;
}
bool unSubscribeConnected(const char * imei)
{
	char bufferSet[256];
	int lengthUnSubscribe = sizeof(imei);
	
	memset(bufferSet, '\0', 200);
	sprintf(bufferSet, UNSUBSCRIBE, lengthUnSubscribe);
	
	if (_responseAT(_atcmd(bufferSet, 30000), SEND, ERRORS))
	{
		if (!_responseAT(_atcmd(imei, 30000), OKS, ERRORS))
		{
			DEBUG.println("ERROR TOPIC UNSUBSCRIBE");
			return false;
		}
	}
	else
	{
		DEBUG.println("ERROR TOPIC UNSUBSCRIBE");
		return false;
	}	
	return true;
}
bool disconectedMqtt()
{
	if (!_responseAT(_atcmd(DISCONNECT_SERVER_MQTT, 30000), OKS, ERRORS))
	{
		DEBUG.println("ERROR DISCONNECTED MQTT");
		return false;
	}
	return true;	
}
bool disconectedClient()
{
	if (!_responseAT(_atcmd(REALESE_CLIENT, 30000), OKS, ERRORS))
	{
		DEBUG.println("ERROR DISCONNECTED CLIENT");
		return false;
	}
	return true;	
}
bool mqttStop()
{
	if (!_responseAT(_atcmd(STOP_SERVER, 30000), OKS, ERRORS))
	{
		DEBUG.println("ERROR MQTT STOP");
		return false;
	}
	return true;	
}
void getImei()
{
	String bufferImei;
	bufferImei = _atcmd(IMEI_SIMCOM, 30000);
}
bool powerOnSimcomGPS()
{
	if (!_responseAT(_atcmd(START_GPS, 30000), OKS, ERRORS))
	{
		DEBUG.println("ERROR POWER ON GPS");
		return false;
	}
	return true;
}
bool powerOffSimcomGPS()
{
	if (!_responseAT(_atcmd(STOP_GPS, 30000), OKS, ERRORS))
	{
		DEBUG.println("ERROR POWER OFF GPS");
		return false;
	}
	return true;
}
String positionGPS()
{
	String gps = _atcmd(READ_GPS, 30000);
	return gps;
}
String positionGNSS()
{
	String gps = _atcmd(READ_GPS, 30000);
	return gps;
}

void setup()
{
	DEBUG.begin(115200);
	SIMCOM.begin(115200);
	
	pinMode(POWERPIN, OUTPUT);
	pinMode(RESETSTM, OUTPUT);
	
	digitalWrite(POWERPIN, LOW);
	digitalWrite(RESETSTM, LOW);
}
void loop()
{
	if (!flag4G)
	{	
		
		if (!_responseAT(_atcmd(TESTE, 10000), OKS, ERRORS))
		{
			DEBUG.println("Init Setup 4G");
			
			if (!powerSimcom())
			{
				DEBUG.println("ERROR Power On 4G");
				return;
			}	
			
			DEBUG.println("testa at");
			while (!_responseAT(_atcmd(TESTE, 10000), OKS, ERRORS))
			{
				
				DEBUG.print("count : ");
				DEBUG.println(countError);
				countError++;
				if (countError > 10)
				{
					countError = 0;
					DEBUG.println("sem resposta correta");
					delay(5000);
					return;
				}
			}
			
			if (!setupInitial())
			{
				DEBUG.println("ERROR 4G Init");
				return;
			}
			
			
		}		
		return;
		
		
		
		if (!_responseAT(_atcmd(TESTE, 10000), OKS))
		{
			{
				
			
				
				if (!mqttConnected())
				{
					if (!setupMqtt("121212", "online", "mqttiot.srv.br"))
					{
						DEBUG.println("ERROR Mqtt connect");
						return;
					}
				}
				
				flag4G = true;				
			}		
		}
		else
		{
			DEBUG.println("ERROR POWER ON 4G");
			delay(5000);
		}
		
	}
	
	/*if (DEBUG.available() > 0)
	{
		int countWait = 0;
		
		DEBUG.print("AT COMMAND ENVIADO : ");
		String ATcommand = DEBUG.readString();
		DEBUG.println(ATcommand);
		SIMCOM.println(ATcommand);
		delay(100);		
		
		while (!SIMCOM.available()) 
		{
			if (countWait > 10000) {
				break;
			}
			DEBUG.print("#." + String(countWait) + "-");
			countWait++;
		}
		if (SIMCOM.available() > 0)
		{
			DEBUG.print("RETORNO AT COMMAND : ");
			DEBUG.println(SIMCOM.readString());
		}
		
		DEBUG.flush();
		SIMCOM.flush();
	}*/
}

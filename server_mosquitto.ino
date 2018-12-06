#include <Ethernet.h>

EthernetServer server(80);

void setup() {
  // detengo el server si estaba corriendo
  system("killall lighttpd");
  delay(3000);
  
  system("telnetd -l /bin/sh");
  delay(2000);
  // bajo la interfaz enp0s20f6
  system("ip link set enp0s20f6 down");
  delay(2000);
  // la renombro a eth0 por convencion
  system("ip link set enp0s20f6 name eth0");
  delay(2000);
  //levanto la interfaz
  system("ip link set eth0 up");
  delay(2000);
 
  // Le asigno la ip dentro de la subred que se encuentra el resto del sistema PC-Galileos
  system("ip addr add 169.254.4.103/8 dev eth0");
  delay(2000);
  // me posiciono en el directorio raiz
  system("cd /");
  // inicializamos el demonio mosquitto
  system("mosquitto -d");
  delay(2000);

  // start the server
  server.begin();
  delay(2000);
}

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) 
  {
    // make a String to hold incoming data from the client
    String currentLine = "";
    
    Serial.println("Nuevo Cliente");

    String selection = "No se ha seleccionado ninguna opcion aun";
    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();
        Serial.write(c);

        // Some checking/work is needed at the end of line
        if (c == '\n')
        {
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) 
          {
            sendPagetoClient(client, selection);
            break;
          }
          currentLine = "";
        }
        else if (c != '\r') {
          // if you got anything else but a carriage return character,
          currentLine += c;   // add it to the end of the currentLine
        }
      }

      // Check to see the client request:
      if (currentLine.endsWith("GET /LED_on")) {
        selection = "Sel1";
        // envio orden de encender el Led
        system("mosquitto_pub --host 169.254.4.103 --topic led --message \"LED_ON\"");
        delay(2000);
        
      }
      if (currentLine.endsWith("GET /LED_off")) {
        selection = "Sel2";
        // envio orden de apagar el LED
        system("mosquitto_pub --host 169.254.4.103 --topic led --message \"LED_OFF\"");
        delay(2000);
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("Cliente Desconectado");
  }
  else {
    Serial.println("client no esta listo");
  }
}

/***********************************************************************/
void sendPagetoClient(EthernetClient client, String selected)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
 
  // the content of the HTTP response follows the header:
  client.print("Para encender el Led <a href=\"/LED_on\"> haga click aqui</a><br>");
  client.print("Para apagarlo <a href=\"/LED_off\"> haga click</a><br>");
  
  if (selected == "LED_on") {
    client.print("Se ha encendido el LED ");
  }
  if (selected == "LED_off") {
    client.print("Se ha apagado el LED ");
  }
  // The HTTP response ends with another blank line:
  client.println();
}

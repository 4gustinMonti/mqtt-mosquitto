void setup() {
  // bajo la interfaz enp0s20f6
  system("ip link set enp0s20f6 down");
  // la renombro a eth0 por convencion
  system("ip link set enp0s20f6 name eth0");
  delay(2000);
  //levanto la interfaz
  system("ip link set eth0 up");
  delay(2000);
 
  //Le asigno la ip dentro de la subred que se encuentra el resto del sistema PC-Galileos
  system("ip addr add 169.254.4.105/8 dev eth0");
  delay(2000);
  // pin 13 como salida -> LED interno  
  pinMode(13, OUTPUT);
  delay(2000);
  // Apago LED interno por si estaba encendido
  digitalWrite(13, LOW);
  delay(2000);
  // inicializamos el demonio mosquitto
  system("mosquitto -d");
  delay(2000);
  //ejecutamos el script de python que se comunica mediante mqtt mosquitto
  system("cd /");
  system("chmod a+x cliente_suscriptor.py");
  delay(2000);
  system("./cliente_suscriptor.py");
  delay(2000);
}
void loop() {
  FILE *f_led_on;
  FILE *f_led_off;
  do {
      delay(1000);
      // intenta abrir led_on.txt
      f_led_on  = fopen("led_on.txt", "r");
      delay(1000);
      // intenta abrir led_off.txt
      f_led_off  = fopen("led_off.txt", "r");
  } while (f_led_on == NULL && f_led_off == NULL);
  // verifico cual archivo abrio exitosamente
  if (f_led_on) {
    //se enciende el led
    digitalWrite(13, HIGH); 
    fclose(f_led_on);
    system("cd /");
    system("rm led_on.txt");
  }
  if (f_led_off) {
    //se apaga el led
    digitalWrite(13, LOW);
    fclose(f_led_off);
    system("rm led_off.txt"); 
  }
}

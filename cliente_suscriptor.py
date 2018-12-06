#!/usr/bin/python
import paho.mqtt.client as mqtt

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    client.subscribe("led")
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    mensaje_recibido = str(msg.payload)
    topico_recibido = str(msg.topic)
    if topico_recibido == 'led':
        if mensaje_recibido == 'LED_ON':
            f = open('led_on.txt', 'w')
            # Creo un archivo llamado led_on.txt
            # y desde arduino enciendo dicho led
            f.write('led_on')
            f.close()
        if mensaje_recibido == 'LED_OFF':
            f = open('led_off.txt', 'w')
            # Creo un archivo llamado led_off.txt
            # y desde arduino apago dicho led
            f.write('led_off')
            f.close()

mensaje_recibido= ''
topico_recibido = ''
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("169.254.4.103", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()
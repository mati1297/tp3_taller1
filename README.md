# Ejercicio 3: My Little Middleware
## Taller de Programación I

**Matias Charrut** - 101137

[Link al repositorio](https://github.com/mati1297/tp3_taller1)

## Descripcion

El ejercicio consistió en el armado de un mini *Middleware*. En este, se tiene un servidor al que se conectan varios clientes, los cuales pueden definir colas de mensajes, y luego operar sobre ellas haciendo *push* y *pop* intercambiando así mensajes. El servidor puede atender varios clientes a la vez utilizando *threads*. 

### Diagrama de flujo

En el siguiente diagrama de flujo se puede ver a grandes rasgos como es el funcionamiento del server. Se muestra donde comienzan y terminan los hilos y cuales son las condiciones.

<p align=center>
    <img src="images/server_flowchart.png" alt="server_flowchart"/>
</p>


## Clases

El servidor se ejecuta a través de la clase principal ```Server```, el cual posee un método principal ```execute()```, el cual es ejecutado por el ```main``` correspondiente a server. Esta clase bindea y deja en escucha el ```Socket```, para luego lanzar el ```AccepterThread``` y queda en espera de que se ingrese la letra 'q' por ```stdin```. Cuando esto ocurra, cerrará el ```Socket```, con lo que morirá el hilo aceptador y luego le hará ```join```, para luego terminar la ejecución.

El hilo aceptador, ```AcceptorThread``` es un wrapper de ```std::thread```, el cual se encarga de aceptar conexiones entrantes, y lanzar por cada una un hilo de cliente ```ClientThread```, que atenderá a los distintos clientes. ```AccepterThread``` quedará bloqueado en un ```accept()``` de ```Socket```, cuando acepte una nueva conexión, revisará la lista de clientes abiertos, y si hay alguno muerto, lo eliminará de su lista y joineará su *thread*. Cuando el *main thread* cierre el socket que acepta, este hilo iterará por todos sus clientes y los irá deteniendo (cerrando sus respectivos sockets), y luego los joineará, para terminar su ejecución.

Los hilos de clientes, ```ClientThread```, se ocupan de atender a los clientes, estos quedan bloqueados en ```receive``` de su ```Socket```, esperando a que lleguen instrucciones. Si el hilo aceptador llama a su método ```stop()```, éste cerrará el socket, y seteará una variable ```std::atomic keep_talking``` en ```false```, de manera tal que el hilo se detendrá.

Por su parte, el cliente trabaja con un solo hilo y se ejecuta a través de la clase principal ```Client```, el cual posee un método principal ```execute()```, el cual se encarga de conectarse al servidor, y a partir de allí esperará que se ingresen instrucciones a través de ```stdin```. El programa enviará los comandos a través de su socket, y quedará en espera de recibir datos si fuera necesario. Para salir del programa se debe recibir por ```stdin``` el comando ```exit```.

### Comunicación

Para la comunicación se utilizan tres clases: ```Protocol```, ```Packet``` y ```Socket```.

La primera se utiliza para enviar y recibir mensajes y comandos. Cuenta con una referencia al ```Socket``` que debe utilizar, y tanto los hilos de cliente como cada cliente, poseen una instancia de esta clase, posee métodos como ```sendDefineQueue()```, ```sendPushMessage()```, entre otros utilizados por los clientes. Y otros como ```receive()```, que interpreta el mensaje que se recibe, utilizado por el servidor.

La segunda, ```Packet```, se utiliza para encapsular los paquetes que se envian y reciben, es utilizada por ```Protocol``` y ```Socket```, la primera generandolos y la otra enviándolos y recibiendolos. Permite de diversas maneras agregar bytes al mensaje, así como también posee métodos para obtener sus datos para leer y enviar.

La tercera, ```Socket```, es un wrapper de sockets. Encapsula los file descriptors y posee métodos de conexión como ```BindAndListen()```, ```Connect()```, ```Accept```, asi como también métodos para enviar y recibir, ```send()``` y ```receive```, los cuales reciben como parámetros objetos tipo ```Packet``` e implementan los ciclos de envio y recepción.

### Colas y su almacenamiento

Para la cola se utiliza el template ```BlockingQueue```, en este caso con ```std::string```. Esta cola es un monitor bloqueante de la clase ```std::queue``` de la STL. Cuando se intenta hacer *pop* sin que haya nada en la cola, ésta bloqueará hasta que alguien haga un *push*. 

La cola posee un método ```unlock()```, el cual permite forzar a que se destrabe la cola. Este método es llamado al cerrar el server. Y se implementó ya que podría ocurrir el caso en que, el server se cierre mientras que un cliente está esperando un *pop*. Si esto ocurriese, nunca se podría cerrar el hilo ya que quedaría un *deadlock*.

Para el almacenamiento de las colas se utiliza el template ```ProtectedMapBlockingQueue```, el cual es un monitor de ```std::map``` el cual en este caso usa como *key* ```std::string```. Este posee métodos para insertar y obtener las colas. Además de un método ```unlockAll()```, para destrabar todas las colas que posee a la vez.

### Diagrama de clases

A continuación se puede ver un diagrama de clases donde se observan las relaciones y dependencias de las distintas clases del programa.

<p align=center>
    <img src="images/class_diagram.png" alt="class_diagram"/>
</p>

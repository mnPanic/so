# Distribuidos

Que es un sistema distribuido? Diferentes maquinas que no compartan un clock.
Entonecs se pierde la nocion de simultaneidad, que quiere suceder que algo
suceda antes o despues.

En distribuidos ni siquiera tenes la garantia de que se ejecutan a la vez,
antes, o despues, todo depende del punto de vista.

Un sistema distribuido podrian ser notebooks conectadas, en un datacenter varios
servidores, o una sola computadora con varios procesadores compartiendo un
mother. O las supercomputadoras.

Si yo mando un msj a varias computadoras que estan conectadas, puede llegar a
destiempo.

Necesitamos que todas las computadoras se pongan de acuerdo en una hora

## 8.1 - Instantanea Global (ffrizo)

Global porque quiero saber el estado de todos los nodos del sistema.

Como no comparten clocks, necesito que si los miro a todos juntos, eso tenga
sentido que haya sucedido en algun momento del tiempo.

idea: un nodo la inicia.
el estado del sistema es el esstado de todos sus nodos, pero necesitamos que
sean consistentes entre ellos.

Por ejemplo, si un nodo en su estado dice que

    A B
    B recibio un mensaje de A
    no puede suceder que A crea que no se lo envio, porque la recepcion siempre
    es posterior al envio. Pero al reves si, ya que el mensaje puede estar en el
    aire

Se puede armar un orden parcial.

Algoritmo (sacado de la diapo de chapa)

- Cuando se quiere hacer una instantanea, envia un mensaje de `marca`.
- Cuando recibo por primera vez un mensaje de marca, guardo una copia de mi
  estado y le mando a todos los otros procesos un mensaje de marca
- Empiezo a registrar los mensajes que recibo hasta que recibo marca de todos

> deberia recibir marca de todos una vez, pues todos lo mandan una vez a todos

- Cuando recibo el ultimo de marca, se que termino la instantanea. Y todos los
  procesos guardaron la copia de su estado. Los mensajes de la primera a la
  ultima estaban en vuelo durante la instantanea.

Preguntas de la practica

a. RPC: Tengo codigo, que parece que se corre en local pero se corre en otro
lugar. se reemplaza por una funcion que hace un llamado a una funcion que esta
en otro lado.

Sobre RPC hay que construir pasaje de mensajes. Si es viable ya esta.
No necesariamente seria lo mas eficiente.

NPI es mas piola para hacer esto. interfaz de pasaje de mensajes. RPC no esta
pensado para pasarse mensajes.

b. ptreads: No tiene nada que ver con sistemas distribuidos.
Necesitamos compartir memoria y un clock para poder usarlo.

c. NFS: Network File System

Guardar un archivo en un directorio compartido es un pasaje de mensaje.
Se podria hacer. Se puede implementar pasaje de mensajes.

Es suficiente tener un espacio de archivos compartido

## 8.4 - ffrizo

La diferencia clave viene por si tienen clock compartido. Entonces no seria.

Otra cosa seria si existe la posibilidad de que se caigan partes. En un sistema
paralelo eso no puede pasar.

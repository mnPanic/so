# Distribuidos 2

Problemas clásicos y algunos algoritmos para abordarlos.

Los algoritmos de hoy son 100% distribuidos

## Modelo de fallas

Antes tenemos que entender cuales son las *condiciones de contexto*

- Nadie falla: Pero no es que supongo que nada va a fallar, te garantizo cierto resultado solo para las corridas en donde nada falla. Si nada falla dejo de darte garantias
- Los procesos caen y no levantan
- Los procesos caen pero no levantan: Si levantan, tengo que lidiar con su información inconsistente. Esto no es un problema para los algoritmos *stateless*.
- Los procesos caen, y pueden levantar pero solo en cierto momento
- La red se particiona
- Fallas bizantinas: Los procesos pueden comportarse de manera impredecible

Acá no contemplamos la seguridad.

## Métricas de complejidad

Una metrica que se tiene mucho en cuenta es la cantidad de mensajes que se envían a través de la red.

Pero no necesariamente el de menor cantidad de mensajes es mejor, ya que pueden ser más o menos robustos en cuanto al modelo de fallas.

Otra forma es *cuanta info necesitan*. Por ej, tengo `n` nodos y necesito llegar a un consenso: Una forma es votar, pero es necesario saber la cantidad de nodos

No es lo mismo un algoritmo donde simplemente difundo info y me escucha quien me escucha, y otro en donde necesito saber como se llaman los demás.

## Familias de problemas

Familias de problemas a abordar

- Orden de ocurrencia entre los eventos
- Exclusión mutua
- Consenso

## Exclusión mutua distribuida

### Token passing

Agarro los procesos que intervienen y los ordeno formando un anillo logico.

Hay un token que esta girando todo el tiempo en la red, y cuando quiero entrar a la sección crítica espero a que me llegue el token, y recién ahí entro.

Cuando no lo necesito lo paso, y cuando lo necesito lo espero hasta que llegue.

Pros

- No tiene un unico punto de falla
- Es relativamente facil agregar un nodo nuevo

Cons

- Que pasa si se muere uno?
- Hay mensajes circulando aun cuando nadie quiere entrar a CRIT
- Hay poder de computo desperdiciado al pasar esos mensajes

Implementaciones

- FDDI
- TDMA

### otro

Solucion principal: No hay mensajes circulando al pedo

Genera un orden ya que cuando yo entro

- Cuando quiero entrar a CRIT, mando un mensaje a todos (incluyendome a mi) que tiene dos parametros. El timestamp del momento y el proceso que lo solicita.
  
  `solicitud(Pi, ts)`

- Cada proceso cuando lo recibe, me responde ahora o en un rato (encolarlo)
- Si todos me dijeron OK, puedo entrar
- Si entro, al salir todos los pedidos que tenia encolados los tengo que responder.
- Respondo inmediatamente si
  - No me interesa entrar a CRIT
  - Quiero entrar, aun no lo hice y el `ts` del pedido que recibo es anterior a mi pedido, el otro tiene prioridad.

Esto requiere utilizar en simultaneao el algoritmo para tener el timestamp de manera global. Usa como fundamento eso.

Supuestos

- No se pierden mensajes
- uno mas

## Locks distribuidos

Tengo una red y recursos en esa red

1. Se puede pensar como computadoras que tienen archivos, y hay distintos procesos que tienen acceso exclusivo a distintos recursos en distintos lugares de la red. Y quieren obtener un lock para esos recursos
2. O tambien, que esos distintos archivos son copias de la misma informacion, entonces me tengo que garantizar de lockear todos a la vez

### Protocolo de mayoria

Segundo caso, objeto con n copias a lo largo de la red.

Considero que tengo un lock sobre el objeto si lo consegui en al menos la mitad mas uno. Yo pido a los distintos nodos si puedo tomarlo o no. Cada uno responde con si o no.

Cada copia ademas tiene un numero de version, entonces si lo escribo aumento el numero de version. Tomo el mas alto de los que habia y lo incremento en uno.

Como siempre agarras la mitad mas uno, aunque haya quedado desactualizado, al menos una copia es de las mas actualizadas.

### Eleccion de lider

Una serie de procesos elige a uno como lider para algun tipo de tarea.

En una red sin fallas, cada proceso sabe si es el lider o no lo es. Cada proceso comienza diciendo que no es el lider.

Los organizamos en un anillo, y le ponemos a cada uno un ID y lo hacemos circular. Cada proceso hace circular el mayor entre el mio y el que circula.

Cuando dio toda la vuelta, el numero que llega es el del lider.

### Snapshot global consistente

Hay un estado general de toda la red que es la suma del estado de todos los nodos.

ver en diapos

Deteccion de propiedades estables: una vez que se consiguen ya no cambian.

### 2PC (Two Phase Commit)

Se usa mucho en bases de datos distribuidas. Se trata de asegurarnos de que una transaccion se hizo de manera atomica.

Cada nodo puede decidir proceder o no proceder. Puedo decidir escribir si todos me dicen que si.

- En la primera fase le preguntamos a todos si estan de acuerdo con hacer la  transaccion, a ver si ven una objecion en avanzar. Con un solo no damos todo de baja.
- Voy anotando los si.
- Cuando paso un timeout y no recibi todos los si, probablemente algun nodo murio
- Si recibi un si de todos, nadie objeta, y podemos escribir, pero le tengo que avisar a todos. Pero al avisarle podria no avisarle.

Si bien no es a prueba de balas, en la practica funciona bastante bien. Puede haber acuerdo bizantino.

## bibliografia

Leer el distributed algorithms de nancy lynch


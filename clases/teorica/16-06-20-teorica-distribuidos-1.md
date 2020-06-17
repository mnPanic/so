# Sistemas distribuidos

Varias maquinas conectadas a una red

Fortalezas

TODO

Debilidades

- Dificultades en la sincronizacion
- Dificultad para mantener la coherencia
- No suelen compartir clock
- Llevan a pensar que podemos necesitar trabajar con información parcial

## Memoria compoartida

### Hardware

#### UMA - Uniform Memory Access

Todos los procesadores comparten la mmeoria a través del mismo bus.

#### NUMA - Non uniform memory access

Cada procesador tiene su memoria propia pero también hay un bus donde se
comparten. Toda la memoria es accesible a todos los procesadores pero no con el
mismo costo. Acceder a la propia es mas barato pero a la de otro tengo que ir a
través del bus.

### Software

- Linda: (tuple-spaces) parecido a map and reduce. Pensar la memoria como un
  repositorio de tuplas, JavaSpaces.
- Arrays distribuidos

## Sin memoria compartida

100% distribuido, computadoras distintas que no comparten memoria y se comunican
a través de una red. Estas computadoras son llamadas *nodos*.

La forma en la que se organizan para procesar es lo que se conoce como
*arquitectura del software*.

### Posibles arquitecturas

#### Telnet

*telnet* en realidad es un protocolo y programa que permite conectarse
remotamente a otro equipo. Es como ssh pero sin encripción.

Hay un equipo que recibe las conexiones que es donde se hace el grueso del
procesamiento. Hay una utilización asimetrica de los recursos. Un servidor
recibe las peticiones y es donde se ejecuta el cómputo.

#### RPC

Es un mecanismo que le permite a los programas hacer *remote procedure calls*.

[foto diapo 7]

El escenario es que hay una parte de un computo que necesito que corra
necesariamente en otra computadora, la pregunta entonces es como hago.

Creo un *stub* del cliente, que es una función que tiene la misma aridad que la
real, pero que en realidad por abajo envía la petición por la red. Le llega la
función al servidor, el cual computa el resultado y se lo envía.

En c se hace directamente a la hora de linkear. Del otro lado tiene que estar
corriendo un programa conocido como *deamon*, que esta todo el tiempo arriba.

La idea principal de RPC es que hago una "llamada a procedimiento", y el hecho
de que sea remota es algo que se le oculta al programador.

Es un mecanísmo **sincrónico**, el proceso queda bloquedao hasta que no se hace
el camino de ida y vuelta.

Quien pone el poder de cómputo? Es asimétrico.

#### Client/server

Alguien inicia el pedido y se lo manda al otro. Esta y la anterior entran en
eso. Cliente y servidor son roles, dependiendo del servicio una computadora
podría tomar ambos.

#### Mecanismos async

RPC async

- Promises: En el lenguaje de programación hago la llamada y obtengo un objeto
  al que yo le puedo preguntar si tiene el resultado, en algún momento me dice
  que sí y ahí lo puedo inspeccionar.

- Futures

Pasaje de mensajes

- Mailbox
- Pipe
- **MPI**: Message passing interface para C/C++
  - Hay varias bibliotecas que implementan esta API (Application Programming
    Interface) es lo que nosotros solemos llamar una *interfaz*.

    La forma de comunicación es distinta, varios procesos se quieren comunicar
    entre sí sin que necesariamente uno le provea un servicio al otro.

    Por ejemplo, en una aplicacion de mensajeria en la que uno le manda un
    mensaje a la otra persona. Quien es el servidor y quien el cliente?

## Pasaje de mensajes

Presuponemos que solo tienen compartido un canal de comunicación y nada más.
Este puede llegar a ser un bus. Esto hace que sea el más general.

Problemas a considerar

- Tengo que manejar la codificación y decodificación de los datos, ya que son
  canales que permiten transmitir flujos de bytes.

- Si tengo una comunicación async, cada tanto tengo que ver si recibí algún
  mensaje.

- La comunicación es "lenta" (en comparación a la velocidad de un procesador)

- Este canal puede ser imperfecto, en general son *muy* imperfectos. Hoy en día
  gracias a TCP/IP podemos hacer de cuenta que hay una capa que soluciona eso, y
  que los canales son más confiables.

- Transmitir mensajes puede tener un costo económico, en terminos reales ($$) o
  en terminos conceptuale, la complejidad se mide en cantidad de mensajes
  intercambiados entre los nodos.

Problemas que ignoramos en esta materia:

- Los nodos pueden morir ()
- La red se puede partir (partes incomunicadas)

Aparecen variaciones de los algoritmos que pueden soportar falencias

> - Tengo un algoritmo lineal que requiere que todos los nodos esten vivos
> - Tengo otro cuadratico que se banca que se caigan hasta la mitad y que no
> haya inconsistencias.
> - Tengo este otro que se banca que se caigan, pero no que reciban

Conjetura de Brewer: En un entorno distribuido, no podes tener mas de dos de 3

- **Consistencia**: Todos los nodos tienen la misma información
- **Disponibilidad**: El sistema está disponible 24/7
- **Tolerancia a fallas**: Se banca que haya nodos que se caigan

Lograr cada una tiene impacto en las demás. Por ej. si se cae un nodo, mi
sistema sigue funcionando, entonces es tolerante a fallas. Pero entonces,
cuando levanta, levanta con información inconsistente.

## Locks

En los entornos paralelos nos preocupamos por la exclusión mutua, donde lo más
básico de todo es un TestAndSet atómico. En un entorno distribuido no existe.

Cómo implementamos locks?

### Enfoque centralizado

Hacer que uno nodo sea diferente a los demás, el coordinador, entonces cuando
un nodo quiere un recurso se lo pide al coordinador. Este hace de árbitro.

- Tiene que tener capacidad de computo y disponibilidad suficiente.
- Si ese nodo cae, cae toda la red. Tengo **un punto único de falla**.
  Una cadena se corta por el eslabón mas débil.

### Canto pri

El que quiere usarlo, canta pri. Y se lo dice a todos. Si dos personas lo piden
al mismo tiempo, todos lo escuchan en algún orden.

Si dos nodos emiten el mensaje al mismo tiempo, como están a distancias
diferentes, llegan en momentos diferentes. En una red, el orden el que se
reciben los mensajes no necesariamente tiene que ver con el orden en el que
fueron emitidos.

Uno podría decir, si todos tenemos la hora, está todo bien, pero trae otras
dificultades también.

### Ordenar eventos en la red

**Cuando sucede un evento antes que otro en una red?**

Hay protocolos que sincronizan los relojes, el mas famoso siendo NTP (network
time protocol), los cuales tratan de lograr que sea suave. Cuando notan un
desfazaje, lentamente van ajustando el tiempo, para que no haya cambios
abruptos.

Los relojes tienden a desincronizarse. Entonces cuando necesito una
sincronización por tiempo muy fina como esta, no sirven.

Ahora bien, necesitamos sincronizar relojes? En realidad queremos 

Leslie lamport terminó de inventar latex, y a parte hizo muchos aportes a
distribuidos. Lo único que nos importa no es cuando suceden las cosas, sino que
pasó primero.

Y en realidad algo mucho más simple, poder asignar un orden *inequívoco* a los
eventos. No exactamente el orden en el que se dieron en la naturaleza, sino uno
acordado entre todos.

#### Orden parcial reflexivo

Leslie: Solo es necesario tener un orden *parcial no reflexivo* entre los
eventos.

- Dentro de un mismo proceso, A sucede antes que B, A -> B
- Si E es el envio un mensaje y R su recepcion, E -> R, aunque E y R sucedan en
  procesos distintos.

- Transitividad, Si A->B y B->C entonces A->C
- Si no vale A->B ni B->A, voy a asumir que A y B son *concurrentes*. No puedo
  determinar un orden entre ellos

#### Implementacion

- Cada procesador tiene su reloj. Puede ser un reloj real o uno monotonico.
- A cada mensaje le pongo un timestamp
- Cuando yo recibo un mensaje enviado en tiempo t, me fijo que hora tengo.
  - Si es anterior, entonces ahora es despues que el instante, ya que la
    recepcion debe ser mayor al envio. Paso al menos a t+1.

Eso da un orden parcial entre los eventos.

Para convertirlo en orden *total*, rompo los empates de concurrencia.

- Va denuevo
- Resuelvo en base al PID
- De forma arbitraria.
- etc.

### Acuerdo bizantino

Problema

> Si atacan a la vez, ganan, y si atacan de a uno pierden.
> El problema es como hacen para atacar al a vez porque no tienen forma
> instantánea de comunicarse. La forma que tenían era la de enviar mensajeros, que
> decía a que hora iban a atacar
> https://www.youtube.com/watch?v=IP-rGJKSZ3s

Decimos de ir a las 10, pasa la hora y no sabemos.

No hay manera de saber que yo se, que vos sabes, que yo se, etc. si el medio
puede perder mensajes. No se puede resolver el problema del acuerdo bizantino.

Version mas complicada: Pueden ser sobornados (en la realidad: pueden llegar
adulterados los mensajes)

#### Formalizacion

Dados

- **Fallas** en la comunicacion
- V = {0, 1} valores
- **Inicio**

[diapo 22]

**No se puede resolver** teniendo 100% de seguridad, pero si versiones
probabilisticas

## Entornos distribuidos concretos

### Clusters

#### Científico

Significa cosas distintas. En el mundo cientifico es un conjunto de
computadoras conectadas a una red con un scheduler de trabajos. No un scheduler
com oel del SO, sino que le doy una serie de tareas y sus caracteristicas, y
este lo manda a correr de forma distribuida de la red.

**CECAR**: Es el de la facu, un cluster de computo científico. Si somos
investigadores, van a los admins, pedimos una cuenta, y a través de una interfaz
envias el programa y queda en un ranking.

#### Resto del mundo

Cluster es un conjunto de computadoras que están trabajando juntas de alguna
forma. A veces es que proveen el mismo servicio redundante, o distintos.

- **Grid**: Conjunto de clusters (ya no se usa mucho)
- **Clouds**: Tengo una serie de computadoras y te las alquilo, en general por
  temas económicos.
  - Servicio *elástico*: Voy a vender un servicio que durante todo el año sea un
    servicio flojito, y que en cierto momento del año duplica, triplica los
    servidores pero durante una cantidad finita de tiempo.

  - Sofisticación: En vez de girar la perilla, que vaya creciendo solo según
    ciertas reglas. *Autoscaling*. Esa es la magia que provee AWS.

## Scheduling

En sistemas distribuidos

- local: Dar el procesador a un proceso listo
- global: Asignar un proceso a un procesador (**mapping**)

Como hacemos en un entorno distribuido para ver quien toma las tareas?

- Global
  - Estatica: **affinity**: las tareas son asignadas a un procesador. Tal vez
    para resolverlas necesito hacer uso de ciertos recursos que no es tan fácil
    mover a través de la red.
  - Dinámica: **migration** La asignación varía durante la ejecución. Es un
    problema complejo, porque es dificil saber cuando mover, y de donde a donde
    mover. Y quien lo define? Los nodos lo definen de forma distribuida? Un nodo
    solo está destinado a ello? Es un quilombo.


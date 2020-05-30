# IO

Cómo se da la interacción con los dispositivos?

Los dispositivos que más nos interesan son los de almacenamiento. Por las
cuestiones relacionadas con swapping y manejo de páginas.

## Tipos de dispositivos de almacenamiento

- Discos rígidos: Los SOs consideran que están presentes.
- Discos removibles: Pueden estar o no, no p
- Unidades de cinta: Se usan para backup

Discos virtuales: *NAS: Network Attached Storage*

- NFS, CIFS, DFS, AFS, Coda.

## Esquema

Un dispositivo de E/S tiene dos partes

- El dispositivo físico
- El controlador del dispositivo. que interactúa con el SO mediante algún bus
  o registro.

Puede haber un controlador para varios díscos

_dibujo diapo 5_

## Driver

_dibujo diapo 6_

Entre el subsistema de E/S y los dispositivos propiamente dicho, está la capa
de los **drivers**. Son componentes de software muy específicos que encapsulan
conocimiento sobre:

- Particularidades del hw con el que hablan. Si el fabricante cambia la lógica
  de su electrónica, el driver tiene que cambiar también.
- Conoce las particularidades del SO.

Los drivers tienen dos responsabilidades grandes

Son un problema, tienen que correr con máximo privilegio, porque acceden
directamente al hardware. Tienen qeu funcionar rápido, porque la manera
en que yo percibo lo rápido que funciona el sistema depende de la velocidad
de la E/S. Lo tiene hacer de forma correcta y rápida.

## Interacción

- **Polling**:
  - El driver cada tanto se fija si una operación ya terminó.
  - Ventajas: Sencillo de implementar, cambios de contexto controlados.
  - Desventajas: Consume CPU. Es como el busy waiting
- **Interrupciones** (o push):
  - En vez de yo preguntarle al hardware si ya termino, que el hardware me avise
    mediante una interrupción. Un evento async que me libera
  - Ventajas: Eventos async poco frecuentes
  - Desventajas: Genera cambios de contexto impredecibles.
- **DMA** (acceso directo a memoria):
  - Con un poquito de ayuda del hardware.
  - Para transferir grandes volúmenes de datos del disco a memoria, en vez de
    hacer que la CPU lea de a un reg por vez, hay una unidad de HW que está
    conectada al bus que le decís "pasa esto de tal lugar del disco a tal otro
    de memoria". Cuando termines todo me avisás (interrupción)

Hoy en día siguen vigentes los 3 porque sirven en contextos distintos

- Drivers de las placas de red: Pasaron de push a un modo combinado. Cuando
  llega una interrupción por un rato hace polling (si llegó un dato,
  probablemente van a llegar muchos otros por un rato).

## Subsistema

Cómo funciona para el programador? Por lo general la API es la api POSIX

- `open()` / `close()`
- `read()` / `write()`
- `seek()`

POSIX: Estándariza los syscalls que se proveen para acceder a cosas que se
parecen a un archivo. Para trabajar con el hardware, el programador trabaja
como si fuese un archivo.

Los dispositivos se dividen en dos grupos

- Char device: Lees byte a byte. Mouse, teclado. No soportan acceso aleatorio
  y no utilizan cache.
- Block device: Podes leer de a más de un byte, se supone que hay mucho.
  podés leer los *próximos* o los de cierta posición.

`/dev`: Dispositivos que el SO se encarga de mantener

### Dialogo con estos dispositivos

- Al abrirlos, hay que especificar
  - lectura
  - escritura
  - lectura / escritura
- Son compartidos o dedicados
- Algunos brindan una comunicación sync y otros async.
  - sync: hago un read, me quedo bloqueado hasta que termine la lectura
  - async: "che quiero leer", y tenés otra función que te dice si está listo
    el resultado. Esto se puede manejar a nivel SO o a nivel proceso.

El SO tenía

- Propietario del HW, cuida el acceso al HW, lo administra
- API de programación: Se ve muy claro con E/S. Nos brinda una api homogenea
  y que encapsula la complejidad de los diferentes dispositivos.

### Planificación de E/S

Como hacemos que ande rápido?

- Necesitamos un buen driver, pero no alcanza solo con eso.

Nace la noción de *planificar* la entrada y salida, que es independiente del
driver.

Mover la cabeza toma tiempo, y queremos minimizarlo.
En vez de todo el tiempo hacer lo que te piden, los buffereas un poco y te fijas
si no podes leerlos en cierto orden, para minimizar el recorrido que tenés
que hacer. Pero cuanto espero? Hay distintas **políticas**.

#### Políticas

- El más simples es FIFO o FCFS (First Come, First Served)
- SSTF: Shortest seek time first

etc.

Algoritmo del ascensor: Espero un rato y la cabeza siempre va del fondo
hacia la punta, va acumulando pedidos y los va haciendo en el camino. Esto
suelen hacer y de forma empírica es lo que funciona mejor.

No es ideal.

## SSDs

El problema pasa de ser la lectura a la escritura. Las celdas no pueden
reescribirse una cantidad infinita de veces, se gastan. Parte del trabajo que
hace el SO y los drivers es asegurar no escribir siempre en el mismo lugar.
Porque sino terminas quemando algunas posiciones y no se pueden volver a usar.

## Gestión de discos

Filesystem: Como se organiza la información.

Dependiendo del SO, formatear y meter un file system puede llegar a ser lo
mismo o no. Estos bytes en este sector, tengo que leer y escribir

- **Formatear**: Cada byte dentro del disco en realidad tiene más bits de los
  que podemos leer. Lee esos bits y se los pasa al controlador. Como los medios
  físicos son corrompibles, se usan esos bytes para detectar errores.
  Funcionan como prefijo y postfijo a la parte donde efectivamente van los
  datos en cada sector. Me asegura que lo que yo lea sea lo que yo tengo que
  leer.

- **Booteo**: Arrancar el SO. La parte donde el SO se carga a memoria.
  Una computadora arranca con la RAM vacía. En la primer sección del disco,
  lee los bytes y pone en memoria y hace un jump a esa dirección. Es fija,
  corta y estándar. El *bootloader*.

  Muchas veces bootean en partes, el primer programa carga un segundo programa
  más largo, que va haciendo otras tareas.

- **Bloque dañado**: Al leer un sector el prefijo y postfijo no coinciden.
  Esa información de redundancia que yo agregué al leer veo que no está bien.
  Los SO a veces tienen un manejo de bloques dañados.
  Los SO "anotan" que un bloque está dañado.

## Spooling

Es una cuarta técnica de manejo de dispositivos, muy particular.

El caso típico es el de la impresora.

> Mandás a imprimir un documento de 30 paginas. El procesador de texto tiene
> una barrita de avance que dice cuanto imprimió, y capaz el procesador de texto
> piensa que terminó de imprimir pero la impresora sigue.

Vas al *spooler* de la impresora, y ahí si dice el estado real.

La idea sencilla es que es una cola de tareas armada por este pseudodispositivo.
A medida que la impresora real se va liberando, el spooler le va mandando.

El procesador de texto le manda por "socket", algo así como "/dev/printer" y
empieza a mandar los bytes ahí. En /dev/printer hay un programa esperando, y
cuando llegan los bytes agarra y los empieza a acumular. Y ese sí hace un open
de la impresora y va escribiendo en el hardware.

El spooler habla con el driver pero no es el driver.

El kernel no se entera que está haciendo spooling, pero el usuario sí.

Spool: *Simultaneus Peripheral Operation On-Line*

IPP internet printing protocol para las impresoras de red.

CUPS: Encapsula toda la complejidad y lo hace multi-impresora

## Locking

"Lock de los pobres"

POSIX garantiza que `open(..., O_CREAT | O_EXCL)` es atómico y crea el archivo
si no existe o falla si ya existe. Un lock de baja contención, en disco.
Para desbloquear tengo que borrar el archivo, lo cual es atómico siempre.

## Protección de la información

!! Tiene sentido proteger la información?

Que valor tiene?

- Cuanto vale para mi
- Que pasa si se pierde
- Que cosas no puedo hacer sin ella

Hay información que puedo recomputar. De acá sale la política de resguardo.

Tiene costo en tiempo y $$.

Estrategias

- MSSVR: Mirá si se va a romper.

### Backup (Copia de seguridad)

Hacer una copia de seguridad consiste en resguardar la información importante
en otro lado. Es fundamental.

Se suele hacer en cinta, o incluso en bibliotecas de cintas robotizadas.
Toma tiempo, y se suele programar a los sistemas para que lo hagan por la noche.

La principal ventaja de las cintas es que se pueden llevar a otro lugar. Si
pongo las cintas al lado de donde estoy, si se prende fuego en centro de computo
perdí la información igual. Cada tanto, las llevo a una sala cofre en otro lado.

Una vez cada cierto tiempo copiás todo al disco rígido

- Una vez al {mes|semana|etc.} copia total
- Todas las noches una copia *incremental*: solo los archivos modificados desde
  la última copia incremental.
  Ventaja: backup + rápido
  Desventaja: Tenés que aplicarlas de a 1. Si justo se rompió la del miercoles
  cagas

- Alternativamente, un backup *diferencial*, guardas las diferencias con
  respecto al backup total. Como un balance entre el total y el incremental.

### Redundancia

A veces no alcanza con una copia de seguridad. El costo de que el sistema salga
de línea es muy alto. Conviene implementar ***redundancia***.

RAID: Redundant Array of Inexpensive Disks.

Me genero un disco virtual muy resistente en base a muchos discos comunes.

**Mirror:** que la controladora escriba en dos discos a la vez, así si uno se
rompe pueda leer en otro. Genero un disco en base a varios.

- Si se rompe un disco no salís de linea.
- Problema: Costo: Pago dos pero uso 1

Sofisticaciones para hacerlo más barato y funcional.

Alternativa, manda a leer de los dos a la vez, tenés el doble de ancho de banda
de lectura. Más velocidad de lectura.

Niveles de Raid:

ver dibujos

- RAID 0
- RAID 1
- RAID 0+1
- RAID 2, 3, 4: no interesan
- RAID 5: 

TODO

- Virus que te borra el disco: No hay nada que hacer.
- RAID te protege de que se dañe físicamente el disco, no te protege de perdida
  de información. 
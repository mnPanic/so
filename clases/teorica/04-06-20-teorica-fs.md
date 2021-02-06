# Filesystems

Antes vimos como se guardan los datos en bruto

## Achivo

Secuencia de bytes sin ninguna estructura, una tira de bytes.

En otros SO tienen algun formato, headers, etc. Es responsabilidad de cada
programa interpretar esa tira de información

Tienen un nombre que los identifica, y podria tener una extension para
distinguir el tipo de contenido. Por lo geeneral es un recordatorio para los
humanos pero no tiene un significado para el SO.

## Filesystem

Hay un modulo del SO que entiende como estan almacenados los archivos en disco.

Algunos SOs modernos tienen uno por default, y te permiten elegir cual usar, ya
no es una caracteristica distintiva de los SOs.

Cada uno tiene sus pros y sus contras como siempre.

- Windows
  - fat
  - fat32
  - ntfs
- linux
  - ufs
  - ffs
  - etx2
  - etx3
  - etx4
  - XFS
  - RaiserFS
  - ZFS

También aparece la idea de acceder a archivos en otras computadoras como si
estuvieran en la mia. Como por ejemplo NFS

- NFS: Estoy en A, y quiero leer B como si fueran de la misma computadora.
- DFS: Es un filesystem "en la nube", yo te doy un nombre de archivo, y está en
  algun lado. Distribuido.

## Responsabilidad

Organización *logica* de los archivos. Como me propone como usuario organizar la
información.

Casi todos usan una estructura de tipo arbol.

### Links

Un archivo está en un directorio, pero se puede hacer una especie de alias en
otro lugar.

Es como un acceso directo. Una especie de puntero.

### Nombres

- Cada archivo deberia tener un nombre que indique como navegar el arbol para
  llegar a el, y tiene que haber un separador.

Ejs:

- Linux
  - `usr/local/etc/apache.conf`
  - `servidor4:/ejercicios/practica3.pdf`

- Win:
  - `C:\Program Files\Antivirus\Antivirus`
  - `\\SERVIDOR3\Parciales\parcial1.doc`

### Punto de montaje

La idea es que tenes tu grafo, y a partir de cierto directorio los nombres que
sigan de ahi, los lees de cierta unidad. Entonces el usuario del SO ve un solo
arbol pero distintas ramas del arbol hacen referencia a distintos dispositivos
fisicos.

```
mount

    Dice donde conectar una unidad
```

Unix representa todo de lo que haga falta hablar como un archivo en algun lugar

## Implementacion

- Como se representa un archivo?
- Como manejo el espacio libre?
- Que hago con los *metadatos* del archivo? (No los datos del archivo, sino los
  datos que describen al archivo.)

### Representacion de archivos

La E/S a los dispositivos no se hace a un byte, se hace de a bloques.

Para un FS el archivo es una lista de bloques + metadata.

- Naive: Todos los bloques de un mismo archivo los guardo uno al lado del otro.
  - Para leerlo es rapidisimo, con los bloques contiguos los lees de corrido
  - Pero a medida que empieza a modificarse el filesystem, se genera mucha
    fragmentacion
  - Va bien para un filesystem *inmutable*. Por ejemplo los viejos CDs (medios
    *read only*)
- Linked list: Representemos el archivo como una lista enlazada de bloques.
  - Cada bloque tiene los datos y un puntero que apunta al siguiente, la forma
    tipica en la que se hace en memoria.
  - Si quedaban contiguos es un desperdicio
  - Como lees? Es muy lento porque vas y venis de un bloque a la vez, y hay que
    intentar de leer varios bloques juntos de disco para aprovechar todo el
    tiempo que tarda el acceso al dispositivo
  - Tampoco sirve
- Tabla: Me guardo en una tabla que por cada bloque me dice en que bloque esta
  el siguiente elemento de la lista. Es una especie de indice.
  Necesito tambien otra informacion para saber donde arranca, pero una vez que
  la tengo ya esta.

  El chiste es que esta informacion la tengo en memoria y no en disco.

  Tiene que estar almacenada en disco. Pero la trabajo como un cache.
  Entonces si se corta la luz o algo, tenemos una inconsistencia en el FS,
  cuando no se llega a escribir. Queda una inconsistencia en el filesystem.

  Este metodo se llama **FAT** (conceptualmente) y **FAT32** tiene cachitos de
  tabla de 32 bits. (FAT: File Allocation Table)

  - Desventajas: Concurrencia y inconsistencias

- inodos: Como resuelve el mismo problema UNIX?
  
  i de indice (nodos de indice)

  Cada archivo tiene un inodo, el cual esta compuesto por
  - Al principio tienen la metadata
  - Luego, tienen punteros a los nodos que contienen los datos.

  Siempre hay una lectura adicional para leer el inodo del archivo.
  
  - Se resuelve el problema de concurrencia porque cada proceso tiene abierto su
    propio inodo. Esto para archivos distintos

  Pero que pasa si se acaba el tamaño? Ya que el inodo tiene espacio para
  punteros finitos.

  imagen

  - Si el archivo es suficientemente chico, tenes punteros a nodos directos.
    Cubre hasta 8KB
  - Sino, tenes un single indirect, un puntero a un inodo. Pagas una segunda
    lectura adicional. Cubre hasta 16MB
  - Si es aun mas grande, tenes un nodo de doble indireccion, que te lleva a un
    inodo de todos punteros dobles. Cubre hasta 32GB
  - Y cuando es aun mas grande se agrega un nivel triple. Cubre hasta 70TB

  Sigue habiendo un tamaño max pero es mucho mas grande.

  Si se corta la luz, no pongo en riesgo todo el FS; sino solo la parte de los
  archivos que estan abiertos en ese momento. Minimiza el riesgo.

con `ls -i` te deja ver el numero de inodo.

### Representacion de directorios

Un directorio es un archivo que tiene un metadatao que dice "esto es un
directorio

### Link simbolico

```bash
ln -s
```

Ahi ves explicitamente una flechita. No comparten el inodo

En el inodo cuyo contenido dice "che, anda a leer el archivo al cual estoy
linkeado"

Es una manera de apuntar a algo exista o no, porque solo se ve el nombre.

El principal uso es hacer un link a un archivo que esta en otro file system.
Si no te deja hacer un link normal porque pertenece a un inodo de otro file
system, pero si se puede hacer el link normal

## Manejo de espacio libre

no es tan importante pero lo leemos

## Cache

Se relaciona con como se hace el acceso a disco.

## Consistencia

Los datos se pueden perder, y los SOs brindan `fsync` que fuerza a escribir las
cosas de la cache, es un *flush* (pero no limpia el cache, entonces no es un
flush).

Las BDs graban en disco la informacion con fsync.

### Verificacion

Que pasa si se rompe el FS? Los SOs de tipo unix tienen un comando
*file system check* (`fsck`). Se corre de manera automatica.

Cuando se ejecuta esa syscall, se graba un bit que dice "esta sincronizado", y
cuando se graba la primera pagina en el cache que todavia no se bajo, se apaga
ese bit. Cuando levanta el SO, si tiene ese bit apagado (i.e no esta sync) corre
este comando o le indica al usuario que lo tiene que correr.

Hace una serie de chequeos que nacen del *invariante* de los inodos. Lo restaura

- Chequea que la cantidad de links coincidan
- Chequea que los bloques que estan marcados como libres no esten apuntados por
  ningun inodo como dato
- Chequea que los inodos que dice el tamaño de un archivo coincida con los
  bloques que son apuntados con el inodo
- ...

Toma mucho tiempo porque puede requerir recorrer todo el disco rígido, entonces
ya no se hace así.

### Alternativas

Evitarlo de manera parcial o total.

- **Soft updates**: Yo cacheo bloques de datos, pero la metadata trato de
  grabarla casi siempre. Deja de ser binario si esta en sync o no.
  La ventaja es que permite que el recorrido para verificar el invariante se
  pueda hacer con el sistema funcionando.

  El sistema arranca, esta un poco mas lento porque corre un proceso, pero no te
  quedas esperando un rato largo a que el sistema levante.

  No es la linea evolutiva ganadora

- **journaling**: Algunos FS llevan un *log* o *journal* de las operaciones de
  escritura. Dicen "che habria que escribir tal cambio en tal parte del disco".
  Reservan una parte del disco de tamaño fijo que se escribe todo el tiempo. Es
  una especie de buffer circular. Cuando se llena, todas las operaciones que
  estan ahi se van realizando.

  Tengo marcadas las operaciones que todavia no realice.

  No toma tanto tiempo escribir porque se hace de manera resumida en un lugar
  fijo. Entonces no uso el cache al 100%, un cachito si escribo a disco pero si
  tengo que levantar levanto mucho más rápido, ya que agarra este log y lo
  aplica.

## Caracteristicas avanzadas

- Cuotas de disco: Cuanto espacio de disco puede usar cada usuario
  - Puede ser dificil deimplementar

- Encripción: https://xkcd.com/538/
- Snapshot: Es una foto en un determinado tiempo del disco. Y lo hace super
  rapido. Pero no hace una copia, se guarda que querias hacer una copia, y a
  partir de este momento, guarda cada modificacion que se hace. Es como un copy
  on write.

## Rendimiento

Que factores lo impactan

- Tecnologia de disco (HDD vs SSD)
- Politica de scheduling E/S
- Tamaños de bloques
- Caches del SO
- Caches de las controladores
- Manejo de locking en kernel
- FS

Siempre está el balance entre performace y mantenibilidad / robustez.

- Bien hecho, auditable
- Garantias por si se corta la luz

# Clase de file systems

De videos de rodo

## Ej 1

[Link](https://docs.google.com/spreadsheets/d/1sAmoAnWv4BJUO5MNk4vThHulSSpIOvgt1Ga3qVs5Lt8/edit#gid=0)

## Ej 2

Se tiene un disco con capacidad de 128 GB, y bloques de 8 KB.
Supongamos un sistema de archivos similar a FAT, donde la tabla se
ubica desde la posición 0, y cada entrada de la tabla es de 24 B.

1. Que tamaño ocupa la tabla?

   1 K = 2^10
   Hay
    128GB / 8KB = 128 * 2^30/ 8 * 2^10
        = 16 * 2^20
        = 2^24
        = 16 M bloques

    Como hay una entrada por bloque, pesa 16*24 MB = 384 MB

2. Cuantos archivos de 10MB es posible almacenar?

    Cada archivo ocuparia
    El espacio libre es la cantidad de bloques menos lo que ocupa la tabla

    El tamaño en MB del disco es 128*2^10 MB

    Entonces entran

        (128*2^10 MB - 384MB) / 10MB = 13068 archivos de 10MB

3. Se sabe que un archivo comienza en el bloque 20. Dada la
siguiente FAT, indicar el tamaño de dicho archivo.

Bloque      0   1  2 3 4 5 6 ... 20 21 22 ...
Siguiente   EOF 2 23 4 5 0 7 ... 21 22  3 ...

20 21 22 3 4 5 0 EOF
7 bloques, cada uno 8KB => [49, 56]KB ya que el ultimo puede no estar lleno.

## Ej 3

Un sistema de archivos de UNIX, tipo ext2, tiene bloques de tamaño
4 KB y el direccionamiento a bloques de disco (LBA) es de 8 bytes. A
su vez, cada inodo cuenta con 5 entradas directas, dos indirectas y
una doblemente indirecta.

Direccionamiento a bloques de disco = logical block address = LBA
El tamano dentro del inodo que ocupa cada puntero son 8 bytes

    Inodo
    dir 1
    dir 2
    dir 3
    dir 4
    dir 5
    indir 1             son bloques que comparten el area de datos
    indor 2
    doble indir 1

1. ¿Cuál es el tamaño máximo de archivo que soporta?

Veo la cantidad maxima de bloques que puede almacenar
Cuantos apuntadores entran en uno bloque de datos?

    4KB / 8B = 2^12 / 2^3 = 2^9 bloques


    5 +             directo 
    2*2^9           2 indirectos
    2^9*2^9         doble indir
    = 263173 bloques

263173 * 4KB = 1052692 KB = 1 GB (mas o menos)

2. Si el 50% del espacio en disco es ocupado por archivos de 2 KB,
el 25% por archivos de 4 KB y el 25% restante por archivos de 8
KB, ¿qué porcentaje del espacio en disco es desperdiciado?
(Considere solo el espacio utilizado en los bloques de datos).

8kb = 2 bloques de datos 100% ocupados
4kb = 1 bloque de datos 100% ocupados
2kb = 1 bloque de datos 50% ocupado

el 25% del disco esta desperdiciado

1. ¿Cuántos bloques hace falta acceder para procesar completo un
archivo de 5 MB?

cuantos bloques tiene?

    5MB / 4KB 
        = 5 * 2^20 / 2^12 
        = 5*2^8 bloques
        = 1280 bloques

Hay que acceder a

    5 * 2^9     todos los bloques del archivo
                los primeros 5 estan en los directos
    + 1         el bloque que contiene al inodo del archivo
    + 1         el 1er bloque de indireccion simple (2^9)
    + 1         el 2do bloque de indireccion simple (2^9)
    + 1         doble
    + 1         simple en doble (2^9)

= 5 * 2^8 + 5 = 1285

## Ej 4

- ¿Qué ventajas tiene un sistema que utiliza inodos sobre uno de
tipo FAT?

El acceso es mucho mas rapido

No es necesario tener la tabla de todos los inodos en memoria a la vez. Y cada
archivo tiene la suya, lo que lo hace mas robusto. Si se corrompe un inodo, solo
se pierde el archivo correspondiente

Se evita tener una unica estructura compartida por todos los archivos. Cada
proceso solo necesita bloquear los inodos de los archivos que esta usando en un
momento dado.

- ¿Qué ventajas tiene un sistema que utiliza FAT sobre uno que usa inodos?

Para unidades pequenas FAT es muy performante y se puede implementar mas facil.

Es soportado por la mayoria de los SOs

## Ejercicios clase

(TODO)

https://campus.exactas.uba.ar/pluginfile.php/196155/mod_resource/content/1/ejerciciosFS.pdf
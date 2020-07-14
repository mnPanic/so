# Practica 5 - File Systems

## Ej 1

Para llegar al ultimo bloque, de un archivo de N, hay que acceder a la FAT.

Suponiendo que se sabe el bloque inicial del archivo, hay que acceder N + 1 veces
al disco, a la fat y luego al siguiente bloque en memoria

## Ej 2

Inodos y bloques de 4KB

a. archivo de 40KB, cuantos bloques es necesario leer para procesarlo
   completamente?

   Tiene 10 bloques, entonces basta con leer los 10 directos dentro del inodo
   (11 lecturas)

b. 80KB

    Tiene 20 bloques, entonces es necesario leer
    
    - los primeros 12 del inodo directo (12 + 1)
    - Los 8 restantes del de indireccion simple, que a su vez es un bloque tambien
        (8 + 1)

## Ej 3

1. Inodos porque en fat no se puede
2. FAT porque inodos tiene muchas estructuras auxiliares
3. FAT, porque en inodos hay un hard cap (aunque alto) que depende de la
   cantidad de indirecciones. De haber mas, estaria limitado por el tamaño del
   disco solamente.
4. En FAT es siempre la misma, y en inodos es O(n) asi que cualquiera de las dos

## Ej 4

todo

--------

Parte 2

## Ej 5

RAID 1 mejor rendimiento para lectura que RAID 0?

Si, porque en raid 1 se pueden leer bloques diferentes de forma concurrente en
ambos discos, mientras que en raid 0 si caen en el mismo disco no.

## Ej 6

- Escritura en un bloque
  Dos, el bloque y el de paridad del stripe.

- 14, porque hay que recalcular los de paridad.

## Ej 7

- total: 1 sola cinta
- diferencial: 2 cintas
- incremental: de 2 a 5, dependiendo de cual sea la periodicidad de los
  incrementos

## Ej 8

- Raid no protege contra borrar o modificar un archivo accidentalmente
  Se combina con copias de seguridad.

- si

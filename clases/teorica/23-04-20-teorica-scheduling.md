# Scheduling

El sistema operativo alterna entre los procesos que ejecuta.
A cada uno se lo ejecuta durante un *quantum*. Y cuanto dura? Acá interviene el
**scheduler**, es el modulo que implementa la *politica de scheduling*

- Cuanto el doy a cada proceso
- Cual es el proceso que sigue

Son centrales y hacen al rendimiento de un sistema operativo.

Es tan importante que hay SOs que vienen con más de una. Dependiendo de la
tarea a realizar brindan diferentes schedulers.

## Objetivos

Por qué no hay un único ganador?

Cosas a optimizar

- **Fairness**: Ecuanimidad, que cada proceso reciba una dosis "justa" del CPU.
  Cada proceso es tratado de la manera mas justa posible. Pero que es justa?
  Es darle a cada uno lo mismo de CPU? O hay que ser fair con respecto a los
  usuarios? Por ej. si hay un usuario que lanzó un montón de procesos.

- **Eficiencia**: Tratar de que la CPU esté ocupada todo el tiempo.
- **Carga del sistema**: Minimizar la cantidad de procesos listos que están
  esperando el CPU.

- **Tiempo de respuesta**: que los *usuarios* perciban que el sistema es rápido.
  Minimizar el tiempo percibido por los usuarios interactivos.

  Por ejemplo, no darle tanta prioridad a un proceso que esté viendo si llegó
  un mail (proceso *batch*).

- **Latencia**: Minimizar tiempo para que un proceso empiece a dar resultados.
- **Tiempo de ejecución**: Minimizar el tiempo total que le toma a un proceso
  ejecutar completamente.
- **Throughput**: Rendimiento, maximizar el nro de procseos terminados
  por unidad de tiempo.
- **Liberación de recursos**: Hacer que terminen cuanto antes los procesos
  que tienen reservados más recursos. Estás beneficiando a los procesos más
  golosos, mientras más cosas acapara, más lo beneficias con la esperanza
  de que los libere.

Muchos son contradictorios, entonces lo primero que hay que definir es *que*
queremos optimizar, lo cual depende de a que uso queremos someter nuestro SO.

no se puede tener ICI20yIMdH: la chancha los 20 y la maquina de hacer chorizos

!!
Cada política de scheduling busca maximizar un objetivo.

## Tipos

El scheduling puede ser *cooperativo* o *con desalojo*

- **cooperativo**: Se daba en los primeros SOs. El razonamiento era: en algún
  momento todos los procesos van a tener que hacer una syscall, entonces cuando
  la hagan nos fijamos de cambiar de proceso.

  Pero es medio peligroso, porque un proceso puede poner

  ```c
  while(true) {}
  ```

  y nunca llama a una syscall, el programa se cuelga y no recuperás más el
  control.

- **con desalojo** (apropiativo o *preemptive*): además de ese mecanismo,
  te colgás del clock para saber cuando se le pasa el quantum

  Se carga en la tabla de interupciones, y se hookea eso.

Pero para un SO con propósito específico tal vez conviene no meterle un clock
con interrupciones.

No da garantías de continuidad a los procesos, puede ser un problema para los
SO de *tiempo real*: en la que la cantidad de tiempo que toman sus acciones
está acotada. El ejemplo menos crítico es un sistema digital de música, por ej
un piano en el que tocás y la duración de una nota no es siempre la misma.

Cuando hay multitarea cooperativa, a veces se proveen llamadas explícitas para
que el kernel desaloje.

## Políticas

El scheduling tiene la dificultad de que en poca cantidad de instrucciones y
poco tiempo de reloj tengo que definir a quien le toque, pero también tengo que
tomar una decisión *sofisticada* sobre a quien le toca.

### FIFO (First in first out)

FCFS (Fist came, first served). Es la mais naive. Como una cola de supermercado.
In al estado *ready* (procesos que lo único que necesitan para ejecutar es la
CPU) de la máquina de estados.

En el mismo orden en el que entran los pongo a ejecutar.

- Problema: Procesos que requieren mucho CPU traban a los demás.

Entonces le podemos agregar *prioridades* al modelo. (por ej. la guardia
del hospital)

- !! Problema: **starvation** o inanición, los procesos de mayor prioridad
  demoran infititamente a los de menor prioridad que nunca se ejecutan.
  > Una persona se desangra por una cortadura de dedo, ya que no paraban
  > de llegar infartados

Una posibilidad de solucionarlo es aumentar su prioridad según su "vejez".

!! **Cualquiera esquema de prioridades fijas corre riesgo de inanición**

### Round Robin

Darle un *quantum* a cada proceso, e ir alternando entre ellos. Es como
*la calesita*.

**Cuanto dura el quantum?**

- Si es muy largo, en SO interactivos podría parecer que el sistema no responde
- Si es muy corto, el tiempo de scheduling+context switch se vuelve una
  proporción importante del quantum. El sistema pasa un % alto del tiempo
  haciendo *mantenimiento* en lugar de trabajo de verdad.

Tengo que encontrar un equilibrio.

Se lo suele combinar con prioridades, que pueden estar dadas por el tipo de
usuario (administrativas, el administrador tiene que hacer algo en beneficio
del sistema) o pueden ser "decididas" por el proceso, lo cual no suele
funcionar.

Estas van decreciendo a medida que los procesos reciben su quantum para evitar
starvation de los que tienen menos.

Los procesos que hacen E/S reciben crédito extra porque son buenos compañeros.
Les da "premios" al pedir entrada y salida.

!! La prioridad en estos casos es **dinámica**

### Multiples colas

En vez de tener un único estado de procesos listos, puede haber varios tipos

Colas con 1, 2, 4, 8 quanta (prural de quantum) c/u

A la hora de elegir un proceso la prioridad la tiene siempre la cola con
menos quanta.

Por ej: los batch van en el 8

Cuando a un proceso no le alcanza su cuota de CPU, es pasado a la cola siguiente
lo que disminuye su prioridad, pero le asigna más tiempo de CPU en el próximo
turno.

Los procesos de máxima prioridad, los interactivos en general, van a la cola
de máxima prioridad.

La idea en general es minimizar el tiempo de respuesta para los procesos
interactivos, suponiendo que los cómputos largos son menos sensibles a demoras.

### SJF (Shortest Job First)

El trabajo mas corto primero consiste en tratar de terminar la mayor cantidad
de trabajos por unidad de tiempo.

Está orientada a maximizar el throughput.
Acá si procesos batch en el sentido más tradicional del termino.

Para usarlo, debería saber masomenos cuanto duran o por lo menos poder
aproximarlo. Si las conozco, es *óptimo*.

No es muy aplicable porque por lo general no se sabe.

### EDF: Tiempo real

Se usan en entornos críticos, en los cuales si el deadline de un proceso no se
cumple pasan cosas malas.

A veces se utiliza una política que es el que está mas cercano a su deadline.

Una posible es EDF: Earliest Deadline First.

## Sistemas SMP

Multiprocesador. Hoy en día son *todos* multiprocesador.

Scheduling en SMP es también un problema distinto.

Un problema es la *cache*, los procesadores tienen una jerarquía de cache.

> Tengo dos procesadores A y B, y un proceso se venía ejecutando mucho en A,
> entonces tiene el caché preparado. Si se libera B, y A está ocupado, mando
> al proceso a B? o espero a que se libere A?

Entonces cada proceso tiene una *afinidad* con el procesador. Ver en que proceso
venía ejecutando e intentar de seguir ahí.

Puede ser *afinidad dura*: si o si en el mismo. O *blanda*: voy a intentar, pero
a veces voy a cambiar, caso en el cual *todos* se ven afectados por la
**invalidación** del caché

### Distribuir

- Push migration: El procesador ocupado revolea procesos a los otros
- Pull migration: El procesador que está libre avisa a los demás que está
  tranquilo así le pasan procesos.

### NUMA (Non Uniform Memory Access)

Hay bancos de memoria que están más cerca de ciertas CPU que otras.

[img diapo 13]

Cada CPU puede acceder a todos los bancos de memoria pero no con la misma
velocidad.

A la hora de hacer scheduling, si estoy en una arquitectura numa, no solo tengo
que tener afinidad por caché sino por memoria.

## En la práctica

- Elegir uno que funcione bien es muy dificil
- A veces se arman modelos matemáticos basados en *teoría de colas*
- Se prueban con patrones de carga tomados de sistemas concretos o
  benchmarks estandarizados, por ejemplo, ver los patrones de uso de una base
  de datos.

### Pistas

- Si un proceso abre una terminal, muy probablemente está por convertirse en
  interactivo

### Micro vs macro

Micro vs macro benchmarkings.

- micro: que cada operación aislada funcione lo más rápido posible
- macro: Lo que se quiere es que ande mejor el SO en general, basado en patrones
  de uso

## Tarea

Ver los libros, en especial para los detalles numéricos.

## Repaso

- Consideraciones que hay que tener en cuanta a la hora de hacer un algoritmo
  de scheduling.
- Analizamos los algoritmos más comunes.
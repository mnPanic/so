# Sincronización entre procesos 2 - Razonamiento y problemas clásicos

## Repaso anterior

TAS Es una operación de user space que no requiere cambio de contexto, mientras
los semáforos si lo requieren. Son pesados, difíciles de implementar y tienen
mucho overhead.

En contraparte, TAS, a pesar de hacer busy waiting, es muy liviana.

Si sabés que vas a dormir mucho tiempo, te conviene pagar un precio caro para
ir a dormir (semáforo), pero si vas a dormir poco, capaz no te conviene el
overhead y es mejor hacer un TAS.

## Intro

Cuando uno estudia un problema clásico lo hace porque de alguna manera repr
una familia de problemas mucho más amplia. Dan herramientas con un alcance
más amplio.

Vamos a ver problemas clasicos del paralelísmo

## Turnos

Hay i = 0, ... n-1 procesos ejecutando.

Cada proceso ejecuta una tarea muy fácil, como imprimir "soy el proceso i"

- Queremos que se impriman en orden.
- No queremos que se ejecuten en desorden. Si solo printean nada garantiza
  el orden de ejecución.

> que una serie de procesos distribuidos se ejecute en un orden específico

## Razonar sobre un programa paralelo

No nos alcanza con decir que de la Pre se llega a la Post, porque tiene más de
una ejecución posible (ṕara cualquier fixture (scheduling posible)se tiene que cumplir)

Una ejecución es

    T = T_0 -> T_1 ...

Donde los T_i son los diferentes estados del programa.

> no vamos a demostrar pero si argumentar que se cumplen esas propiedades

Ahora la noción de correcto se reemplaza por una serie de propiedades, que se
tienen que cumplir para cualquier scheduling posible.

Entran en dos grandes familias: **safety** y **liveness**

- **safety**: Nos hablan de que las cosas malas no suceden. (ej: el programa no
  tiene deadlock)
  Por ejemplo, un programa que no hace nada no tiene deadlock

- **liveness** (o progreso): En algún momento algo bueno sucede. Cumple su
  objetivo

### Contraejemplo

Es una sucesión de pasos que muestra una ejecución del sistema que viola
cierta propiedad.

### Safety

la intuición es que *nada malo sucede*, pero la definición formal es que tienen
un contraejemplo finito. Es decir, que puedo mostrar que la cago en una cantidad
finita de pasos.

Ejemplos:

- No hay deadlock
- Exclusión mutua
- No pérdida de mensajes
- "Los relojes nunca están mas de N unidades desincronizados"

### Liveness

Intuición: Algo bueno en algun momento sucede

Definición: Los contraejemplos no son finitos.

Ejemplos

- Cada vez que se recibe un estímulo el sistema responde

No te puedo demostrar que una X cantidad de pasos muere, sino que en base a la
manera en la que funciona el sistema, entra en un loop en el cual lo que estás
buscando no sucede.

### Fairness

O equanimidad. Tercera propiedad. Más que probarla uno la *demanda*, la pone de
precondición para poder probar otras propiedades (por lo general, liveness)

Te voy a probar que mi sistema hace cosas buenas siempre y cuando tiene una
ejecución *fair*. A veces es imposible probarlo para *cualquier* scheduling.

Intuición:

La ejecución de los procesos fue ecuanime? El scheduler le dio su turno a un
proceso o lo invocó? O en el pero caso hubo inanición? O un caso menos extremo,
los procesos del 5 al 10 se ejecutaron 1000 veces y del 1 al 4 5 veces. Eso
sería un scheduling injusto.

- Incondicional
- Fuerte
- Debil

Intuición más débil: "No se van a dar escenarios poco realistas donde alguien
es postergado para siempre", o "masomenos anda bien"

### Practica

En la práctica se suelen usar versiones acotdas de estas propiedades
"el sistema responde en a lo sumo X unidades de tiempo"

Para trabajar con estas props se inventaron *logicas temporales*, que permiten
incorporar la temporalidad a la lógica.

- LTL: menos poderosa pero computacionalmente es más fácil y la intución es
  bastante sencilla.
- CTL
- TCTL
- ITL

## LTL

```
p OR q
p AND q
```

Y todo lo proposicional

```
T |= F?             los estados t satisfacen F

F = p
F = F1 AND F2
F = F1 OR F2
F = [] F1           en todos los estados vale

Por ejemplo,

[] NOT(deadlock)    en ningun estado de mi sistema hay un deadlock

F = <> F1           en algun momento del futuro vale F1

<> estado == final  en algun momento llego al estado final.

[] (pedido => <> respuesta)     tipica propiedad de liveness
```

Herramientas

- Theorem provers (hacen las manipulaciones algebraicas)
- Model checkers (fuerza bruta)

## Back to turnos

```
TURNOS: los s_i ejecutan en orden: s_0, s_1, ..., s_n-1
```

"No se da una ejecución en desorden"

Es de safety

## Rendevouz

Punto de encuentro o barrera de sincronización

Cada Pi,j tiene que ejecutar a(i); b(i)

BARRERA: b(j) se ejecuta después de **todos** los a(i)

No hay que imponer ningun orden entre los a(i) y b(i)

N. Lynch 

todo proceso lo podemos pensar como que de alguna manera tiene estas etapas,
una sección crítica y una parte que es después de la sección crítica, y una
parte que intenta acceder a la sección crítica.
Además, hay otra parte del código que no nosi mporta

{diagrama diapo 18]

### WAIT-FREEDOM

Prop WAIT-FREEDOM

"Todo proceso que intenta acceder a la sección critica, en algún momento lo
logra, cada vez que lo intenta."

prop diapo 19

"para todo ejecucion, todo paso, todo proceso, si esta en un estado try,
existe un estado posterior que esta en la sección crítica"

Intuición: "libre de procesos que esperen *para siemore*"

Lamentablemente es mucho pedir, no siempre se puede cumplir. Serie de props mas
más laxa

### Fairness

diapo 22

### Exclusión mutua

Para toda ejecución y todo estado, no puede haber más de un proceso que esté
dentro de la sección crítica

    EXCL = [] #CRIT <= 1

### Progreso

    LOCK-FREEDOM = [] (#TRY >= 1 ^ #CRIT = 0 => <>#CRIT>0)

Si hay alguien que esta queriendo entrar y la seccion critica esta vacia, en
algun momento alguien va a entrar

### Auxiliares

- Lograr entrar "el sueño del pibe"

IN(i) = i \in TRY => <> i \in CRIT

- Logra salir

OUT(i) = i \ in ..

En algunm momento logra salir

### Progreso global dependiente

STARVATION-
LOCKOUT-
STARTVATION-FREEDOM

    STARTVATION-FREEDOM = V i [] OUT(i) => V i [] IN(i)

Si todos salen, todos entran en algun momento.

    WAIT-FREEDOM = V i [] IN(i)

a veces no se puede probar

- Mi algoritmo no lo cumple. El proceso puede morir o cosa asi
- Hay dificultad de demostración, o argumentación que pruebe WAIT-FREEDOM,
  a pesar de que creemos que lo cumple, no lo podemos demostrar, entonces
  probamos algo más débil.


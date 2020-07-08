# Practica 7 - Proteccion y seguridad

## 7.4

a.

Cuando se ejecuta login, se crea el *stack frame*, donde lo primero que aparece
es la return address. Y las variables van apareciendo en el stack, en el orden
inverso en el que son declaradas.

![stack](img/stack-convention.png)

de direcciones bajas a altas

```
user.name
user.pass
real_pass
EBP
RET
```

Eso es porque la estructura user es una sola variable, entonces queda ordenada.

Mirando el diagrama, veamos que datos controla el usuario a traves de fgets

A fgets le pasamos una dir de memoria. Le damos el control de esa dir mas la
cant de bytes que le pasaste en el tamaño

```
fgets(user.name y sizeof(user) (64 bytes))
```

Tiene acceso a la pass. Lo cual esta mal, porque a lo sumo es su contraseña.

En la segunda, controla user.pass y realpass

b. Como lo explotamos?

Basta con escribir cualquier cadena de 32 char repetida 2 veces para que esa sea
la real pass.

Esto es un **buffer overflow**.

Para arreglarlo, basta con darle menos espacio, lo mas prolijo seria poner

```
sizeof(user.name)
sizeof(user.pass)
```

### 7.7

Stack randomization. no puedo usar valores de una corrida para romper otra.

a. da igual porque las direcciones son relativas
> rosica: Si un adversario tiene permitido escribir sobre un buffer que no verifica límites, puede sobreescribir el EIP de retorno sin siquiera saber dónde está ubicada la pila del proceso.

El buffer esta en stack, y la func tiene acceso al stack, y no importa donde
esta, lo ve y lo tiene a su alcance.

b. yo controlo el buffer en stack y hay buffer overflow. No solamente retorna a
un lugar que yo manejo, sino al codigo del buffer.

La direccion de retorno es absoluta, entonces no podemos poner una direccion.
Stack randomization te complica las cosas

c. no sirve, las syscall tienen una func wrapper parte de la libc. queremos
saltar a libc. Por lo general, si son usadas en un programa pueden estar
cargadas de forma estatica (compilaron contra libc, entonces forma parte de
text) o dinamica (cuando se carga).

En ambos casos esta en un cacho de memoria cuya posicion no se randomiza.

Puedo ejecutar el programa, inspeccionar la ejecucion, ver donde esta cargada, y
despues usar esa info para saltar a ese lugar.

### 7.8

Es peligroso llamar a `system` sobre algo sobre lo que el usuario tiene mucho
control. Le paso cualquier cosa y system lo ejecuta.

a. ej

```text
dir = "&& cat /etc/passwd"      # condiciona la ejecucion del 2do al ok del 1ro
      "; cat /etc/passwd"       # equivalente a ejecutar uno y desps el otro
```

b.

Lo que ejecuta system es

```text
ls \" {...} \"
```

La intencion es que cualquier otro comando queda dentro de las comillas,
entonces es interpretado como input de ls. Podria manipular el input para
que lo que se ejecute sea

`ls \" ." {...} " \"`

c. 

Tampoco porque se puede usar `&&`

d. Hay que tener cuidado cuando uno sanitiza parametros, y no suele ser un
buen approach ir prohibiendo cosas. Es muy facil olvidarse de prohibir algo mas.

- usar `exec` con `ls`
- bastaria tambien con prohibir que cierre las comillas

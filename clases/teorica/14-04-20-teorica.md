# Primera clase teorica

martes 14-04-2020, teorica

## Bibliografía

Esperan que complementemos los temas con alguno de estos dos libros para el
grueso de la materia

- Tanenbaum: Escrito por una sola persona, muy ameno de leer, donde él opina
  y da sus opiniones en primera persona
- Silberschatz: Es por 3 autores, es más impersonal y mas profundo, completo.
  Pretende ser más enciclopedico.

Ambos cubren casi todos los temas, y para cubrir todos se necesitan ambos, y no
necesariametne uno es mejor que otro.

## Diapositivas

No son autocontenidas, tomemos apuntes anotando los slides.

## Divisón de sistemas informativos

- Hardware
- Software **específico**

Hay un intermediario: el SO, que cumple dos roles.

- para que el soft especifico no se tenga que preocupar por los detalles de
  bajo nivel, del hardware. (visión del usuario - programador).
  Abstrae la complejidad del hardware
- Para que el usuario use correctamente el hardware, como propietarios del
  hardware. Que no lo abuse

## Historia

Para entender la segunda perspectiva.

[diapo 7]

Batch: Cargo un lote de tarjetas una cinta, y se procesa todo junto.

El opuesto es interactivo, le das una instrucción y vuelve.

Primera preocupación para los SO: cada usuario debe obtener solo su resultado
y no el de los demás.

Se dice biblioteca y no librería, se origina de que se guardaban las cajas de
zapatos que tenían las funciones en bibliotecas.

### OS/360

The mythical man month: Diario de brooks de OS/360
Recomendación de lectura de chapa

idea: paralelizar programa cargado y uno que se esta cargando.
Para no tener downtime durante las cargas.

### Nuevos conceptos y problemas

- **multiprogramación**: tenemos varios programas en memoria a la vez.
  El *throughput* o *rendimiento* aumenta.

  Un trabajo `j1` toma el mismo tiempo que antes, o más, pero `j1 + j2` tarda
  menos. El tiempo total de ejecución de dos programas baja.

  La computadora no es mas rápida, ya que los programas individualmente no se
  ejecutan más rápido, pero tiene mejor *rendimiento*

  Ejecutan uno después de otro.

Y nace un nuevo problema

- **contención**: Antes había un solo programa, y era amo y señor de la
  computadora, toda a su disposición. Ahora hay dos programas que están a la vez
  y hay dispositivos únicos, por los cuales hay competencia o *contención*, la
  necesidad de acceder desde más de un lugar a la misma cosa.

  Hay que buscar una forma de resolverla de manera

  a. Correcta
  b. Eficiente

### Timesharing

En los 70' aparecen el monitor y el teclado de manera combinada en *la terminal*.
Este era un **socotroco** del tamaño de dos microondas que era como una tele
con un teclado.

En vez de hacer que la computadora tenga un único casble, que se le puedan
conectar varias *terminales* a la vez, el extremo de input y output de la
computadora.

Esto se le llama **timesharing**, es una evolución de la *multiprogramación*,
que no haya solo varios programas a la vez, sino que vayamos alternando.
No todos los usuarios van a estar tipeando a la vez, en el tiempo entre tecla y
tecla y eso, ejecuto el programa de otro usuario.

Acá realmente había contención.

**Cuanto tiempo se le daba a cada usuario?** Es una buena pregunta.
Era arbitrario y tenía muchas oportunidades de optimización.

Todos los SOs de hoy en día son timeshared, y una manera central en la que
se diferencian es cuanto tiempo le dan a cada programa. Por lo general es
variable.

El primer SO que tiene esto es MULTICS, del que desciende UNIX (Ken Thomson y
Dennis Ritchie). De esto salen *muchos* sistemas operativos modernos.

Todo desciende de Unix. Tienen muchas herramientas de autodiagnóstico.
Por ejemplo ver todos los procesos que corren a la vez.ç

## Software

Cómo hacés en assembler para leer de un diskette?

- Mando al controlador de la disketera muchos parámetros
- Se activa un motor que hace girar el disco, mueve la cabecera, lee el dato
  y lo pone en un registro.

> Tengo que saber cuanto tarda

- El HW no es amigable.

Pero leer un archivo desde un lenguaje de alto nivel es muy amigable.
*que bueno que es tener al SO en el medio para zafar del quilombo*.

## SO

Un sistema operativo es un intermediario entre el hardware y los programas de
usuario.

- Tiene que manejar la concurrencia (programas a la vez) y contención (mismos
  recursos)

Y

- Hacerlo con buen rendimiento
- Que funcione correctamente

**Este es el problema central de todo sistema operativo**.

Para lograr corre esto corre en nivel de *privilegio 0*, es decir,
*máximo privilegio*. Tiene control de toda la computadora, y va regulando que
hacen el resto de los programas.

Es un *programa privilegiado*

### Partes de un SO

#### Herramientas

Hay herramientas que no son partes del soft que corre con privilegio 0, como
la calculadora, el administrador de impresión, el administrador de tareas, etc.
son programas cualquiera para el SO.

Es el software que emula la cosa grandota que antes permitia dialogar directo
con el sistema operativo es la **terminal**.

#### Kernel

Sistema operativo es una palabra que cambia.

El **kernel** es el que hace de administrador y corre con nivel 0. Todo lo que
vamos a discutir a lo largo de la materia tiene que ver con los diseños de los
distintos kernels (o *nucleo*). Expone distintas interfaces para que los
programas hablen con ellos.

#### Drivers

Pedacitos del kernel que hablan con distintos tipos de hardware específico.

Dos usuarios pueden tener la misma versión del OS pero drivers distintos porque
tienen hardware diferente.

#### Shell

Es lo que uno corre desde la terminal, pero también la interfaz de usuario.
El programa en el que uno presiona con iconitos, es un programa común y
corriente que permite poner a correr otros programas.

Cuando una computadora se enciende, tiene que levantar algún tipo de shell para
poder interactuar con ella.

Un *sistema embebido* por ejemplo no requiere tener una shell.

Ejemplos de shell en Unix:

- bash
- sh
- csh
- ksh

En windows

- explorer.exe (shell gráfico desde el termino mas amplio)
- cmd.exe
- powershell.exe

#### Proceso

Un programa en ejecución + la memoria que ocupa.

Los SOs no administran programas sino *procesos*.

## Terminología

- archivo: secuencia de bytes con un nombre y una serie de atributos
  (o metadata) que indican permisos

- directorio: colección de archivos y directorios que contiene un nombre
  y está organizado jerárquicamente. Genera un grafo (no necesariamente un
  arbol)

- dispositivo virtual: TODO

- filesystem: TODO
- directorios de sistema: directorios donde el propio SO pone cosas que necesita
  por ej. `/boot` el kernel, `/devices` archivos falsos que repr. a los disp
  `C:\Windows\system32` drivers, archivos, etc.
- binarios del sistema: Archivos que sirven para hacer tareas de operación y
  mantenimiento que son muy importantes pero no forman parte del kernel
  - `/usr/sbin(system binaries)/syslogd`: guardan los eventos del sistema
    en un archivo.
  - `/bin/sh` o `/bin/bash` shell
  - `usr/bin/who` quienes están logueados en el sistema

- Archivos de configuración: de texto pero con info importante. Por ej
  `/etc/passwd` o `C:\Windows\system32\user.dat` que contienen las contraseñas

- usuario: Representación de las personas que lo usan. Sirve para separar las
  cosas entre sí. Aislar información y establecer limitaciones.

- grupo: colección de usuarios

## Repaso

- so (mantra)
  - administrador de recursos: se pone la gorra y se fija que no nos mandemos
    cagadas
  - interfaz de programación: nos deja hacer cosas fácil con el hardware
- evolución historica
- misión fundamental
- SO batch e interactivos
- multiprogramación
- que cosas son parte del SO (kernel) y cuales no.

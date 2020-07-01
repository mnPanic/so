# Kernels

## Monolíticos

El kernel tradicional que imaginamos en esta materia tiene una serie de problemas

- Cacho de código grande que corre todo con máximo privilegio
- Cuando hay que actualizarlo es muy complicada, que tal vez requiere reiniciar el sistema, y tal vez queria solamente actualizar una partecita.+
- Menos flexible, los módulos no se pueden reutilizar.
- Si el kernel falla se cuelga todo el sistema
- Los servicios que brinda el sistema opeartivo son iguales para todos los usuarios.

## µkernel

Minimo kernel

- Manejo basioc de memoria
- IPC liviano
- Manejo basico de E/S

El resto se provee por procesos que corren fuera del kernel: **servicios**

- Hay un proceso que implementa TCP/IP, otro para filesystems, etc.

Se intentó hacer experimentos, y fallaron, no se logró la misma performace.

Pero en el camino los kernels se aligeraron. 

## Virtualización

Posibilidad de que un conjunto de recursos físicos se vean como varias copias de recursos lógicos. La acepción más común es pensar en una computadora realizando el trabajo de varias.

Objetivos:

- Portabilidad (por ej la JVM)
- Simulación / testing
- Aislamiento, particionamiento de HW, agrupamiento de funciones (consolidation)
  Es lo que suelen hacer los proveedores de internet. Si quiero tener un servidor en internet
- Protección ante fallas del HW.

### Simulación

El sistema *anfitrión* construye una variable de estado artificial que representa al sistema *huesped*. Se lee cada instrucción y se modifica el estado como si esta se ejecutase realmente.

Puede llegar a ser muy lento

### Emulación

El sistema corre realmente en la cpu de la computadora anfitriona.

Las cosas que funcionan en el HW tal cual, las ejecuto, y las que no las reemplazo por equivalentes (*traduccion binaria*).

El gran problema que tiene es la separación de privilegios

### Virtualizacion asistida por HW

Estoy en la virtual y tengo un kernel que está pensado para correr con máximo privilegio, pero en realidad es un proceso que está corriendo como usuario

Se agregan al procesador instrucciones VT-x de virtualizacion, con dos modos

- VMX root
- VMX non-root

## Contenedores

Es una especie de maquina virtual, no simula, su motivación tiene que ver.

La idea es correr apps en la misma computadora sin que interfieran entre si, generando un entorno lo más replicable posible.

tengo un proceso que corre distintos contenedores, ambientes virtuales que no necesariamente son completos.
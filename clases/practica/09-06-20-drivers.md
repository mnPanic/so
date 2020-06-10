# Drivers

El driver es parte del SO a efectos practicos. Corre en espacio de kernel.

El controlador del dispositivo es un software que corre dentro del controlador,
es parte del hardware. y hasta podria estar implementado en HW.

Es driver es una pieza de software que conoce muy bien el kernel donde esta
instalado y el dispositivo

Traduce el open, read, write que hace el usuario, a poner en tal registro tal
bit, etc. Permiten brindarle al soft de usuario una interfaz unificada para
dispositivos distintos.

## Tipos de dispositivos

- char device: A los que escribis y lees como si fuera un stream de datos
  Por ejemplo un teclado.
- block device: Das una direccion y vas y lees en esa direccion, o escribis

El robot es un char device, solo lees y escribis, ni decís donde. Hay como un
stream de bytes que se escriben en el robot.

## Interfaz

La interfaz de la diapo es una version *simplificada*

Carga y descarga: en gral el driver se considera como parte del sistema
operativo.

## Desarrollo

Como driver hay que asumir responsabilidad de kernel, entre ellas garantizar la
seguridad de los datos.

!! Nunca hacer wait en un handler

Un deadlock en kernel es irrecuperable.

## Taller

Definir todo como `static`! hace que quede en una pos fija de memoria, en la
seccion de codigo.

Todo lo que se hace en el init se debe deshacer cuando se saca.
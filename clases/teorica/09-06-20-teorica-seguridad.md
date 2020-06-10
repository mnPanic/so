# Seguridad y proteccion

Nosotros vamos a tratar a proteccion y seguridad como lo mismo

## Seginf

Seguridad de la informacion se entiende como preservar lo siguiente de los datos:

- confidencialidad
- integridad
- disponibilidad

## Protagonistas

- Sujetos
- Objetos
- Acciones: permitidas o no permitidas

Por ej. un proceso (sujeto) quiere escribir (accion) en un pipe (objeto)

El sujeto por lo general es el usuario

### Usuarios

#### Grupo

Los usuarios se agrupan en grupos. Se pueden dar permisos a colectivos de
usuarios en vez de usuarios en particular.

#### Rol

Tiene que ver con la capacidad de hacer o no algo.

Si bien la implementacion es un grupo, la abstraccion es diferente.

En vez de una bolsa en la que metes a los usuarios, etiquetas cada uno asi
dandole roles.

## Triple A

- Authentication: Sos quien decis ser? (poner la pass correcta)
- Authorization: Que podes hacer? (una vez que el usuario es correcta, que puede
  hacer?)
- Accounting (Contabilidad): dejar registro de que hizo cada usuario, para poder
  saber quien lo realizó.

Descomponer "la seguridad de un sistema"

## Criptografía

Criptografia: Se encarga de ocultar la info de forma tal que solo pueda ser
leida por quien uno quiera. Sistemas de encripción

El criptoanálisis no es lo mismo, sino que se encarga de ver como romper lo que
hizo la crypto.

Los algoritmos de encripción se dividen en simétricos y asimétricos.

Simetrico: la misma clave que uso para encriptar me sirve para desencriptar.

- DES: Data Encryption Standard. Fue por muchos años el estandar. Es de fines de
  los 60s.

Como establezco una nueva clave? El primero es RSA, que tiene claves
asimetricas.

### RSA

Las siglas son del los creadores

La clave publica la puede conocer cualquier persona

Requiere factorizar un numero en sus componentes primos, lo cual es NP completo,
es decir, no se conoce una solucion polinomial.

La clave privada es de cada receptor, entonces con cada persona que te comunicas
generas una clave publica y una privada.

Cualquier mecanismo de encripcion usa esto como una parte. Se suela combinar con
simetrica.

### Hashing

Funciones de hash, hay una familia que se llaman *one way hash*.

Lo que yo hasheo no es fácil (salvo por brute force) no es facil volver a
entender que valor era

- Resistencia a la preimagen. Dado h resultado de h = hash(m) quiero que sea
  dificil encontrar tal m
- Resistencia a la segunda preimagen: Deberia ser dificil encontrar dos valores
  que hasheen a lo mismo.

Se usan para almacenar las contraseñas de los usuarios.

Antes estaban en texto plano, y el problema es que si hay un problema con los
permisos, un usuario tenia los pw de todos, y tambien que el admin tiene todas
las pwds.

Los sistemas cuando pones la contraseña calculan el hash, y lo comparan contra
el hash.

Una funcion de hash simple es suceptible a *ataques de diccionario*.

Lo de salt es agregar especie de numeros a azar y hashearlo todo junto.

### Firma digital

Como se que alguien firmo un documento

Para firmar:

- Calculo el hash del documento
- Encripto con mi clave privada (que solo yo tengo)
- Entrego el documento y el hash encriptado (la firma)

La persona que recibe

- Agarra el doc y lo hashea, y deberia obtener el mismo hash que el que firmó.
- Desencripta con la publica. Si da lo mismo, el unico que pudo haber encriptado
  es el dueño de la privada, y se valida la firma.

En la practica, hay key servers que tienen las claves publicas de la gente.

#### Tokens

Dispositivos que permiten almacenar la clave privada para firmar.

## DAC vs MAC

DAC: Sistemas de control de acceso discrecional. Es lo que conocemos, yo decido
los permisos que tienen las cosas

MAC: Control de acceso obligatorio. Inspirado por el mundo del espionaje, Cada
sujeto tiene un grado o un nivel. Los objetos heredan el grado del ultimo
sujeto que los modificó

### En UNIX se usa DAC

Los permisos son r, w, x

    dueño grupo usuarios
tipo rwx   rwx    rwx

## Revocar

Yo borro un usuario. Lo saco de la lista de passwords, no se va a poder loguear
mas, y si tiene un proceso corriendo, lo tengo que matar.

Entonces si ya accedio, lo tengo que sacar del sistema.


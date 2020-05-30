# Redes

Le quiero enviar un cacho de bytes a un proceso en otra computadora.

Como hago para decir a quien? En una red, cada una tiene que tener un número
único, lo cual se llama *dirección IP*, y cada una es un nro de 32 bits.

    192.168.3.2 = 2*256^0 + 3*256^1 + 168*256^2 + 192*256^3

Y a un programa dentro de una computadora? Además de la dirección IP, está la
idea de puertos.

    192.168.3.2:8080

La info en el medio se puede perder.

- Problemas en el orden electromagético
- Se corto un enlace
- Ruido e interferencia

## TCP - Transfer Control Protocol

Es una idea brillante (se ve en redes), que es una manera de enviar las cosas
de una manera muy astuta. Simulás que tenés un canal confiable, teniendo
mecanismos de reintentos, etc.

## Telnet / SSH

Usada para conectarse de manera remota

- hostname: host de la computadora
- uname -a: sistema operativo
- ifconfig -a: todas las direcciones ip
- telnet: solo conectarse y mandar un mensaje por tcp, es inseguro porque no
  encripta.
- ssh: arriba de una conexión tcp encripta

- /usr/sbin/sshd

## Network byte order

Endianness (little o big endian) / network byte order.

`htons`: La corres antes de mandar la info a la red, si coincide con el network
byte order no hace nada, y sino lo cambia al endianness adecuado.
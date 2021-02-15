# Finales

- (telegram) Mariana M con chapa, 1 fecha 02/21

  Me tomó Chapa,
  - preguntó lo mismo de procesos que pregunta siempre: si un sistema está lento por qué funcionaría agregar otro procesador, y lo mismo pero agregando memoria.
  - Lo mismo con sincronización: qué primitivas de sincronización usar para distintos escenarios.
  - De FS me preguntó qué FS me conviene para implementar un undo del último cambio hecho (acá hice aguas). Y diferencias entre FAT e inodos.
  - De seguridad me preguntó también, la típica de buffer overflow, mecanismos para prevenirlo, y en qué se basa MAC.
  - De memoria me mostró un árbol de procesos y cada proceso tenía páginas asignadas y cada proceso tenía repetida la página C y me preguntó por qué podía pasar eso.

- (telelgram) Facu linari con Baader 1 fecha 02/21, fuente telegram

  Temas

  - VFS (Virtual File System), como funciona y si se lo puede considerar distribuido
  - RAID
  - Inversión de prioridades
    > Me dijo: sabes que es inversión de prioridades? Y ahí empezas a decir que dado un algoritmo de scheduling con prioridades si tenes un proceso de maxima prioridad A que espera un recurso R que está usando C(proceso con minima prioridad).Este proceso A va a tener que esperar que C libere el recurso pero C espera que B termine por tener prioridad median.Luego A que era de prioridad maxima esta esperando a todo el mundo(se le invirtio la prioridad). Se soluciona haciendo que C herede la prioridad de A hasta que libere R

  - 2PC
  - Algoritmos de scheduling de disco
  - Buffer overflow

- (telegram) Juan Cruz Basso con baader 1 fecha 02/21

  - Hablar de file systems en si (FAT/inodes) hable un poco las pros y las contras de uno. Hablamos tambien de cuando tiene sentido tener los archivos no en lugares aleatorios, sino de manera secuencial (CD/DVD que tenes una sola escritura). Para que sirven los block groups.
  - File systems distribuidos, hablamos de la interfaz VFS aca. Sus limitaciones. Como podria hacer para poder tener 2 discos distribuidos, y que los dos contengan la misma informacion, para usarlo de redundancia. Ahi hablamos de como se podia manejar esto para que no tenga fallas, si se caia la conexion en el medio y demas
  - Seguridad: Como poder brindar de manera segura las actualizaciones de un software. Como poder un cliente A conectarse a un servidor B (podia usar publica y privada A)
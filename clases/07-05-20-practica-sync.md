# Sincronización entre procesos

## Semaforo

Es un TAD que permite controlar el acceso a un recurso compartido por
múltiples procesos.

Tiene un valor al cual no podemos acceder. La única de interactuar con el
semáforo es mediante las primitivas `wait()` y `signal()`, las cuales son
**atómicas** a efectos de los procesos. (es decir, no se entrelazan con
otros procesos, no debería haber condiciones de carrera por ese lado)

Primitivas

- `sem(uint val)`: Devuelve un nuevo semáforo inicializado en ese valor
- `wait()`: Mientras el valor sea <= 0 se bloquea el proceso esperando
  un signal. Luego decrementa el valor del semáforo.
- `signal()`: Incrementa en uno el valor del semáforo y despierta a
  *alguno* de los procesos que estén haciendo `wait` sobre él.


### Mutex

```mutex = sem(1)```

`mutex.lock` sería `wait(sem)` y `mutex.unlock` sería `signal(sem)`

### Ej 3

```text
s_c = sem(1)
s_a = sem(0)
s_b = sem(0)

A               | B             | C
s_c.wait()      | s_a.wait()    | s_b.wait()
...             | ...           | ...
s_a.signal()    | s_b.signal()  | s_c.signal()
```

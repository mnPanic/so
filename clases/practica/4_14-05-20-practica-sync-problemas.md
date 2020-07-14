# Problemas de sync

## Barrera

seccion concurrente
rendezvous

wait(mutex)
count += 1
if count == n
    signal(barrera)
signal(mutex)

wait(barrera)
seccion critica
signal(barrera)

### Reutilizable

en un while true

n
count = 0
barrera = sem(0)
mutex = sem(1)

while(true):
    count = 0

    seccion concurrente
    rendezvous

    wait(mutex)
    count += 1
    if count == n
        signal(barrera)
    signal(mutex)

    wait(barrera)
    signal(barrera)

    wait(mutex)      # lock
    seccion critica
    signal(mutex)    # unlock

## Queue


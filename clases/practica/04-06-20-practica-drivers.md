# Drivers - Estrategia

Ejercicios con escenarios

## 1 - Touch de un celu android

1. No tiene sentido porque lo realentizaria mucho y queremos que sea interactivo
2. .
   1. Spooling: No tiene sentido
   2. DMA: Es matar una mosca a cañonazos. No justifica.
   3. Polling: gasta mucha bateria, el procesador funciona todo el tiempo
   4. Interrupciones: Si tiene sentido.

## 2 - Carpeta de Google Drive/Dropbox

1. Buffer: Si se hace una sync intensiva el buffer no seria tan copado
   Depende de la necesidad. Si no molesta mandar los cambios un poco mas tarde,
   pero si yo necesito el tiempo en vivo, por ej. con Google Drive un documento
   compartido ahí tiene mas sentido mandar los cambios.

   Pero para un backup puede convenir usar un buffer.
2. Implementarlo con
   1. Spooling: Problemas de concurrencia.
   2. DMA: si es muy grande no
   3. Polling: 

## 3 - GPU

1. Buffer: No tiene mucho sentido porque es mas tiempo real
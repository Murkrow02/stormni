# Step 02 — Movimento

## Obiettivo

Un cerchio che **si muove** da sinistra a destra con velocità costante. Quando esce dal bordo destro, riappare a sinistra.

## Fisica: integrazione di Eulero esplicito

Hai una **posizione** `p` e una **velocità** `v`. In tempo continuo: `dp/dt = v`. Sullo schermo il tempo procede a salti discreti di durata `dt` (un frame, ~16.7 ms a 60 FPS). Approssimazione di Eulero:

```
p(t + dt) ≈ p(t) + v · dt
```

In codice:

```cpp
float dt = GetFrameTime();   // raylib ti dà il dt del frame appena passato
pos_x += vel_x * dt;
pos_y += vel_y * dt;
```

**Importante:** `vel_x` è in **pixel al secondo** (non per frame). Se metti `vel_x = 200`, il punto attraversa 200 pixel in 1 secondo, a prescindere dal frame rate. Questo è il motivo per cui moltiplichi per `dt`: rende l'animazione **indipendente dal frame rate**.

Se invece di `*dt` mettessi semplicemente `pos_x += vel_x`, su un computer veloce il punto sfreccerebbe, su uno lento andrebbe a lumaca. **Sempre moltiplica per dt.**

## Cosa devi fare

Apri `main.cpp`:

1. **TODO 1**: dichiara due `float` per la posizione (`pos_x`, `pos_y`) e due per la velocità (`vel_x`, `vel_y`). Inizializza pos al centro, velocità `(150, 0)` (orizzontale verso destra).
2. **TODO 2**: dentro il loop, prima del disegno, aggiorna `pos_x += vel_x * dt;` e `pos_y += vel_y * dt;`.
3. **TODO 3**: se `pos_x > LARGHEZZA`, riportalo a `0` (wrap semplice).
4. **TODO 4**: disegna il cerchio in `(pos_x, pos_y)`.

## API raylib utili

| Funzione | Cosa restituisce |
|----------|------------------|
| `float GetFrameTime()` | Tempo trascorso dall'ultimo frame, in secondi |

## Verifica

Cerchio bianco che traversa la finestra da sinistra a destra, rientra dall'altro lato, ciclicamente.

## Domande per riflettere

- Se metti `vel_y = 100`, in che direzione si muove? Verso l'alto o verso il basso? (Risposta: verso il **basso**, ricorda che `y` cresce verso il basso sullo schermo.)
- Cosa succede se non moltiplichi per `dt`? Prova: imposta `pos_x += vel_x` (senza `*dt`). Cosa cambia? Cambia se modifichi `SetTargetFPS(30)` o `SetTargetFPS(120)`?
- Eulero esplicito è la **forma di integrazione più semplice** ma anche la meno accurata. In questo progetto va bene; in fisica seria per sistemi caotici si usa Runge-Kutta o leapfrog. Per i nostri storni a 60 FPS, Eulero basta.

## Quando hai finito

→ **avanza a `03_vettori/`**.

# Step 03 — Vettori 2D

## Obiettivo

Smettere di trascinarci dietro coppie di `float` separati (`pos_x, pos_y`, `vel_x, vel_y`, …) e usare un'unica `struct Vec2`. Capire le **operazioni vettoriali base** che useremo per tutto il resto del progetto.

## Fisica: ripasso vettori in 2D

Un vettore in 2D è una coppia `(x, y)`. Servono queste operazioni:

| Operazione | Definizione | A cosa serve |
|------------|-------------|--------------|
| **Somma** | `a + b = (a.x+b.x, a.y+b.y)` | Spostare un punto: `p_nuovo = p + v·dt` |
| **Sottrazione** | `a - b = (a.x-b.x, a.y-b.y)` | Vettore "da `b` a `a`" |
| **Moltiplicazione per scalare** | `a * k = (a.x*k, a.y*k)` | Scalare una velocità, applicare `dt` |
| **Norma (lunghezza)** | `|a| = √(a.x² + a.y²)` | Quanto è grande il vettore |
| **Normalizzazione** | `â = a / |a|` (se `|a| > 0`) | Versore: direzione di `a`, lunghezza 1 |
| **Distanza** | `dist(a,b) = |a - b|` | Quanto distano due punti |

Tutto qui. Con queste 6 operazioni implementi le 3 regole di Reynolds.

## Cosa devi fare

In questa cartella trovi:

- `vec2.h` — la **dichiarazione** della struct e delle funzioni (incompleta)
- `main.cpp` — un piccolo test visivo: disegna un cerchio mosso usando `Vec2`

1. Apri `vec2.h` e **implementa il corpo delle funzioni** `sub`, `mul`, `norm`, `normalize`, `dist` (la `add` è già fatta come esempio).
2. Apri `main.cpp`: è quasi identico allo step 02, ma usa la tua `Vec2`. Non c'è molto da modificare; compila e gira.

> **Stile.** Mettiamo le funzioni *inline* nell'header per semplicità (niente file `.cpp` separato, niente problemi di linker). È normale per "header-only mini-libraries".

## API raylib utili

Le stesse di prima (`DrawCircle`, `GetFrameTime`).

## Verifica

Il cerchio si muove come nello step 02. Se va, le tue operazioni vettoriali sono corrette.

Test extra (facoltativo): in `main.cpp` aggiungi una stampa a console con `printf("|v| = %f\n", norm(vel));` e verifica che la velocità sia ~150.

## Domande per riflettere

- Perché normalizzare? Se hai un vettore "da A a B" e vuoi "muoversi verso B a velocità 200 px/s", fai `vel = normalize(B - A) * 200`. Senza normalizzare, la velocità dipenderebbe da quanto è lontano B.
- Cosa succede se chiami `normalize` su un vettore nullo `(0,0)`? Divisione per zero! Nel codice usiamo `if (n > 0)` per evitarlo.

## Quando hai finito

→ **avanza a `04_stormo/`**.

# Step 01 — Disegna un Punto

## Obiettivo

Disegnare un **cerchio bianco fermo al centro** della finestra.

![risultato atteso: un puntino bianco al centro di un rettangolo nero]

## Concetto: il sistema di coordinate dello schermo

Le coordinate sullo schermo **non** sono come in matematica:

```
        (0,0) ───────────► x
          │
          │
          ▼
          y
```

- L'origine `(0,0)` è in **alto a sinistra**.
- `x` cresce verso **destra**.
- `y` cresce verso il **basso** (al contrario di come si fa di solito in fisica!).

Per disegnare al centro: `x = LARGHEZZA / 2`, `y = ALTEZZA / 2`.

Più avanti, quando integreremo la fisica, dovremo ricordare che **una velocità verticale positiva in pixel = movimento verso il basso**. Lo terremo a mente.

## Cosa devi fare

Apri `main.cpp` e completa il `// TODO`:

1. Dentro `BeginDrawing() ... EndDrawing()`, chiama `DrawCircle(x, y, raggio, colore)` per disegnare un cerchio al centro.

Suggerimento sui parametri:

```cpp
DrawCircle( LARGHEZZA/2,    // posizione x
            ALTEZZA/2,      // posizione y
            8,              // raggio in pixel
            WHITE );        // colore (raylib ha già definiti BLACK, WHITE, RED, ...)
```

## API raylib utili in questo step

| Funzione | A cosa serve |
|----------|--------------|
| `DrawCircle(int x, int y, float r, Color c)` | Cerchio pieno |
| `BLACK`, `WHITE`, `RED`, `GREEN`, `BLUE`, `RAYWHITE`, `YELLOW` | Colori predefiniti |

## Verifica

`./build.sh` → finestra nera con **un cerchio bianco** al centro. Premi ESC.

## Domande per riflettere

- Cosa succede se metti `DrawCircle` **fuori** dal blocco `BeginDrawing/EndDrawing`? (Prova! raylib emette warning e non disegna.)
- Cosa succede se non metti `ClearBackground(BLACK)` dentro il loop? (Prova! Il vecchio frame "resta", il cerchio sembra sparire ma la finestra si riempie di residui.)

## Quando hai finito

→ **avanza a `02_movimento/`**.

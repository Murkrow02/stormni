# Step 00 — Setup

## Obiettivo

Verificare che la **toolchain funziona**: aprire una finestra nera 800×600 che si chiude premendo `ESC` o la X.

In questo livello **non devi scrivere niente**: `main.cpp` è già completo. Serve solo a controllare che `raylib` sia installato e compilabile.

## Cosa devi fare

1. Da terminale, nella cartella `00_setup/`:
   ```bash
   chmod +x build.sh
   ./build.sh
   ```
2. Dovrebbe aprirsi una finestra nera con titolo "Storni - Step 00 - Setup".
3. Premi `ESC` per chiuderla.

## Se non funziona

| Errore | Causa | Soluzione |
|--------|-------|-----------|
| `raylib.h: No such file or directory` | raylib non installata | `brew install raylib` |
| `Package raylib was not found in the pkg-config search path` | pkg-config non trova raylib | `brew install pkg-config` (e magari `brew reinstall raylib`) |
| `clang++: command not found` | manca Xcode CLI tools | `xcode-select --install` |
| Permission denied su `./build.sh` | Manca permesso esecuzione | `chmod +x build.sh` |

## Cosa fa `main.cpp` qui

Tre cose, niente di più:

1. `InitWindow(LARGHEZZA, ALTEZZA, "...")` — crea la finestra.
2. Loop `while (!WindowShouldClose())` — gira finché l'utente non chiude.
3. `BeginDrawing() / ClearBackground(BLACK) / EndDrawing()` — riempie ogni frame di nero.

`SetTargetFPS(60)` limita a 60 frame al secondo, così il loop non frigge la CPU. Tutta la simulazione girerà a questo ritmo: ogni iterazione del `while` è **un frame**.

## Domande per riflettere

- Perché un *loop* e non una singola chiamata "disegna"? (Risposta: animazione = sequenza di frame nel tempo. Ogni frame ridisegna tutto da capo.)
- Quanto dura un frame a 60 FPS? (Risposta: `1/60 ≈ 16.67 ms`. È il nostro `dt` di integrazione fisica.)

## Quando hai finito

Se la finestra si apre e si chiude pulita → **avanza a `01_punto/`**.

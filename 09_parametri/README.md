# Step 09 — Tara i Parametri in Tempo Reale

## Obiettivo

Modificare i pesi delle 3 regole **mentre la simulazione gira**, premendo tasti sulla tastiera, per **vedere subito** l'effetto sul comportamento dello stormo. Anche i valori correnti vengono stampati sullo schermo.

> **Nota.** Inizialmente il piano era usare la libreria `raygui` (slider grafici). Ma `raygui` aggiunge una dipendenza header-only da scaricare. Più semplice e didattico: tasti + `DrawText` per i valori. Stesso effetto, zero dipendenze.

## Concetto: input da tastiera

`raylib` rende l'input semplice:

```cpp
if (IsKeyDown(KEY_Q))   // tieni premuto Q
if (IsKeyPressed(KEY_R)) // R appena premuto (singolo evento)
```

Per **rendere fluida** la regolazione, usiamo `IsKeyDown` con un piccolo incremento `* dt`:

```cpp
if (IsKeyDown(KEY_Q)) W_SEP -= 1.0f * dt;
if (IsKeyDown(KEY_A)) W_SEP += 1.0f * dt;
```

(Q diminuisce, A aumenta. Stesso schema per le altre due regole.)

## Mostrare valori a schermo

```cpp
DrawText(TextFormat("W_SEP  = %.2f  (Q-/A+)", W_SEP), 10, 10, 18, WHITE);
```

`TextFormat` è l'equivalente raylib di `printf` — restituisce una stringa pronta per `DrawText`.

## Cosa devi fare

Partendo dallo step 08 (la murmuration funzionante):

1. **TODO 1**: trasforma `W_SEP`, `W_ALL`, `W_COES` da `const float` a **variabili modificabili** (rimuovi `const`). Spostale dentro `main()` o tienile globali ma non `const`.
2. **TODO 2**: nel loop, prima della fisica, leggi i tasti:
   - `Q`/`A` → modifica `W_SEP`
   - `W`/`S` → modifica `W_ALL`
   - `E`/`D` → modifica `W_COES`
   - `R` (con `IsKeyPressed`) → resetta tutti a 1.0 e rigenera lo stormo
3. **TODO 3**: assicurati che i pesi non scendano sotto 0 (clamp). Sennò le regole si "invertono" (separazione che attrae, ecc.) e ottieni roba interessante ma confusionaria.
4. **TODO 4**: alla fine del rendering, prima di `EndDrawing()`, disegna 3 righe di testo coi valori correnti.

## API raylib utili

| Funzione | Cosa fa |
|----------|---------|
| `IsKeyDown(key)` | true se il tasto è premuto in questo frame |
| `IsKeyPressed(key)` | true solo nel frame in cui viene premuto |
| `KEY_Q`, `KEY_A`, ... | costanti per i tasti |
| `DrawText(testo, x, y, size, colore)` | scrive testo |
| `TextFormat(fmt, ...)` | stile printf |

## Verifica

Apri la simulazione. Premi:

- `Q` per indebolire la separazione → i boid si accavallano in punti densissimi
- `A` per rinforzarla → si distanziano
- `S` per indebolire l'allineamento → la "direzione comune" sparisce
- `E`/`D` per giocare con la coesione → stormo che si stringe/sparpaglia
- `R` per ricominciare

## Domande per riflettere

- Esiste un "punto giusto"? In che senso? (Per un comportamento "visivamente realistico" sì; ma molti regimi diversi sono interessanti.)
- Come si chiama la branca della fisica che studia comportamenti come questo al variare di parametri? (*Diagrammi di fase*, *transizioni di fase*. La transizione da "moto random" a "stormo coerente" è analoga a una transizione di fase di secondo ordine.)
- Una sfida: trovare la **soglia di formazione dello stormo** facendo variare solo `W_COES`. Sotto un certo valore, niente stormo. Sopra, stormo. Esiste un *parametro critico*?

## Quando hai finito

→ **avanza a `10_predatore_e_ostacoli/`** per il bonus, o `11_3d_bonus/` per il 3D.

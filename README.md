# Storni — Simulazione di Murmurations in C++

Tutorial passo-passo, stile **nand2tetris**, per costruire da zero un simulatore del volo coordinato degli storni (le famose *murmurations*) usando le **regole di Reynolds**.

> Progetto pensato per un corso di fisica. Il C++ qui dentro è il più semplice possibile: `struct`, `std::vector`, funzioni libere. Niente classi astruse, niente template, niente smart pointer. L'obiettivo è capire **la fisica**, non torturarsi col linguaggio.

---

## Cos'è una murmuration?

Migliaia di storni che volano insieme formando nuvole danzanti, senza un capo, senza spigoli, senza collisioni. Sembra magia ma non lo è: ogni uccello segue **3 regole locali semplicissime** guardando solo i 6-7 vicini più prossimi (studi STARFLAG su storni reali di Roma, Ballerini et al., 2008).

Craig Reynolds nel 1987 (paper "Flocks, herds and schools") dimostrò che queste 3 regole bastano:

1. **Separazione** — non urtare i vicini troppo stretti
2. **Allineamento** — vola nella stessa direzione media dei vicini
3. **Coesione** — stai vicino al gruppo

Da queste 3 regole locali emerge il comportamento globale dello stormo. È un esempio classico di **fenomeno emergente / auto-organizzazione**.

Lo costruirai tu, un pezzo alla volta.

---

## Setup (macOS)

Servono 3 cose: un compilatore C++ (già hai `clang++` su macOS), la libreria grafica **raylib**, e **pkg-config** per linkarla facilmente.

```bash
brew install raylib pkg-config cmake
```

Verifica:

```bash
pkg-config --cflags --libs raylib
```

Se stampa una riga con `-lraylib` e dintorni, sei a posto.

---

## Come usare questo tutorial

Le cartelle sono numerate da `00_setup` a `11_3d_bonus`. Ogni cartella è un **livello**: si apre, si legge il `README.md`, si modifica `main.cpp` completando i `// TODO`, si esegue il livello. Se il risultato visivo corrisponde a quello descritto, si passa al successivo.

Ci sono **due modi** di compilare ed eseguire un livello: terminale (consigliato per partire) o CLion (consigliato dopo).

### Modo A — Terminale (script semplice)

```bash
cd 00_setup
cat README.md          # leggi il livello
# (modifica main.cpp se serve)
chmod +x build.sh      # solo la prima volta
./build.sh             # compila ed esegue
```

### Modo B — CLion (autocompletamento + debugger)

Nella cartella root c'è un `CMakeLists.txt` che dichiara un eseguibile per ogni stage.

1. Apri CLion → `File` → `Open` → seleziona la cartella `storni/`.
2. CLion riconosce il `CMakeLists.txt` e ti propone "Open as CMake project" → conferma.
3. Aspetta che indicizzi (qualche secondo): a quel punto vedrai 12 target nella tendina in alto a destra (`00_setup`, `01_punto`, ..., `11_3d_bonus`).
4. Seleziona il target del livello su cui stai lavorando, premi il pulsante verde **Run** (▶). CLion compila ed esegue.
5. Autocompletamento delle funzioni raylib (`DrawCircle`, `InitWindow`, ecc.) funziona dentro **ogni** stage.

> **Suggerimento.** Quando aggiungi/modifichi parametri, premi `⌘⇧A` → "Reload CMake Project" se CLion non aggiorna l'indice da solo.

In ogni cartella trovi:

| File | Cosa contiene |
|------|---------------|
| `README.md` | Spiegazione fisica + lista TODO del livello |
| `main.cpp` | Codice di partenza con `// TODO` da completare |
| `build.sh` | Una riga di compilazione + esecuzione |
| `_soluzione.cpp` | Soluzione completa. **Non sbirciare prima di provare!** |

### Regola d'oro

> Non aprire `_soluzione.cpp` finché non hai provato almeno 15 minuti. La fatica produttiva è il punto del tutorial.

---

## Percorso

| Step | Argomento | Cosa vedrai |
|------|-----------|-------------|
| 00 | Setup | Una finestra nera che si chiude con ESC |
| 01 | Disegnare un punto | Un cerchio bianco al centro |
| 02 | Movimento | Il punto si sposta a velocità costante |
| 03 | Vettori 2D | Implementi tu somma/scala/norma |
| 04 | Stormo di N boid | 100 pallini che vanno random |
| 05 | Bordi | Wrap toroidale: chi esce a destra rientra a sinistra |
| 06 | Separazione | Regola 1 — i boid si evitano |
| 07 | Allineamento | Regola 2 — direzioni si uniformano |
| 08 | **Coesione** → **MURMURATION** | Regola 3 — emerge lo stormo |
| 09 | Slider parametri | Tara i pesi delle regole in tempo reale |
| 10 | Predatore + ostacoli | *(Bonus)* Un falco e palazzi |
| 11 | 3D | *(Bonus)* Lo stormo in 3 dimensioni |

---

## Riferimenti

- Reynolds, C. (1987). *Flocks, herds and schools: A distributed behavioral model.* SIGGRAPH '87.
- Ballerini et al. (2008). *Interaction ruling animal collective behavior depends on topological rather than metric distance.* PNAS — il celebre studio sugli storni di Roma (progetto STARFLAG).
- raylib cheatsheet: <https://www.raylib.com/cheatsheet/cheatsheet.html>

Buon volo. 🪶 *(ok, una sola)*

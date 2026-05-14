# Step 04 — Uno Stormo di N Boid

## Obiettivo

Da **un punto** a **uno stormo**. Disegnare `N = 100` boid (così chiameremo i nostri "uccelli"), ciascuno con posizione e velocità casuale. Per ora si muovono in linea retta, **senza interagire** tra loro.

> Termine. *Boid* = "bird-oid", nome originale di Reynolds per le particelle del suo modello. Le useremo come sinonimo di "uccello virtuale" da qui in poi.

## Concetto: struct Boid + std::vector

In C++ raggruppiamo posizione e velocità in una `struct`:

```cpp
struct Boid {
    Vec2 pos;
    Vec2 vel;
};
```

Per averne `N`, usiamo `std::vector<Boid>` — un array dinamico della libreria standard. Si manipola così:

```cpp
std::vector<Boid> stormo;       // vector vuoto
stormo.push_back(Boid{...});    // aggiunge un boid
stormo[i].pos                   // accesso all'i-esimo
stormo.size()                   // quanti sono
```

## Numeri casuali

Per le velocità iniziali serve un random. Usiamo le funzioni standard di C:

```cpp
#include <cstdlib>   // per rand() e RAND_MAX
#include <ctime>     // per time()

srand(time(NULL));   // seed all'inizio del programma
float r = (float)rand() / RAND_MAX;          // numero in [0, 1]
float v = (r * 2.0f - 1.0f) * 100.0f;        // numero in [-100, +100]
```

Non è il random più sofisticato, ma per la simulazione visiva basta.

## Cosa devi fare

1. **TODO 1**: includi `<vector>` e `<cstdlib>` e `<ctime>`. Definisci `const int N = 100`.
2. **TODO 2**: dichiara `std::vector<Boid> stormo` e in un ciclo riempilo con N boid: posizione casuale dentro la finestra, velocità casuale con componenti in `[-150, +150]`.
3. **TODO 3**: nel loop principale, per ogni boid `i`: `stormo[i].pos = add(stormo[i].pos, mul(stormo[i].vel, dt));`
4. **TODO 4**: disegna ogni boid con `DrawCircle`. Per ora niente wrap dei bordi (lo facciamo nel prossimo step). Usa un raggio piccolo, es. 3.

## API raylib utili

`DrawCircle`, `GetFrameTime`, già viste.

## Verifica

100 puntini bianchi che partono dal centro (o sparsi) e si allontanano verso bordi, poi escono dalla finestra e spariscono. Nessuna interazione ancora.

## Domande per riflettere

- Quanti calcoli stai facendo per frame? (N=100 update di posizione, 100 disegni). Banale a 60 FPS. Più avanti, quando ogni boid guarderà tutti gli altri (regole di Reynolds), saranno N² = 10.000 confronti/frame. Ancora gestibile.
- Cosa cambia se metti N=10000? Provalo (alla fine del progetto). A un certo punto il loop O(N²) diventa il collo di bottiglia, non il rendering.

## Quando hai finito

→ **avanza a `05_bordi/`**.

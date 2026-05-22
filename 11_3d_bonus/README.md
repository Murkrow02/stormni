# Step 11 — Murmuration in 3D (Bonus)

## Obiettivo

Estendere lo stormo alla **terza dimensione**. Visivamente: una camera 3D che osserva uno spazio cubico, con i boid che
volano liberi in tutte le direzioni. La logica è **identica** a 2D, basta sostituire `Vec2` con `Vec3`.

## Concetto: aggiungere una dimensione "gratis"

Le regole di Reynolds **non dipendono** dalla dimensionalità: sono espresse in termini di vettori e norme. Se hai
`Vec3`, le stesse formule funzionano. È uno degli aspetti più eleganti del modello.

Cosa cambia, nel concreto:

- `Vec2 → Vec3` ovunque (pos, vel, sterzate)
- Norma e distanza usano 3 componenti: `√(x²+y²+z²)`
- Le funzioni di sterzata sono **letteralmente identiche** se rinomini `Vec2` in `Vec3`
- Il rendering è diverso: serve una **camera 3D** e `DrawSphere`

## raylib in 3D

```cpp
Camera3D camera = {0};
camera.position   = { 0.0f, 200.0f, 400.0f };  // dove sta la "telecamera"
camera.target     = { 0.0f, 0.0f, 0.0f };      // dove guarda
camera.up         = { 0.0f, 1.0f, 0.0f };      // quale direzione è "in su"
camera.fovy       = 60.0f;                     // angolo di vista verticale
camera.projection = CAMERA_PERSPECTIVE;

// Nel loop:
BeginDrawing();
ClearBackground(SKYBLUE);
BeginMode3D(camera);
    UpdateCamera(&camera, CAMERA_ORBITAL);     // ruota da sola attorno al target
    DrawSphere({x, y, z}, raggio, WHITE);
    DrawCubeWires({0,0,0}, BOX, BOX, BOX, GRAY); // bordo dello spazio
EndMode3D();
EndDrawing();
```

`Vector3` di raylib è già definito con la stessa shape della nostra `Vec3`. Ma per evitare conflitti tra `Vector3` (
raylib) e la nostra `Vec3`, **manteniamo `Vec3` per la fisica** e convertiamo solo quando passiamo a raylib per il
rendering:

```cpp
Vector3 to_rl(Vec3 v) { return { v.x, v.y, v.z }; }
```

## Cosa devi fare

1. **TODO 1**: copia il `main.cpp` dello step 08 (la murmuration 2D), e sostituisci ovunque `Vec2` con `Vec3`. Le
   funzioni `add/sub/mul/norm/normalize/dist` esistono già in `vec3.h`.
2. **TODO 2**: aggiungi una componente `z` all'inizializzazione (es. `casuale(-BOX, BOX)`). Definisci
   `const float BOX = 200.0f;` come "metà lato" del cubo dello spazio.
3. **TODO 3**: il wrap toroidale diventa 3D — wrap anche `z` su `[-BOX, +BOX]`.
4. **TODO 4**: rendering. Sostituisci la finestra 2D con `BeginMode3D` / `EndMode3D`. Disegna ogni boid come
   `DrawSphere(to_rl(pos), 2.0f, WHITE)`. Disegna `DrawCubeWires` per vedere il "contenitore".
5. **TODO 5**: `UpdateCamera(&camera, CAMERA_ORBITAL)` per ruotare automaticamente attorno allo stormo. Oppure usa
   `CAMERA_FREE` e i tasti.

## API raylib utili

| Funzione                               | Cosa fa                                                                           |
|----------------------------------------|-----------------------------------------------------------------------------------|
| `BeginMode3D(camera) / EndMode3D()`    | Apre/chiude il rendering 3D                                                       |
| `DrawSphere(centro, raggio, colore)`   | Sfera piena                                                                       |
| `DrawCubeWires(centro, lx, ly, lz, c)` | Filo del cubo                                                                     |
| `UpdateCamera(&cam, MODE)`             | Aggiorna la camera. Modi: `CAMERA_ORBITAL`, `CAMERA_FREE`, `CAMERA_FIRST_PERSON`. |

## Verifica

Uno spazio cubico delimitato (`DrawCubeWires`) con dentro 100 sferette bianche che si muovono come uno stormo, mentre la
camera ruota automaticamente.

## Domande per riflettere

- Il comportamento qualitativo cambia rispetto al 2D? (Risposta: sostanzialmente no — la murmuration emerge ugualmente.
  Cambia che in 3D lo stormo ha **più spazio** per ruotare/evolvere, e tipicamente si vedono "tubi" e "fronti"
  tridimensionali.)
- Gli storni reali sono 3D. Quante delle correlazioni misurate nello studio STARFLAG (PNAS 2008) sarebbero diverse se
  gli storni fossero 2D? (Un punto interessante: la struttura "topologica" — guardare i 6-7 vicini più prossimi — non
  cambia con la dimensione. Lo studio STARFLAG aveva infatti dovuto fare ricostruzioni 3D da coppie di telecamere
  stereo.)
- Il nostro algoritmo è O(N²). In 3D, con N=10000 storni reali, servirebbero strutture spaziali (octree, grid hashing).
  Quanto è grande il guadagno?

## Quando hai finito

Hai costruito una murmuration 3D **da zero** in C++. Buon viaggio nel resto della fisica computazionale.

→ Letture suggerite: Vicsek model (1995), self-propelled particles, active matter, statistical physics of flocking.

# Simulazione del comportamento di stormi (Boids)

**Autori:** Marco Coppola, Lucia [...]  <!-- completare con nomi e cognomi del gruppo -->
**Data:** 26 giugno 2026
**Repository Git:** https://github.com/Murkrow02/stormni

---

## 1. Descrizione del progetto

Il progetto implementa una simulazione del comportamento di stormi (*boids*)
secondo il modello classico di Craig Reynolds, esteso a uno spazio
tridimensionale e a più specie di uccelli che coesistono nello stesso ambiente.

Ogni individuo (*boid*) si muove autonomamente seguendo tre regole locali:

- **Separazione**: evita di avvicinarsi troppo ai vicini.
- **Allineamento**: tende ad adeguare la propria direzione a quella dei vicini
  della stessa specie.
- **Coesione**: tende a muoversi verso il centro di massa dei vicini della
  stessa specie.

A queste si aggiunge una regola di **fuga dai pericoli** (*danger*): le pareti
del box di simulazione e i gabbiani, che agiscono da predatori nei confronti
delle altre specie.

Sono presenti tre specie con parametri differenti:

| Specie              | Colore     | `max_speed` | Ruolo                         |
|---------------------|------------|-------------|-------------------------------|
| Rondine (`Swallow`) | nero       | 105         | preda, molto veloce           |
| Storno (`Starling`) | marrone    | 65          | preda                         |
| Gabbiano (`Seagull`)| bianco     | 60          | predatore (è anche un pericolo)|

L'applicazione è grafica e interattiva: usa la libreria **raylib** per il
rendering 3D e **raygui** per i widget dell'interfaccia (menu iniziale e slider
di controllo a runtime).

---

## 2. Principali scelte progettuali e implementative

### 2.1 Struttura dei file

Il codice è suddiviso in più *translation unit*, tutte nel namespace `sim`:

| File                         | Contenuto                                                        |
|------------------------------|------------------------------------------------------------------|
| `vec3.hpp`                   | struttura `Vec3` e funzioni di algebra vettoriale (header-only)  |
| `configs.hpp`                | parametri di configurazione globali (dimensioni box, pesi, ecc.) |
| `boid.hpp` / `boid.cpp`      | classe base `Boid`: stato, regole di Reynolds, integrazione      |
| `swallow.*`, `starling.*`, `seagull.*` | specie concrete derivate da `Boid`                     |
| `danger.hpp`                 | interfaccia astratta `Danger` (pericolo)                         |
| `wall.hpp`                   | `Wall`: parete del box, implementa `Danger`                      |
| `engine.hpp` / `engine.cpp`  | classe `Engine`: ciclo di simulazione, rendering, GUI            |
| `utils.cpp`                  | conversione `Vec3` → tipo `Vector3` di raylib                    |
| `main.cpp`                   | punto di ingresso                                                |
| `test_vec3.cpp`              | unit test dell'algebra vettoriale                                |
| `test_boid.cpp`              | unit test della logica dei boid e delle pareti                   |

### 2.2 La classe `Vec3`

`Vec3` è una struct leggera con i tre componenti `x, y, z` e gli operatori
aritmetici (`+`, `-`, `*`, `/`, `+=`, `*=`, negazione, uguaglianza). Le funzioni
libere `norm`, `norm_sq`, `normalize`, `limit`, `dot`, `dist`, `dist_sq`
completano l'algebra necessaria. Dove possibile si lavora con le norme al
quadrato (`norm_sq`, `dist_sq`) per evitare radici quadrate inutili nei confronti
tra distanze.

### 2.3 La classe base `Boid` e la gerarchia per specie

`Boid` contiene lo stato comune (posizione, velocità, *heading* disegnato,
parametri di comportamento, colore, geometria del cono) e implementa l'intera
logica di Reynolds. Le specie concrete (`Swallow`, `Starling`, `Seagull`)
derivano da `Boid` e si limitano a passare al costruttore i propri parametri:
si tratta quindi di personalizzazione tramite **costruttore**, senza
ridefinizione del comportamento.

Il gabbiano (`Seagull`) eredita sia da `Boid` sia dall'interfaccia `Danger`
(ereditarietà multipla): è al tempo stesso un boid che si muove secondo le
regole dello stormo e un pericolo per le altre specie.

### 2.4 Il modello di steering

Il cuore della simulazione è `Boid::evolve`, organizzato in quattro fasi:

1. **Accumulo dai vicini** (un solo passaggio sullo stormo): per ogni altro boid
   si calcola la separazione (verso *tutti* i boid entro `r_sep`, anche di specie
   diversa, per evitare collisioni tra specie) e si accumulano velocità e
   posizioni dei soli vicini della **stessa specie** entro `r_view` per
   allineamento e coesione.
2. **Tre regole come forze di sterzo**: ciascuna regola produce una forza
   tramite il metodo `steer`, che applica il classico schema di Reynolds
   `limit(normalize(desired) * max_speed - vel, max_force)`.
3. **Fuga dai pericoli**: per ogni `Danger` (le sei pareti e i gabbiani) si
   calcola il punto più vicino e si genera una repulsione tanto più forte quanto
   minore è la distanza, pesata per la pericolosità (`base_threat`) e per la
   "paura" della specie (`fear_factor`).
4. **Somma pesata e limite totale**: le forze sono combinate con i pesi
   (per-specie × pesi globali regolabili a runtime) e l'accelerazione risultante
   è troncata a `max_force`.

Per garantire che tutti i boid evolvano leggendo lo **stesso stato** (quello del
frame precedente), si adotta uno schema *double-buffer*: `evolve` salva
l'accelerazione in `pending_acc` senza modificare lo stato, poi `apply` aggiorna
velocità e posizione di tutti i boid. L'`Engine` esegue quindi due cicli
distinti sullo stormo (prima tutti `evolve`, poi tutti `apply`).

### 2.5 Spazio toroidale e geometria del box

Il dominio è un parallelepipedo centrato nell'origine (asse Z dimezzato rispetto
a X e Y). I bordi sono **toroidali**: un boid che esce da un lato rientra dal
lato opposto (`apply_wrap`). Le pareti, pur essendo attraversabili, generano una
forza repulsiva (`Wall` come `Danger`) che tende a mantenere lo stormo
all'interno: l'effetto visivo è quello di uno stormo che evita i bordi ma che,
in caso di spinta sufficiente, "rimbalza" sul lato opposto.

`Wall::get_closest_point` proietta ortogonalmente la posizione del boid sul
piano della parete tramite il prodotto scalare con la normale.

### 2.6 Heading filtrato

La direzione con cui viene disegnato il cono di ogni boid (`heading`) non è la
velocità istantanea, ma una versione filtrata con *easing* esponenziale
(`alpha = 1 - exp(-TURN_RATE * dt)`), indipendente dal frame-rate. In questo modo
le virate appaiono morbide invece che a scatti.

### 2.7 Generazione di numeri casuali

La posizione e la direzione iniziali sono le uniche grandezze casuali. Si usano
`std::random_device`, `std::default_random_engine` e
`std::uniform_real_distribution` (motore e *device* `static`, condivisi tra le
istanze), secondo le convenzioni della standard library.

### 2.8 Interfaccia e parametri a runtime

- Un **menu iniziale** (`Engine::setup_screen`) consente di scegliere il numero
  di boid per ciascuna specie e di attivare la rotazione orbitale della
  telecamera.
- Durante la simulazione, quattro **slider** regolano in tempo reale i pesi
  globali di separazione, allineamento, coesione e paura.
- Un **HUD** mostra FPS e, per ciascuna specie, numero di individui, velocità
  media, velocità massima e un *parametro d'ordine* (norma della media delle
  direzioni normalizzate, ovvero quanto lo stormo è "allineato").

---

## 3. Costrutti non introdotti a lezione

<!-- Verificare e completare in base a quanto effettivamente svolto a lezione.
     Candidati tipici da dichiarare/giustificare: -->

- Uso della libreria grafica esterna **raylib** e di **raygui** (vedi §5).
- Variabili globali `inline` in `configs.hpp` (C++17).
- `dynamic_cast` in `Boid::evolve` per individuare i boid che sono anche
  pericoli (gabbiani).

---

## 4. Strategia di test

I test usano il framework **doctest** e sono divisi in due eseguibili:

**`test_vec3`** — verifica l'algebra vettoriale: operatori aritmetici,
uguaglianza, `norm`/`norm_sq`, `normalize` (incluso il caso del vettore nullo,
senza divisione per zero), `limit`, `dot`, `dist`/`dist_sq` e relative proprietà
(simmetria, `dot(v,v) == |v|²`).

**`test_boid`** — verifica la logica di simulazione su configurazioni
controllate:

- set/get di posizione e velocità;
- *clamp* della velocità a `max_speed`;
- *wrap* toroidale ai bordi del box;
- **separazione**: due boid troppo vicini si allontanano (anche tra specie
  diverse);
- **coesione**: un boid sterza verso un vicino fermo della stessa specie;
- **allineamento**: un boid adegua la direzione a quella del vicino (isolando la
  regola azzerando il peso della coesione);
- **assenza di allineamento/coesione tra specie diverse**;
- moto rettilineo uniforme di un boid isolato;
- convergenza dell'`heading` filtrato verso la direzione della velocità;
- proiezione di `Wall::get_closest_point` sul piano della parete.

I test riproducono il passo di simulazione esattamente come l'`Engine`
(funzione `step`: prima tutti `evolve`, poi tutti `apply`), così da validare
anche lo schema *double-buffer*.

---

## 5. Librerie esterne

Il progetto dipende da **raylib** (rendering 3D e finestra). **raygui** è incluso
direttamente nel progetto come header (`raygui.h` + `raygui_impl.cpp`) e non
richiede installazione separata.

### 5.1 Installazione di raylib

Il `CMakeLists.txt` localizza la libreria con `find_package(raylib REQUIRED)`:
è quindi necessario che raylib sia installata e visibile a CMake **prima** di
configurare il progetto. Sono richiesti CMake ≥ 3.16 e un compilatore con
supporto C++17. Di seguito le istruzioni per le piattaforme principali.

#### Debian / Ubuntu

Se la distribuzione è abbastanza recente (Ubuntu 24.04+), raylib è nei
repository:

```shell
sudo apt update
sudo apt install libraylib-dev cmake ninja-build build-essential
```

Su distribuzioni più vecchie il pacchetto può mancare o essere troppo datato:
in tal caso compilare raylib dai sorgenti (vedi §5.1, "Compilazione dai
sorgenti").

#### Fedora

```shell
sudo dnf install raylib-devel cmake ninja-build gcc-c++
```

#### Arch Linux

```shell
sudo pacman -S raylib cmake ninja base-devel
```

#### macOS (Homebrew)

```shell
brew install raylib cmake ninja
```

Homebrew installa raylib con i file di configurazione CMake, quindi
`find_package(raylib)` la trova senza ulteriori impostazioni.

#### Windows (vcpkg)

Con [vcpkg](https://vcpkg.io):

```shell
vcpkg install raylib
```

Poi passare a CMake il toolchain file di vcpkg in fase di configurazione:

```shell
cmake -S . -B build -G "Ninja Multi-Config" ^
  -DCMAKE_TOOLCHAIN_FILE=<percorso-vcpkg>/scripts/buildsystems/vcpkg.cmake
```

In alternativa, su Windows si può usare MSYS2/MinGW:
`pacman -S mingw-w64-x86_64-raylib`.

#### Compilazione dai sorgenti (qualsiasi sistema)

Quando il gestore di pacchetti non offre raylib (o offre una versione troppo
vecchia), si compila e installa dai sorgenti. Su Linux servono prima le
dipendenze di sviluppo di X11/OpenGL:

```shell
# Solo Debian/Ubuntu — dipendenze di build
sudo apt install build-essential cmake git \
  libgl1-mesa-dev libx11-dev libxrandr-dev libxi-dev \
  libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
```

Quindi:

```shell
git clone --depth 1 https://github.com/raysan5/raylib.git
cd raylib
cmake -S . -B build -DBUILD_SHARED_LIBS=OFF
cmake --build build
sudo cmake --install build
```

`sudo cmake --install build` installa header, libreria e file di configurazione
CMake nei percorsi di sistema (es. `/usr/local`), dove `find_package` li trova
automaticamente. Se si installa in un prefisso non standard, indicarlo a CMake
con `-DCMAKE_PREFIX_PATH=<prefisso>` durante la configurazione del progetto.

#### Verifica

Dopo l'installazione, la configurazione del progetto (`cmake -S . -B build`,
§6) deve completarsi senza l'errore
`Could not find a package configuration file provided by "raylib"`. Se
quell'errore compare, raylib non è installata o non è nei percorsi noti a CMake:
usare `-DCMAKE_PREFIX_PATH` per indicarne la posizione.

### 5.2 Nota sui sanitizer (`CMakeLists.txt`)

Su macOS il runtime di AddressSanitizer va in *hang* all'avvio, perciò i
sanitizer sono attivati solo su piattaforme
non-Apple (es. l'ambiente Linux di valutazione). Nessun altro flag di
compilazione (warning, asserzioni della standard library) è rilassato.

---

## 6. Istruzioni di compilazione ed esecuzione

Configurazione e build (come da consegna):

```shell
cmake -S . -B build -G"Ninja Multi-Config"
cmake --build build --config Debug
cmake --build build --config Debug --target test
cmake --build build --config Release
cmake --build build --config Release --target test
```

Esecuzione dell'applicazione grafica:

```shell
./build/Release/game     # oppure ./build/Debug/game
```

### 6.1 Esecuzione dei test

I comandi `--target test` qui sopra compilano *e* lanciano gli unit test.
Per eseguirli separatamente dopo la build ci sono tre modi.

**Via CTest** (lancia tutti i test della configurazione e ne riassume l'esito):

```shell
ctest --test-dir build --build-config Debug --output-on-failure
ctest --test-dir build --build-config Release --output-on-failure
```

`--output-on-failure` stampa l'output completo di doctest solo per i test
falliti. Per vedere i nomi di tutti i casi eseguiti aggiungere `-V`.

**Via target CMake** (build + run in un solo comando):

```shell
cmake --build build --config Debug --target test
```

**Eseguendo direttamente i binari** (utile per i flag di doctest, es. filtri):

```shell
./build/Debug/test_vec3
./build/Debug/test_boid
./build/Debug/test_boid --test-case="*separazione*"   # filtro doctest
```

I tre eseguibili prodotti sono:

- `game` — applicazione grafica;
- `test_vec3`, `test_boid` — unit test (eseguibili anche via `ctest`).

---

## 7. Input e output

**Input.** L'applicazione non legge parametri da file o da linea di comando:
tutti i parametri si impostano dal menu grafico iniziale.

- Numero di boid per specie: interi nell'intervallo `[0, MAX_PER_BREED]`
  (validato dal widget `GuiValueBox`). La somma totale è limitata a 800; oltre
  tale soglia la simulazione non parte.
- Rotazione orbitale della telecamera: *checkbox* on/off.

A simulazione avviata, i quattro slider regolano i pesi globali nell'intervallo
`[0.0, 3.0]`.

**Output.** L'output è esclusivamente grafico: la scena 3D con i boid (coni
colorati per specie), il box di simulazione e l'HUD con FPS e statistiche per
specie (numero, velocità media, velocità massima, parametro d'ordine).

---

## 8. Interpretazione dei risultati

Con i parametri di default si osserva l'emergere spontaneo di stormi coesi e
ordinati a partire da posizioni e direzioni casuali. Aumentando il peso della
**coesione** gli stormi si compattano; aumentando la **separazione** si dispergono;
la **paura** governa quanto nettamente le prede evitano pareti e gabbiani. Il
**parametro d'ordine** nell'HUD quantifica il livello di allineamento dello
stormo (vicino a 1 = moto coerente, vicino a 0 = disordine).

Poiché allineamento e coesione agiscono solo tra individui della stessa specie,
mentre la separazione vale tra tutti, le tre specie formano stormi distinti che
si compenetrano evitando le collisioni; i gabbiani perturbano localmente gli
stormi delle prede.

---

## 9. Uso di sistemi di Intelligenza Artificiale generativa

<!-- Dichiarazione obbligatoria: completare con sincerità.
     Esempio: "Sono stati usati strumenti di IA generativa come supporto alla
     stesura della relazione e alla revisione del codice; la logica e
     l'implementazione sono state comprese e verificate dagli autori." -->

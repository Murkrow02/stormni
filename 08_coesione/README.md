# Step 08 — Regola 3: Coesione → MURMURATION

## Obiettivo

Aggiungere la **terza e ultima regola di Reynolds**. Con tutte e 3 attive, dopo qualche secondo i 100 boid formano uno o più stormi che danzano in modo coerente. È la **murmuration**.

Questa è la pagina più importante del tutorial: qui appare il fenomeno emergente che hai inseguito dall'inizio.

## Fisica: andare verso il baricentro dei vicini

Per ogni boid `i`, calcola il **centro di massa locale**: la media delle posizioni dei vicini entro `R_VIEW`. Sterza verso di esso:

```
p_media = (1/k) · Σ_{j vicini} p_j
steer_coes_i = p_media - p_i
```

Combinato con le altre due regole:

```
a_i = W_SEP · steer_sep_i + W_ALL · steer_all_i + W_COES · steer_coes_i
v_i ← clamp(v_i + a_i · dt, max_speed)
p_i ← p_i + v_i · dt
```

Tre forze concorrenti:

- **Separazione** spinge fuori dai gruppi troppo stretti (raggio piccolo, peso alto)
- **Allineamento** uniforma le direzioni (raggio medio, peso medio)
- **Coesione** raduna (raggio medio, peso basso-medio)

Quando i pesi sono **ben bilanciati**, il sistema si auto-organizza: emerge uno stormo. Quando NO, hai esplosioni, collassi o moto random.

## Cosa devi fare

Partendo dallo step 07:

1. **TODO 1**: aggiungi `const float W_COES = 1.0f`.
2. **TODO 2**: scrivi `Vec2 calcola_coesione(const std::vector<Boid>& stormo, int i)`:
   - Accumula la somma delle posizioni `p_j` dei vicini in `R_VIEW`, e conta `k`.
   - Se `k > 0`, ritorna `p_media - p_i`. Altrimenti `{0,0}`.
3. **TODO 3**: nel loop, calcola e somma anche `coes[i]`:
   ```
   acc = W_SEP·sep + W_ALL·all + W_COES·coes
   ```

> **Suggerimento didattico.** Per capire ciascuna regola, prova ad **attivarne una alla volta**: metti `W_ALL = 0; W_COES = 0;` e vedi solo separazione. Poi solo allineamento. Poi solo coesione. Poi tutte e tre. Ti rende l'idea di cosa fa ciascuna.

## Verifica

Con i parametri di default (`R_SEP=20, R_VIEW=50, max_speed=200, w_sep=1.5, w_all=1.0, w_coes=1.0`), dopo 10-20 secondi vedrai:

- Boid che si raggruppano in uno o più "stormi"
- Stormi che si muovono coerentemente in una direzione
- Quando si incrociano, mescolano o si dividono
- Nessuna collisione persistente: la separazione mantiene la spaziatura

Questa è la **murmuration**.

## Domande per riflettere

- **Ordine vs disordine.** L'ordine emerge da regole locali, senza nessun "direttore". È un esempio di *self-organization*, centrale nei sistemi complessi (es. transizioni di fase, mercati, neuroni). Cosa accomuna tutti questi sistemi?
- **Parametro d'ordine.** Una grandezza che quantifica quanto lo stormo è "ordinato" è il modulo della **velocità media normalizzata**: `Φ = | (1/N) Σ_i v_i / |v_i| |`. Φ ≈ 0 → caos. Φ ≈ 1 → tutti vanno nella stessa direzione. Potresti stamparlo a console (`printf`) e vedere come evolve nel tempo.
- **Studi reali.** Negli storni veri ogni uccello interagisce con esattamente ~6-7 vicini, **indipendentemente dalla distanza** (Ballerini et al. 2008, "topological vs metric"). Questo è diverso dal nostro modello "metrico" (raggio fisso). Vorrebbe dire usare il *k*-nearest neighbour invece del raggio. Sarebbe un esercizio interessante: cambierebbe la stabilità dello stormo a densità diverse?
- **Cose che non abbiamo modellato.** Inerzia rotazionale, predatori, gravità, vento, paura, fame. Sono tutti possibili miglioramenti.

## Bonus

Senti di voler giocare? Prova questi esperimenti:

1. Cambia N a 500 o 1000 (potrebbe rallentare: l'algoritmo è O(N²)).
2. Imposta `W_SEP = 5.0` o `W_COES = 5.0` (rotture di equilibrio).
3. Disattiva la separazione (`W_SEP = 0`): cosa succede? (I boid collassano in un punto.)
4. Disattiva la coesione (`W_COES = 0`): cosa succede? (Si allineano ma non si raggruppano.)

## Quando hai finito

Se vedi gli stormi danzare → **avanza a `09_parametri/`** per regolare tutto in tempo reale con slider.

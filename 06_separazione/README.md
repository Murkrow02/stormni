# Step 06 — Regola 1: Separazione

## Obiettivo

I boid **si evitano**: se due sono troppo vicini, sterzano per allontanarsi. Senza ancora le altre regole.

Visivamente: i boid si "respingono" come palline magnetiche dello stesso polo, formando un pattern sparso ma non più uniforme.

## Fisica: la sterzata come accelerazione

Da qui in poi calcoleremo, per ciascun boid `i`, un **vettore di sterzata** `steer_i` (una specie di forza). Lo applichiamo come accelerazione:

```
a_i = w · steer_i               (w = peso della regola)
v_i ← v_i + a_i · dt
v_i ← clamp(v_i, max_speed)     (limita la velocità massima)
p_i ← p_i + v_i · dt
```

Il `clamp` serve perché altrimenti i boid accelerano all'infinito. Negli storni reali c'è un limite biologico alla velocità di volo (~20-25 m/s).

## La regola di separazione

Per ogni boid `i`, guarda i vicini entro un raggio `R_sep` (piccolo, ~20 pixel). Per ciascun vicino `j`, calcola il vettore `p_i - p_j` (punta **lontano** da `j`). Più sono vicini, più questo deve pesare → divide per la distanza:

```
steer_sep_i = Σ_{j vicini} (p_i - p_j) / |p_i - p_j|²
```

Oppure più semplicemente, somma i versori `normalize(p_i - p_j)` divisi per la distanza. Esistono mille varianti; la nostra:

```cpp
Vec2 spinta = {0, 0};
for ogni j != i:
    float d = dist(stormo[i].pos, stormo[j].pos);
    if (d > 0 && d < R_sep):
        Vec2 diff = sub(stormo[i].pos, stormo[j].pos);
        spinta = add(spinta, mul(normalize(diff), 1.0f / d));
```

`spinta` è il `steer_sep_i`. Poi lo applichiamo come accelerazione.

## Cosa devi fare

1. **TODO 1**: definisci `const float R_SEP = 20.0f`, `const float MAX_SPEED = 200.0f`, `const float W_SEP = 1.5f`.
2. **TODO 2**: scrivi una funzione `Vec2 calcola_separazione(const std::vector<Boid>& stormo, int i)` che ritorna `spinta`.
3. **TODO 3**: nel loop principale, **prima** dell'integrazione di posizione:
   - per ogni boid: `steer = calcola_separazione(stormo, i)`
   - `stormo[i].vel = add(stormo[i].vel, mul(steer, W_SEP * dt))`
   - clamp della velocità: se `norm(vel) > MAX_SPEED`, riportala a `MAX_SPEED` con `vel = mul(normalize(vel), MAX_SPEED)`.

> **Attenzione: due passaggi separati!**
> Calcola **prima** la sterzata di tutti i boid (usando lo stato vecchio), **poi** aggiorna le velocità e le posizioni. Se aggiorni dentro lo stesso loop, i boid letti dopo "vedono" già lo stato nuovo dei precedenti, e l'ordine influenza il risultato. Tecnicamente non rovina lo stormo, ma non è pulito.
>
> Più semplice: salva tutte le sterzate in un vettore `std::vector<Vec2> sterzate(N)`, poi applica.

## Verifica

I 100 boid non si "appiccicano" più: ogni volta che due si avvicinano, deviano. Il movimento globale resta caotico (manca allineamento e coesione).

## Domande per riflettere

- Se aumenti `R_SEP` a 100, cosa succede? (Risposta: spinta repulsiva enorme, i boid esplodono verso i bordi.)
- Se `W_SEP` è troppo basso (es. 0.1), che vedi? (Risposta: la regola non si nota, sembra ancora moto random.)
- Perché diviso `d` (distanza) e non `d²`? In teoria `1/d²` è più fisico (tipo Coulomb), `1/d` è una scelta di Reynolds, più "dolce". Provale entrambe!

## Quando hai finito

→ **avanza a `07_allineamento/`**.

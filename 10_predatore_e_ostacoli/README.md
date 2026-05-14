# Step 10 — Predatore e Ostacoli (Bonus)

## Obiettivo

Aggiungere due elementi al mondo:

1. **Un predatore** (un falco rosso) che si muove col mouse. I boid devono fuggire quando entra nel loro raggio di percezione.
2. **Ostacoli** circolari fissi (palazzi/alberi) che i boid devono evitare.

Visivamente: muovendo il mouse sopra lo stormo, si vede un "buco" aprirsi e i boid scappano in ondate. Gli ostacoli funzionano come "scogli" davanti a cui il flusso si divide.

## Fisica: due nuove regole

### Regola di fuga (predatore)

Per ogni boid `i`: se il predatore è entro `R_FUGA` (es. 80 pixel), aggiungi una sterzata **opposta** al predatore, di intensità grande:

```
steer_fuga_i = normalize(p_i - p_predatore) · F_FUGA       se entro R_FUGA
              = 0                                          altrimenti
```

`F_FUGA` è il peso (molto alto, es. 5-10): la paura batte tutto.

### Repulsione da ostacoli

Stessa logica della separazione, ma rispetto ai centri degli ostacoli, con raggio extra pari al **raggio dell'ostacolo** + un margine di sicurezza.

```
steer_ost_i = Σ_{ostacoli k entro distanza} normalize(p_i - p_k) / d
```

## Cosa devi fare

Partendo dallo step 09:

1. **TODO 1**: aggiungi parametri:
   - `const float R_FUGA = 80.0f;`
   - `const float W_FUGA = 5.0f;`
   - `const float W_OST  = 2.0f;`
2. **TODO 2**: aggiungi un vettore di ostacoli:
   ```cpp
   struct Ostacolo { Vec2 pos; float raggio; };
   std::vector<Ostacolo> ostacoli = {
       { {200, 300}, 40 },
       { {500, 200}, 60 },
       { {600, 450}, 30 }
   };
   ```
3. **TODO 3**: nel loop, leggi posizione mouse: `Vec2 predatore = { (float)GetMouseX(), (float)GetMouseY() };`.
4. **TODO 4**: scrivi `calcola_fuga(boid, predatore)` e `calcola_ostacoli(boid, ostacoli)`. Sommale alla sterzata totale con i loro pesi.
5. **TODO 5**: disegna il predatore (cerchio ROSSO grande) e gli ostacoli (cerchi GRAY).

## API raylib utili

| Funzione | A cosa serve |
|----------|--------------|
| `GetMouseX()`, `GetMouseY()` | Posizione del puntatore |
| `DrawCircle(x, y, r, RED)` | Predatore |
| `DrawCircleLines(x, y, r, RED)` | Cerchio non pieno, per il raggio di fuga (debug) |

## Verifica

- Boid danno una **rotta uniforme** quando non c'è nulla.
- Sposti il mouse sui boid: si aprono come un banco di pesci sotto attacco.
- Boid che si avvicinano agli ostacoli grigi deviano e li girano intorno.

## Domande per riflettere

- Negli storni reali, il predatore (falco pellegrino) attacca e lo stormo reagisce con onde di paura che si propagano molto più veloci della velocità di volo di un singolo uccello. È studiato sperimentalmente — un'onda di "manovra" può propagarsi a 90 m/s anche se i singoli storni volano a 12 m/s. Il nostro modello cattura qualcosa di analogo? (In parte sì: l'allineamento + fuga propagano la deviazione tramite i vicini.)
- Cosa succede se metti `W_FUGA = 100`? E `W_FUGA = 0.5`?
- Cosa cambierebbe se gli ostacoli avessero forma diversa (rettangoli, polilinee)? Bisogna calcolare la distanza punto-forma, non più punto-punto.

## Quando hai finito

→ vai a `11_3d_bonus/` per il 3D.

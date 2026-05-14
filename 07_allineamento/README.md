# Step 07 — Regola 2: Allineamento

## Obiettivo

I boid **si allineano** alle direzioni dei vicini. Questa è la regola che fa "girare insieme" lo stormo: senza, ognuno andrebbe per fatti propri.

Visivamente: nuvolette di boid che cominciano a muoversi tutti nella stessa direzione, anche se ancora non si raggruppano (manca la coesione).

## Fisica: media delle velocità

Per ogni boid `i`, guarda i vicini entro un raggio di **visione** `R_VIEW` (più grande di `R_SEP`, ~50 pixel — l'uccello "vede" più lontano di quanto stia "stretto"). Calcola la **velocità media** dei vicini, e sterza verso di essa:

```
v_media = (1/k) · Σ_{j vicini} v_j     (k = numero vicini)
steer_all_i = v_media - v_i
```

`steer_all_i` è la "correzione di rotta" verso la direzione media del gruppo locale.

> **Nota.** Reynolds nel paper originale normalizza `v_media` a `max_speed` prima di sottrarre `v_i`. È una sottigliezza: senza, l'allineamento dipende dal modulo delle velocità dei vicini, non solo dalla direzione. Per il nostro tutorial non normalizziamo (più semplice e funziona lo stesso).

## Cosa devi fare

Parti dal codice dello step 06.

1. **TODO 1**: aggiungi `const float R_VIEW = 50.0f` e `const float W_ALL = 1.0f`.
2. **TODO 2**: scrivi `Vec2 calcola_allineamento(const std::vector<Boid>& stormo, int i)`:
   - Accumula la somma delle velocità dei vicini in `R_VIEW`, e conta `k`.
   - Se `k > 0`, calcola `v_media = somma / k`, e ritorna `v_media - v_i`.
   - Altrimenti, ritorna `{0,0}`.
3. **TODO 3**: nel loop, somma anche questa sterzata: `sterzate[i] = add(sep, mul(all_steer, W_ALL / W_SEP))`. Più semplice: tieni due array separati `sep_i` e `all_i`, poi `vel += (W_SEP*sep + W_ALL*all) * dt`.

## Verifica

I 100 boid formano gruppetti che si muovono coerentemente nella stessa direzione, anche se ancora dispersi. Tipicamente vedi 2-3 "correnti" di boid che vanno insieme.

## Domande per riflettere

- Cosa succede se metti `R_VIEW = R_SEP`? (Le 2 regole agiscono sullo stesso raggio → competizione, comportamento meno chiaro.)
- Cosa succede se `W_ALL >> W_SEP`? I boid si accavallano perché vince l'allineamento sulla separazione. **Le regole vanno bilanciate.**
- Fisicamente: cosa garantisce che `v_media` sia un buon "consenso" della direzione del gruppo? È una media aritmetica di vettori. Per direzioni angolari sarebbe meglio una **media circolare** (media dei coseni e seni separati), ma con vettori 2D le cose semplici funzionano.

## Quando hai finito

Manca solo una regola e poi vedrai la magia.

→ **avanza a `08_coesione/`**.

# Step 05 — Bordi: Wrap Toroidale

## Obiettivo

Far rientrare i boid dal lato opposto quando escono dalla finestra. Lo schermo diventa un **toro**: come Pac-Man.

## Fisica: perché wrap e non "rimbalzo"?

Due opzioni quando un boid esce:

1. **Rimbalzo** (riflessione speculare): inverti la componente della velocità (`vel.x = -vel.x` se esce a destra/sinistra). È fisicamente più naturale (parete elastica) ma crea bordi "speciali" che possono catturare boid e disturbare la dinamica dello stormo.
2. **Wrap toroidale**: il boid riappare dal lato opposto, mantenendo la velocità. Lo spazio simulato non ha bordi, è un **toro 2D**. Più "pulito" per studiare dinamiche emergenti senza artefatti di bordo.

Useremo il **wrap**. È quello che usano quasi tutte le simulazioni di flocking accademiche (anche le simulazioni di gas ideale spesso usano *periodic boundary conditions*, è la stessa idea).

> **Nota fisica.** Nello studio STARFLAG sugli storni di Roma, ovviamente, i bordi sono il cielo libero e gli stormi non rientrano da nessun lato. Ma in una simulazione 2D finita ci serve un trucco per non perdere boid: il wrap è il più indolore.

## Cosa devi fare

Partendo dal codice dello step 04:

1. **TODO 1**: dopo l'aggiornamento di `pos`, controlla i 4 lati:
   - se `pos.x < 0`        → `pos.x += LARGHEZZA`
   - se `pos.x > LARGHEZZA` → `pos.x -= LARGHEZZA`
   - analogo per `pos.y` con `ALTEZZA`

Usa `+=` e `-=` (non `= 0`!) così la velocità residua non viene persa.

## Verifica

I 100 boid si muovono random e nessuno sparisce: quando uno tocca un bordo, riemerge dall'altro. Se aspetti, vedrai uno schermo "pieno" di boid distribuiti casualmente.

## Domande per riflettere

- Cosa succede ai boid se mettessi `pos.x = 0` invece di `pos.x += LARGHEZZA`? (Risposta: si accumulerebbero progressivamente, perché perdono lo "scarto" oltre il bordo.)
- Più avanti, quando calcoleremo distanze fra boid, il wrap creerà un problema: due boid vicini ai bordi opposti sono in realtà **vicini** (passando "dall'altro lato"). Tipicamente lo si ignora per semplicità. Lo ignoreremo anche noi. È un'approssimazione accettabile finché la finestra è molto più grande del raggio di interazione `R_view`.

## Quando hai finito

→ **avanza a `06_separazione/`**.

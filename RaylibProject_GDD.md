# Game Design Document (WIP)

# Core Concept
* Top‑down, 2D auto‑attacker “survivor” game: player only controls movement, all weapons fire automatically on timers.
* Goal of a run: survive a fixed duration (e.g. 15–20 minutes) against escalating enemy waves, or die trying.
* Focus: process management and build crafting rather than twitch aiming (positioning, pathing, and upgrade choices).

# High-Level Gameplay Loop
## Moment‑to‑moment loop within a run:
* Move to kite enemies and avoid contact damage.
* Auto‑attacks trigger on cooldowns, killing enemies in range.
* Enemies drop XP orbs and occasional pickups (heals, currency).
* Collect XP to level up.
* On level up, pause and choose one of 3–4 random upgrades.
* Repeat with denser, tougher waves until time expires or HP hits zero.

## Meta‑loop between runs:
* Spend gold from previous runs on permanent upgrades and/or new characters.
* Unlock new stages or weapons after reaching milestones (e.g. “survive 10 minutes”).

# Systems Overview
## Player and Stats
* Player has: max HP, move speed, magnet radius, armor, might (damage), cooldown reduction, etc.
* Starts with one basic weapon depending on chosen character (e.g. “Bolt” projectile, “Aura” ring).
* Takes touch damage from enemies; contact‑based, no enemy projectiles initially.​
* Upgrade sources: Level‑up choices (in‑run), permanent meta upgrades (between runs).

## Weapons and Upgrades
* Weapons auto‑fire based on their pattern: radial burst, orbiting projectiles, short‑range aura, etc.
* Each weapon has: damage, cooldown, number of projectiles, speed, area, lifespan
* Weapon also has level. Leveling a weapon increases these stats and possibly adds behaviors (extra projectiles, pierce, etc.). TL;DR: higher level, better weapon.
* Stretch goal: evolved weapons that require a specific weapon + passive combo, unlocked at max level.

## Enemies and Waves
Enemies spawn continuously with strength and density scaling over time.

### Types:
* Trash mobs (low HP, swarm).
* Elites (high HP, give big XP/gold or a treasure).
* Stretch goal: boss at the end‑time mark.

### Spawn system:
* Time‑based wave script (e.g. minute 0–3: type A, minute 3–5: type B, etc.)
* Parameters per wave: spawn rate, enemy type, max concurrent count, maybe directional pushes.

## Progression and Economy
In‑run XP: dropped as gems; level thresholds grow over time.

### In‑run pickups:
* Healing item (rare)
* Currency (e.g. gold)
* Powerups (e.g. double XP for 30 seconds, etc).

### Meta‑currency (gold): persistent across runs; spent on:
* New characters with different starting stats/weapons.
* Global upgrades (max HP +5%, move speed +5%, etc.).
* Stretch goal: cosmetic customizations

# Session and Difficulty Structure
Target run length: 15–20 minutes to keep scope realistic and to mirror the classic pacing (early weak phase → mid chaos → late power fantasy).

## As difficulty increases:
* Higher spawn density.
* Higher enemy HP and move speed.
* More elites at fixed times.

## End conditions:
* Survive until time limit → win screen, bonus gold, maybe unlock.
* Die before time limit → lose screen, smaller gold payout, still some meta progression.
* Stretch goal: “end‑screen reaper” equivalent (unbeatable late‑spawn enemy) as a clear run terminator.

# Technical / Pattern Targets
These are hooks for the design patterns unit, not full designs yet:

## Game States: 
* Title, Meta‑Upgrades, Character Select, In‑Run, Pause, Results.
* Candidate patterns: State pattern for high‑level game flow.

## Entities (player, enemies, projectiles, pickups):
* Factory / Abstract Factory to spawn enemies and bullets by type.
* Object Pool for projectiles and maybe enemies (high churn, perf sensitive).

## Weapons and upgrades:0
* Strategy or Component‑based approach for different firing patterns and behaviors.
* Data‑driven configs (JSON / structs) where possible to separate data from code.

## Events:
* Observer pattern for “on level up”, “on enemy killed”, “on pickup collected” to decouple systems.

## Meta progression:
* Simple Repository / Manager singleton for unlock data and persistent upgrades.

# Milestones
## MVP (Minimum Viable Product)
Target: A 2-minute playable run showing core loop + 3 patterns. Submit-ready for Pass G (all learning outcomes).​

### Core Features:
* WASD player movement with HP bar.
* 1 enemy type spawning continuously (time-based Factory pattern).
* 1 auto-fire weapon (e.g. radial burst on cooldown; Strategy pattern).
* XP gems from dead enemies, auto-magnet to player, level-up → choose 1 of 3 upgrades (pause state).
* Win condition: survive 2 minutes. Lose: HP=0. Basic title/results screens (State pattern).

## Patterns Demonstrated:
* State: Game states (Title → Playing → Paused/Upgrade → Results).
* Factory: Enemy/XP spawns.
* Strategy: Single weapon behavior swap via upgrade.

## Raylib Tech:
* Rectangles/circles for visuals.
* GetFrameTime() for smooth updates.
* CheckCollisionRecs() for basic overlaps.

Time Est.: 1-2 weeks solo.

## Milestone 1: Core Loop
* Player moves, enemies spawn/chase, weapon auto-fires, XP collects → levels.
* No upgrades or states yet—just raw loop.
* Test: Survive 30 seconds without patterns (procedural baseline).

## Milestone 2: Patterns Foundation (MVP Core)
* Add State pattern for screens, Factory for spawns, Strategy for weapon.
* Now playable with upgrade pause.
* Test: Full 2-min run with 1 upgrade choice.

## Iteration 1: Polish + Extra Patterns
* Extend to 5-min runs, add Object Pool (projectiles/enemies), Observer (death/XP events).
* 2 enemy types, 2 weapons.
* Meta-currency stub (gold on death, simple Singleton manager).
* Test: Multiple runs showing perf benefits of pooling.

## Stretch Goals (VG Distinction)
If time:
* Component for modular entities, Command for inputs/upgrades, evolved weapons.
* 15-min runs, elites, healing pickups.
* Data-driven waves (structs/enums, no JSON).
​

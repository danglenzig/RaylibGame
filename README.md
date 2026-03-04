## Raylib Vampire Survivor-Style Clone

This project is a **scoped-down Vampire Survivor-style game** built in **C++ with raylib** for a Game Programming course assignment.  
The focus is not just on gameplay, but on **applying and demonstrating object-oriented design patterns**.

## Assignment Context

- **Course assignment**: Implement a small clone of *Vampire Survivor* with a subset of its core features.
- **Learning goal**: By the end, I should be able to:
  - **Describe** several design patterns.
  - **Identify** where they are useful in a game.
  - **Explain / fix** issues that arise when patterns are not used.

This project is my implementation of that assignment, with an emphasis on clean architecture and pattern usage rather than full feature parity.

## Current Game Overview

- **Engine / Library**: C++ with `raylib` and `raymath`.
- **Window**: 1280×720 at 60 FPS.
- **Player**:
  - Controlled with **WASD** movement.
  - Faces the direction of movement.
  - Has **IDLE**, **WALKING**, and **ATTACKING** animation states.
  - Left mouse click triggers an **attack** with a cooldown.
- **Enemies**:
  - Spawn periodically at predefined points around the edges of the screen.
  - Move toward / around the player using slightly randomized paths.
  - Are animated via a simple frame-based system.
  - Can be killed either by:
    - Entering the player’s **attack sector** (a cone-shaped area in front of the player), or
    - Getting too close and “self-destructing” (kamikaze radius).
  - A quick **hit marker** effect is drawn when an enemy dies.
- **HUD**:
  - Current **enemy count** is displayed on screen.

The game loop lives in `main.cpp` and delegates almost all logic to systems and objects (player, enemies, render system, update relay, etc.).

## Design Patterns Used (So Far)

- **Finite State Machine (FSM) pattern**
  - Implemented in `StateMachine.h` (`FSM`, `FSM_State`, `FSM_Events`).
  - The `Player` owns an `FSM` instance that manages named states (`IDLE`, `WALKING`, `ATTACKING`) and allowed transitions.
  - The FSM also emits **state enter/exit events**, which the player listens to in order to:
    - Switch animations when entering a new state.
    - Trigger gameplay effects such as spawning an attack sector when entering `ATTACKING`.
  - This separates **state management** from **state-specific behavior**, improving clarity vs. a big if/else/switch in the update loop.

- **Observer / Event pattern**
  - **Frame updates**:
    - `UpdateRelay` keeps a list of listeners and **broadcasts delta time each frame**.
    - `Player` and `EnemySystem` register callbacks, so the main loop just calls `UpdateRelay::Dispatch(deltaTime)` once.
  - **Input events**:
    - `InputHandler` centralizes input and holds a list of **attack listeners**.
    - When the left mouse button is pressed, all registered listeners are notified (e.g., the `Player`’s attack handler).
  - **Animation events**:
    - `Animator` fires events like `STARTED`, `FRAME_UPDATED`, `LOOPED`, and `FINISHED` via listeners.
    - The `Player` uses these events (for example, to transition back to IDLE when the attack animation finishes).
  - **Enemy death callbacks**:
    - `Enemy` objects expose `SetDeathCallback`, which `EnemySystem` uses to be notified when an enemy dies.
    - This decouples **death detection** (inside `Enemy`) from **pool management and visual effects** (inside `EnemySystem`).

- **Factory Method / Simple Factory (enemy creation)**
  - `EnemySystem::SpawnEnemy(EnumEnemy enemyType, Vector2 spawnPos)` acts as a **factory method** for `Enemy` objects.
  - The caller only specifies **which enemy type** and **where** to spawn; `EnemySystem` decides:
    - Which sprite frames to use (via an internal frame pool).
    - How to configure animation fps, size, speed, and a unique ID.
    - How to wire up the **death callback** so that the system can remove enemies and spawn hit markers.
  - This centralizes enemy construction logic in one place, which makes it easier to:
    - Add new enemy types by extending the configuration instead of scattering `new Enemy(...)` calls.
    - Change how enemies are initialized without touching game loop code.
  - While `EnemySystem` also manages updating and rendering enemies, its `SpawnEnemy` function is a valid, pattern-aligned example of a **simple factory method**.

- **Inheritance-based GameObject hierarchy (ECS-inspired)**
  - `GameObject` is a base class with shared **draw data** (`position`, `size`, `lookAngle`, `Texture2D*`).
  - `Player` and `Enemy` inherit from `GameObject` and override:
    - `OnFrameUpdate` for per-frame logic.
    - `FixDrawData` to keep the render data in sync.
  - `RenderSystem` takes a `GameObject*` and draws it using only the `GameObjectDrawData` struct, decoupling rendering from game logic.

- **Flyweight pattern (shared textures / sprite frames)**
  - Individual enemies **do not load or own textures**; they only know which animation frame index they are on (plus their transform).
  - `EnemySystem` loads each enemy sprite frame **exactly once** into an internal pool and reuses those `Texture2D` objects across all enemies of that type.
  - `RenderSystem` uses the shared texture pointer along with each enemy’s draw data (position, size, angle) to render.
  - This matches the Flyweight idea of separating **intrinsic state** (shared image data) from **extrinsic state** (per-enemy position/orientation/frame index), which reduces memory usage and keeps rendering concerns centralized.

As the project continues, I plan to **lean further into these patterns** (and potentially add more, like a more fully featured `CombatSystem` and additional systems/components).

## Project Structure (High-Level)

- `main.cpp` — Sets up the window, main loop, and high-level orchestration (player, enemy system, rendering).
- `GameObject.h` — Base class for drawable/updatable entities.
- `Player.h` — Player-specific data, animations, input handling, and FSM integration.
- `Enemy.h` — Enemy behavior, basic AI movement, animation frame tracking, and reaction to player attacks.
- `EnemySystem.h` — Spawning, updating, rendering, and killing enemies; manages enemy pool and hit markers, and acts as a **factory** for `Enemy` instances.
- `RenderSystem.h` — Centralized rendering of `GameObject` instances using raylib.
- `StateMachine.h` — Reusable finite state machine implementation with events.
- `Animator.h` / `Animation.h` — Simple 2D animation system using sprite frames and timer-based updates.
- `InputHandler.h` — Abstraction over raw raylib input; provides movement vector and attack events.
- `UpdateRelay.h` — Global frame update dispatcher for anything that needs per-frame callbacks.
- `CombatSystem.h` — Placeholder for a more explicit combat/damage system (planned).

## Controls

- **Movement**: `W`, `A`, `S`, `D`
- **Attack**: Left mouse button (fires an attack with a cooldown; enemies in front of the player and inside the attack sector can be killed)

## Building and Running

- **Requirements**:
  - A C++ compiler and **Visual Studio** (project files included: `RaylibGame.vcxproj`, `RaylibGame.slnx`).
  - **raylib** installed and configured for C++ (include directories, library directories, and linker input set up in the project).
- **To run**:
  - Open the project in Visual Studio.
  - Select the `x64` configuration (Debug or Release).
  - Build and run the `RaylibGame` target.
  - Make sure the `Resources` folder (sprites, etc.) is available relative to the executable so textures can load correctly.

## Status and Future Work

- **Implemented**:
  - Basic movement and animation for the player.
  - Enemy spawning, simple AI movement, and destruction.
  - Finite state machine for the player.
  - Global update relay and multiple observer-style systems (input, animation, enemy death callbacks).
  - A simple factory method for centralizing enemy creation inside `EnemySystem`.
- **Planned / In progress**:
  - Fill out `CombatSystem` for clearer damage and health management.
  - Improve balancing (spawn rates, enemy behavior, difficulty curve).
  - Add additional enemy types and player progression (levels, stats).
  - More visual polish and audio.

As I continue working on the assignment, I’ll update this README to better explain any new patterns and features I add.

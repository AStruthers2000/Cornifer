# Copilot Instructions — Unreal Engine 5, Modern C++, Modular Game Design

Purpose
- Guide Copilot completions toward UE5 best practices, idiomatic modern C++, and modular, testable game architecture.
- Prioritize safety, performance, and maintainability.

General rules
- Prefer clarity and correctness over cleverness.
- Keep functions short (<= 60 lines), single responsibility.
- Add brief comments for intent, not for obvious code.
- Favor compile-time checks (constexpr, templates) and RAII for resource management.
- Use consistent UE naming/conventions: UClass names start with 'U', Actor classes with 'A', Components with 'U' + suffix 'Component', interfaces with 'I'.

Unreal-specific guidance
- Use UPROPERTY / UFUNCTION where reflection/GC/replication is required. Mark with minimal necessary specifiers (EditAnywhere vs EditDefaultsOnly).
- Prefer Components over inheritance for behavior composition.
- Avoid holding raw UObject* in non-UObject owners; use TWeakObjectPtr or TStrongObjectPtr when appropriate.
- For non-UObject data, use modern C++ containers (std::vector, std::array) or TArray for engine interoperability.
- Avoid calling NewObject/GetMutableDefault/ConstructObject except in designated factory/initialization code.
- Prefer BeginPlay / PostInitializeComponents for runtime setup rather than constructors for UObject-derived classes.
- Replication: mark properties with ReplicatedUsing when you need OnRep handlers; keep network authoritative logic on the server.
- Use FTimers / FTimerHandle instead of manual tick when possible. When using Tick, minimize work and guard with delta time accumulation.

Modern C++ guidance
- Use smart pointers (std::unique_ptr, std::shared_ptr) only for non-UObject ownership. For UObjects, rely on engine GC and weak/strong UE pointer types.
- Prefer auto, structured bindings, range-for, algorithms over manual loops.
- Use constexpr, noexcept, and explicit where appropriate.
- Prefer small value objects and pass by const& or by value for trivially copyable types.
- Avoid macros except when wrapping UE reflection macros; prefer inline functions or templates.

Modular game design
- Break features into modules/plugins. Modules should expose clear interfaces and hide implementation.
- Design gameplay features as data-driven: use DataAssets, DataTables, and config-driven behavior.
- Keep gameplay logic testable: separate pure logic (no UE API) into plain C++ modules to unit-test.
- Use message-driven or event-driven architecture (delegates, event buses) to decouple systems.
- Provide clear boundaries between systems (UI, Gameplay, Audio, IO) and minimal coupling.

Performance & safety
- Profile before optimizing. Use Unreal Insights and stat scopes.
- Minimize allocations in hot paths; prefer pooling for frequently created objects.
- Use thread-safe patterns for background work: FRunnable, AsyncTask, or the Task Graph. Never access UObjects from worker threads.
- Validate pointers and inputs; fail fast with ensureMsgf for dev-only checks.

Tooling, style & CI
- Configure clang-format (Google/LLVM style with UE tweaks) and run in pre-commit hooks.
- Run clang-tidy with modernize checks and address warnings.
- Use UnrealBuildTool (UBT) targets for CI builds; run EditorAutomation and packaged game tests in CI.
- Prefer incremental builds and targeted editor tests in CI to keep fast feedback.

Copilot prompt templates (use as starting text when requesting code)
- "Create an Unreal Engine 5 UActorComponent in modern C++ that manages X using components and exposes minimal UPROPERTY for configuration. Include BeginPlay and a safe tick implementation that avoids allocations."
- "Refactor this game system into a modular plugin: separate interface, implementation, and data assets. Keep engine API usage inside the implementation layer; make core logic pure C++ for unit tests."
- "Write a thread-safe background worker using AsyncTask that processes immutable data and dispatches results to the game thread using AsyncTask(ENamedThreads::GameThread, ...)."

Example quick checks for Copilot-generated code
- Does any background thread access UObject*? If yes — reject and add a marshal-to-game-thread fix.
- Are UPROPERTY specifiers minimal and correct for intended editor/replication behavior?
- Are allocations inside Tick or other high-frequency loops? If yes — suggest reuse/pooling.

Keep prompts minimal and explicit about ownership, threading, reflection needs, and testability.

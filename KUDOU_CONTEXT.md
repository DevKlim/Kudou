# Kudou Engine - AI Agent Context

This document provides a high-level overview of the Kudou (a Godot fork) source code, intended to guide an AI agent in understanding and modifying the engine for agentic game development tasks.

## Core Concepts

- **Nodes:** Everything in a game scene is a Node. Nodes are organized in a tree structure. The base class is `scene/main/node.h`. They have lifecycle callbacks like `_ready()` (on scene start) and `_process(delta)` (every frame).
- **Scenes:** A scene is a collection of nodes saved to a file (usually `.tscn`). They are blueprints that can be instanced within other scenes.
- **Resources:** Data containers like textures, models, sounds, and scripts. They are reference-counted and can be shared. The base class is `core/io/resource.h`.
- **Scripting:** Game logic is primarily written in GDScript, a Python-like language. The AI agent will mostly generate or modify `.gd` script files.
- **Signals:** A form of the observer pattern. Nodes can emit signals, and other nodes can connect to them to react to events. This is a primary way to decouple code.

## Key Source Code Directories

### `core/` - The Engine's Foundation

The `core` directory contains the fundamental building blocks. Understanding this is key to understanding the engine's API.

- **`core/object/`**: The heart of the C++ object system.
  - `object.h`: Defines `Object`, the base class for almost everything (including Nodes and Resources). It provides core features like properties, methods, and signals.
  - `class_db.h`: The "Class Database". It holds metadata for all registered engine classes, their methods, properties, and signals. This is essential for runtime introspection and can be programmatically queried to understand the engine's full API surface.
- **`core/variant/`**: Defines all core data types used by GDScript and the engine API (e.g., `Vector3`, `Color`, `String`, `Array`, `Dictionary`). The AI must use these types when interacting with the engine.
- **`core/io/`**: Handles file access and resource loading/saving.
  - `resource_loader.h`: API for loading resources from disk (e.g., `ResourceLoader::load("res://path/to/resource.tscn")`).
  - `resource_saver.h`: API for saving resources to disk.

### `scene/` - The Scene and Node System

This directory implements the scene graph and all the built-in game nodes. This is the primary sandbox for game creation.

- **`scene/main/node.h`**: The base `Node` class. It provides the tree structure, lifecycle callbacks, and core scene functionality. Key methods include `add_child()`, `get_node()`, `queue_free()`.
- **`scene/2d/`**: Contains all built-in 2D nodes (`Sprite2D`, `Camera2D`, `CharacterBody2D`, `Area2D`).
- **`scene/3d/`**: Contains all built-in 3D nodes (`Node3D`, `MeshInstance3D`, `Camera3D`, `CharacterBody3D`, `Area3D`).
- **`scene/resources/`**: Defines core resource types used in scenes, such as `PackedScene` (the C++ representation of a `.tscn` file), `Mesh`, `Material`, and `Animation`.

### `modules/gdscript/` - The GDScript Language

This is where the primary scripting language is implemented. It's a critical area for AI integration.

- **`gdscript.h/.cpp`**: The main class for the GDScript language singleton.
- **`gdscript_parser.cpp`**: The code parser that turns GDScript text into a token stream and parse tree.
- **`gdscript_compiler.cpp`**: Compiles the parse tree into bytecode for the VM.
- **`language_server/`**: The implementation of the Language Server Protocol (LSP). This is a prime integration point for providing AI features like advanced code completion, diagnostics, and code generation to any connected editor (including the built-in one).
  - `gdscript_language_server.h`: The main LSP server class.

### `editor/` - The Kudou Editor

This contains the C++ source code for the editor application itself. Adding agentic UI elements will require modifying files here.

- **`editor_node.h/.cpp`**: The main class that initializes and runs the entire editor UI. This is the root of the editor's scene tree.
- **`code_editor.h/.cpp`**: The text editor widget used for editing scripts. This is the primary place to add features like inline AI suggestions, a chat panel, or code transformations.
- **`editor_inspector.h/.cpp`**: The inspector dock UI. Can be extended to provide AI-driven suggestions for properties.
- **`filesystem_dock.h/.cpp`**: The file system dock. Can be extended with AI-powered asset generation or organization features.
- **`editor_interface.h/.cpp`**: A singleton that exposes editor functionality to scripts and plugins. It can be used to open scenes, get the current selection, and manipulate editor state.

### Build System

- **`SConstruct`**: The main SCons build script at the root.
- **`SCsub`**: Sub-directory build scripts. To add a new `.cpp` file to the engine, you must add it to the `sources` list in the relevant `SCsub` file and re-run the SCons build command.
# Integrated Development Plan: Kudou Engine Agent

This document provides a comprehensive guide for the MCP, detailing the relevant files and strategic approach for each phase of development.

## Phase 1: Foundational UI (The Agent Dock)

**Objective:** Establish the user-facing interface for the AI agent within the Godot editor.

This phase is purely about UI implementation. The MCP's focus should be on understanding how to create and register a new editor dock via an `EditorPlugin`.

### Key Files for This Phase:

-   **Files to be Created:**

    -   `editor/ai/agent_dock.h` & `.cpp`: Defines the `EditorPluginAgentDock` class. This plugin is responsible for creating its own UI (a `VBoxContainer` with chat history and input) and adding it as a dock to the editor. The correct integration is to use `EditorPlugin::add_control_to_dock()` in `NOTIFICATION_ENTER_TREE` and `EditorPlugin::remove_control_from_docks()` in `NOTIFICATION_EXIT_TREE`. This encapsulates the logic and ensures proper cleanup.
    -   `editor/ai/SCsub`: The SCons build script to compile the new `ai` directory.

-   **Files to be Modified (for integration):**
    -   `editor/editor_node.h` & `.cpp`: The main editor class. The MCP will need to read this to understand how `EditorPlugin`s are registered and activated. The plugin itself handles adding its UI components to the editor.
    -   `editor/SCsub`: The build script for the entire editor module. The MCP must add the new `ai/SCsub` to it.

## Phase 2: Core Agentic Functionality (Context & Communication)

**Objective:** Enable the agent to understand the user's project, parse relevant files, and communicate with external LLM APIs.

This is the most critical phase for data ingestion and contextual understanding. The MCP must learn to think like a Godot developer, understanding how scripts, scenes, and resources are interconnected.

### Key Files for This Phase:

#### 1. API Communication & Configuration

-   `core/io/http_client.h` & `.cpp`: The foundational, non-blocking class for making robust API requests to any web service or LLM API without freezing the editor.
-   `editor/editor_settings.h` & `.cpp`: To be studied and modified to create a new settings section for securely storing API keys and model preferences.
-   `core/os/os.h`: Contains `shell_open()`, which is essential for the "open in web chat" feature.

#### 2. File Content & High-Level Dependencies

-   `editor/filesystem_dock.h` & `.cpp`: To monitor user selections and identify the primary file for context gathering.
-   `core/io/file_access.h` & `.cpp`: The fundamental tool for reading the raw text content of any selected file.
-   `core/io/resource_loader.h`: Contains the vital `get_dependencies` method, which the MCP can use to quickly map out all files a given scene or script relies on.

#### 3. Deep Script & Scene Parsing (The Auto-Context Engine)

-   **GDScript:**
    -   `modules/gdscript/gdscript_parser.h` & `.cpp`: **Crucial.** This is the definitive source for GDScript grammar. The MCP must study this to build an Abstract Syntax Tree (AST) from script content, identifying variables, functions, signals, and dependencies.
    -   `modules/gdscript/gdscript_analyzer.h` & `.cpp`: The next level of understanding. This performs semantic analysis on the AST to resolve types and validate node paths.
-   **C#:**
    -   `modules/mono/`: The entire directory is relevant for understanding C# integration, especially how Godot attributes like `[Export]` and `[Signal]` are handled. The MCP should leverage an existing C# parser (like Roslyn) but use this context to understand how it glues to the engine.
-   **Scenes:**
    -   `scene/resources/packed_scene.h` & `.cpp`: The blueprint for `.tscn` files. The MCP will parse this text-based format to understand the scene hierarchy, node properties, and which scripts are attached to which nodes.

## Phase 3: Advanced Capabilities (Specialized Knowledge & Tooling)

**Objective:** Transform the agent into a specialized Godot expert that can provide documentation-aware answers and directly modify code.

### Key Files for This Phase:

#### 1. Documentation-Aware Chat (RAG Implementation)

-   **Source Data:**
    -   `doc/classes/*.xml`: The **golden source** of the engine's entire class reference. This is the knowledge base for the RAG system.
-   **Parsing & Retrieval:**
    -   `core/io/xml_parser.h` & `.cpp`: The tool the MCP will use to parse the `.xml` documentation files into a structured format.
    -   `core/string/fuzzy_search.h` & `.cpp`: A potential built-in tool for the retrieval step. The MCP can load key terms from the docs into this class to quickly find relevant topics. For more advanced semantic search, integrating a dedicated vector library would be the next step.
-   **Supporting Files:**
    -   `core/doc_data.cpp` & `.h`: Shows how the editor itself loads and processes this documentation, providing a working example for the MCP.

#### 2. Agentic Code Generation & Refactoring

-   **The Target - The Code Editor:**
    -   `editor/code_editor.h` & `.cpp`: The module for the main script editor. The MCP will need to interact with its internal `CodeEdit` node.
    -   `scene/gui/text_edit.h` & `.cpp`: The underlying class that provides the low-level API for programmatically inserting, deleting, and selecting text. This is how the agent will "write" code.
-   **Advanced Refactoring:**
    -   `modules/gdscript/gdscript_language_server.h` & `.cpp`: For advanced, language-aware operations, the MCP can interface with the Language Server Protocol (LSP). This allows for more robust actions like renaming symbols across files or triggering code formatting, moving beyond simple text manipulation.

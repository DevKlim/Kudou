# Development Roadmap: Kudou Engine Agent

This document outlines the phased development plan for integrating AI and agentic capabilities into the Kudou editor. The plan progresses from foundational UI to advanced, specialized AI systems.

## Phase 1: Foundational UI (The Agent Dock)

### 1.1. Objective

Create a new, dockable panel in the editor to serve as the primary user interface for all future AI and agentic features. This establishes the physical space where the user will interact with the AI.

### 1.2. Key Features

*   A dockable panel named "AI Agent" that can be positioned alongside the "Scene" and "Inspector" docks.
*   A basic UI layout inside the dock, containing a `RichTextLabel` for conversation history and a `TextEdit` or `LineEdit` for user input.

### 1.3. Relevant Files for This Phase

#### Files to be Created
*   `editor/ai/agent_dock.h`: Header defining the `AgentDock` class, inheriting from a `Control` node (e.g., `VBoxContainer`).
*   `editor/ai/agent_dock.cpp`: Implementation for `AgentDock`, responsible for creating and managing its child UI nodes.
*   `editor/ai/SCsub`: The SCons build script for the new `editor/ai` directory.

#### Files to be Modified
*   `editor/editor_node.h`: Add a member variable pointer for our dock: `class AgentDock; AgentDock *agent_dock;`.
*   `editor/editor_node.cpp`: In an initialization function (like `_ready` or `_initialize_docks`), instantiate `AgentDock`, give it a name and icon, and add it to a `DockSlot` (e.g., `dock_slot[DOCK_SLOT_RIGHT_UL]`).
*   `editor/SCsub`: Add `SConscript("ai/SCsub")` to include the new directory in the editor build module.

## Phase 2: Core Agentic Functionality (Context & Communication)

### 2.1. Objective

Bring the Agent Dock to life by enabling it to communicate with external LLM APIs and understand the user's current project context. This phase makes the agent useful.

### 2.2. Key Features

*   **API Integration:** Connect the dock's input to an external LLM API (e.g., OpenAI, Anthropic, or a local server) and display the response.
*   **File Context:** Allow the user to select a file in the `FileSystemDock` and have its contents automatically included in the prompt context.
*   **Auto-Context Parser:** Automatically analyze a selected GDScript file to find related scripts, attached nodes, and scene dependencies to build a more comprehensive context package for the LLM.
*   **Persistent Settings:** Create a new section in the Editor Settings to store API keys, model preferences, and other AI-related configurations.

### 2.3. Relevant Files for This Phase

#### Context & File Handling
*   `editor/filesystem_dock.h` & `.cpp`: To connect to its signals (like `file_selected`) to know what the user is clicking on.
*   `core/io/file_access.h` & `.cpp`: The core class for reading the raw text content of selected script files.
*   `modules/gdscript/gdscript_parser.h` & `.cpp`: **Crucial for auto-context.** Used to parse a script into an Abstract Syntax Tree (AST) to identify functions, variables, and dependencies.
*   `modules/gdscript/gdscript_analyzer.h` & `.cpp`: Works with the parser to perform deeper semantic analysis, resolving types and node paths (`$Node/Path`).
*   `scene/resources/packed_scene.h` & `.cpp`: To programmatically load and inspect `.tscn` files to understand the scene hierarchy and how scripts are attached to nodes.
*   `core/io/resource_loader.h`: Use its `get_dependencies` method to quickly find all files a given scene or script relies on.
*   **New File:** `editor/ai/context_parser.h` & `.cpp`: A new helper class to encapsulate the logic of using the above tools to build a final, concise context string.

#### API Communication & Configuration
*   `core/io/http_client.h` & `.cpp`: A low-level, non-blocking HTTP client in the C++ core, ideal for making robust API requests without freezing the editor.
*   `editor/editor_settings.h` & `.cpp`: To add and manage a new settings category (e.g., "AI" or "Kudou") for storing API keys and model choices securely.
*   `core/os/os.h`: To use `shell_open()` for the "open in web chat" feature, which would format a URL with the context and open it in the user's default browser.

## Phase 3: Advanced Capabilities (Specialized Knowledge & Tooling)

### 3.1. Objective

Evolve the agent from a general-purpose assistant into a specialized Godot expert capable of understanding the engine's API on a deep level and directly modifying project files.

### 3.2. Key Features

*   **Documentation-Aware Chat (RAG):** Implement a Retrieval-Augmented Generation (RAG) system. When a user asks a question, Kudou will first search the Godot documentation for relevant articles and API references, then pass those findings along with the user's question to the LLM. This provides highly accurate, context-specific answers about how to use Godot.
*   **Agentic Code Generation:** Grant the agent the ability to apply its suggestions directly to the open script in the `CodeEditor`, for example, by inserting a function or refactoring a block of code with user approval.

### 3.3. Relevant Files for This Phase

#### Documentation-Aware Chat (RAG)
*   `doc/classes/*.xml`: This directory (and similar ones in modules) contains the entire Godot class reference in a structured XML format. This is the **source data** for our RAG system.
*   `core/io/xml_parser.h` & `.cpp`: The primary tool for parsing the `.xml` documentation files to extract class descriptions, method signatures, properties, and signals.
*   `core/string/fuzzy_search.h` & `.cpp`: A potential built-in tool for the "retrieval" step. We can load key terms from the docs into this class to quickly find relevant topics based on the user's query. For more advanced needs, we might integrate a proper vector database library.
*   **New File:** `editor/ai/doc_retriever.h` & `.cpp`: A new class responsible for pre-processing the XML docs on startup, storing them in a searchable index (using `FuzzySearch` or another method), and providing a function to retrieve relevant doc snippets for a given query.

#### Agentic Code Generation
*   `editor/code_editor.h` & `.cpp`: This is the module for the main script editor. We will need to interact with the `CodeEdit` sub-node within it.
*   `scene/gui/text_edit.h` & `.cpp`: The base `TextEdit` class provides the low-level API for programmatically interacting with the text: `get_text()`, `insert_text_at_caret()`, `select()`, and `get_selected_text()`. This is how the agent will "type" into the editor.
*   `modules/gdscript/gdscript_language_server.h` & `.cpp`: For advanced refactoring, the agent could interface with the Language Server Protocol (LSP). This would allow it to trigger formatting, request diagnostics, or perform more complex, language-aware transformations.
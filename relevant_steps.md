# Task: Implement the AI/Agent Dock

This document outlines the plan for integrating a new "AI/Agent" dock into the Kudou editor UI. This will be the first step in creating the agentic user interface.

## 1. Objective

Create a new, dockable panel in the editor that will serve as the primary interface for AI and agentic features. This panel should appear as a new tab that can be docked on the left or right side of the main viewport, similar to the "Scene" and "Inspector" docks.

## 2. High-Level Plan

1.  **Create the AI Dock Node:**
    *   We will define a new UI node in C++, let's call it `AgentDock`, which will inherit from a `Control` node (e.g., `VBoxContainer`).
    *   This new class will contain the UI elements for our AI chat/agent interface. Initially, it can be a simple placeholder.
    *   This will require creating new files: `editor/ai/agent_dock.h` and `editor/ai/agent_dock.cpp`.
    *   We will also need a new `SCsub` file in `editor/ai/` to tell the build system how to compile these new files.

2.  **Integrate the Dock into the Editor Layout:**
    *   We will modify the main editor class, `EditorNode`, to be aware of our new `AgentDock`.
    *   In `editor_node.cpp`, we will instantiate our `AgentDock`, give it a name and an icon, and add it to one of the editor's main `DockSlot` containers. This is how all the other docks are added.

3.  **Update the Build System:**
    *   We will modify `editor/SCsub` to include our new `editor/ai` subdirectory in the build process.

## 3. Relevant Files for This Task

This list identifies the files we will need to create and modify to implement the AI dock. We should review these files to understand how existing docks are implemented before we begin.

### Files to be Created

*   `editor/ai/agent_dock.h`: The header file for our new UI panel. It will define the `AgentDock` class.
*   `editor/ai/agent_dock.cpp`: The implementation file for `AgentDock`. This is where the panel's logic and UI children will be set up.
*   `editor/ai/SCsub`: The SCons build script for the new `editor/ai` directory.

### Files to be Modified

*   `editor/editor_node.h`: The main editor class header. We will need to include `agent_dock.h` and add a member variable for our new dock, e.g., `AgentDock *agent_dock;`.
*   `editor/editor_node.cpp`: The main editor class implementation. This is the most important file. We will modify its `_ready()` or a similar initialization function to:
    1.  Create an instance of `AgentDock`.
    2.  Add it as a child to the scene tree.
    3.  Add it to one of the main dock slots (e.g., `dock_slot[DOCK_SLOT_LEFT_UR]`).
*   `editor/SCsub`: The main build script for the `editor` directory. We need to add a line here to tell SCons to process the `SCsub` file in our new `ai` subdirectory.

Once you approve this plan, we can proceed with creating and modifying these files in the next step.
# Cornifer

A runtime plugin for Unreal Engine 5 that provides an interactive, data-driven map system for travel, exploration, and event-based gameplay.

## Overview

Cornifer is a modular UE5 plugin designed to handle interactive world maps with nodes (points of interest), edges (connections between locations), and event-driven travel mechanics. The plugin supports user interaction, pathfinding, and random encounters during travel.

## Core Features

### Interactive Map Widget
- **Pan/Drag**: Click and drag to navigate the map
- **Zoom**: Scroll or pinch to zoom in/out of the map view
- **Visual Feedback**: Clear visual representation of nodes and edges

### Map Elements
- **Nodes (Points of Interest)**: Represent locations, cities, landmarks, or other important places
- **Edges (Connections)**: Visual and logical connections between nodes, defining travel routes
- **Data-Driven Design**: Map structure defined using Unreal Engine Data Assets

### User Interactions
- **Node Inspection**: Click on nodes to view detailed information
- **Edge Inspection**: Click on edges to learn about routes and travel conditions
- **Party Navigation**: Select destination nodes and initiate travel using pathfinding algorithms

### Travel System
- **Pathfinding**: Automated route calculation based on configurable cost functions
- **Random Events**: Event tables defined in `DataAsset_MapEdge` trigger text-based encounters during travel
- **Party Management**: Track party position and state during travel between nodes

## Architecture

### Key Components
- **Data Assets**: 
  - `DataAsset_MapNode`: Defines node properties, visuals, and metadata
  - `DataAsset_MapEdge`: Defines edge properties, travel cost, and random event tables

## Planned Features

### Current Status
- ✅ Plugin structure and basic setup
- 🚧 Map widget with pan and zoom
- 🚧 Node and edge data assets
- ⏳ Interactive node/edge selection
- ⏳ Pathfinding implementation
- ⏳ Travel system with party movement
- ⏳ Random event system
- ⏳ Event table configuration

### Future Enhancements
- Advanced pathfinding algorithms (A*, Dijkstra with custom heuristics)
- Conditional travel (blocked routes, time-of-day restrictions)
- Fog of war / map discovery mechanics
- Persistent map state and save/load

## Usage

### Installation
1. Clone or copy the `Cornifer` plugin into your project's `Plugins/` directory
2. Regenerate project files
3. Build the project
4. Enable the plugin in the Unreal Editor (Edit → Plugins → search "Cornifer")

### Basic Setup
1. Create a `DataAsset_MapNode` for each location on your map
2. Create `DataAsset_MapEdge` assets to connect nodes

## Design Philosophy

- **Data-Driven**: Map content should be defined in Data Assets, not hardcoded
- **Modular**: Systems should be composable and testable independently
- **Event-Driven**: Use delegates and message passing to decouple gameplay systems
- **Performance-Conscious**: Minimize allocations and optimize for real-time interaction
- **Designer-Friendly**: Expose intuitive configuration options in the editor

## Credits

Cornifer - Named after the cartographer character, representing the exploration and mapping theme of this plugin.

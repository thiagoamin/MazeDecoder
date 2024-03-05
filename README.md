# Maze Decoder

## Overview

This project is a pathfinding and image decoding application that leverages the Breadth-First Search (BFS) algorithm and a Deque data structure to navigate through mazes embedded within PNG images. The primary goal is to decode hidden paths in these images, revealing the mazes, and rendering solutions. 

<div style="display: flex; justify-content: space-around; align-items: center; flex-wrap: wrap;">
  <div style="margin: 10px;">
    <p>Figure 1: Image with Embedded Maze Hidden</p>
    <img src="https://github.com/thiagoamin/MazeDecoder/assets/122248078/7f1e541d-5c45-42f4-a2a6-5ca2bb55f011" alt="embedded maze" style="max-width: 300px;">
  </div>
  <div style="margin: 10px;">
    <p>Figure 2: Image with Embedded Maze Revealed</p>
    <img src="https://github.com/thiagoamin/MazeDecoder/assets/122248078/1c7db908-1724-4397-a5c1-3d35021cb31c" alt="solution maze maze" style="max-width: 300px;">
  </div>
  <div style="margin: 10px;">
    <p>Figure 3: Solution to Maze Found</p>
    <img src="https://github.com/thiagoamin/MazeDecoder/assets/122248078/608e78a5-3280-498b-aa4d-df1994accd43" alt="solution maze" style="max-width: 300px;">
  </div>
</div>

## Key Components

- `decoder.h/cpp`: Core of the application, responsible for decoding paths from PNG images. It initializes variables, constructs solution paths, and renders solutions and mazes.
- `queue.h/cpp`: Implements a queue data structure, essential for the BFS algorithm to hold and manage nodes yet to be explored.
- `stack.h/cpp`: Implements a stack data structure, used for storing paths or performing operations where LIFO (Last In, First Out) order is required.
- `PNG library`: Used for image manipulation, allowing the program to read from and write to PNG files.

## Algorithm and Data Structure

### Breadth-First Search (BFS)

BFS is utilized to systematically explore the maze's pixels (nodes) level by level, starting from a given start point. As it progresses, BFS ensures the shortest path to any point from the start is always found first, making it ideal for finding the shortest path through a maze. This algorithm is crucial for both decoding the hidden path in the image and rendering the maze itself by exploring all accessible nodes.

### Deque Data Structure

A Deque (Double-Ended Queue) is used to support both queue and stack operations, allowing elements to be added or removed from either end efficiently. This flexibility is crucial in scenarios where the algorithm might need to switch between FIFO (First In, First Out) queue behavior for BFS traversal and LIFO stack behavior for path reconstruction or other operations. The Deque is implemented in a manner that optimizes space and time complexity, dynamically resizing itself and minimizing memory overhead.

## Building and Running

To build the project, ensure you have a C++ compiler and the PNG library installed. Compile the source files together with the PNG library. An example compilation command might look like this:

```sh
g++ -o decoder decoder.cpp queue.cpp stack.cpp -lpng

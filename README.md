# Boids Simulation with OpenMP

This project is a parallelized implementation of a Boids simulation using OpenMP for concurrency and SFML for real-time visualization. The purpose of this project is to demonstrate the benefits of parallel processing in computationally intensive tasks like simulating the flocking behavior of birds (boids).
## Demo

You can watch a demo of the Boids Simulation in action on YouTube:

[![Watch the video](https://img.youtube.com/vi/5O4LElh-8Cg/maxresdefault.jpg)](https://youtu.be/5O4LElh-8Cg)


## Features

- **Parallel Processing**: The core computation for the boids' behavior (separation, alignment, cohesion) is parallelized using OpenMP.
- **Real-time Visualization**: SFML is used to render the boids and their movements in real-time.
- **Performance Comparisons**: The project includes both a parallel (OpenMP) and a non-parallel version of the simulation to allow comparison of performance under different numbers of boids.

## Boids Rules Implemented

1. **Separation**: Boids steer to avoid crowding neighbors.
2. **Alignment**: Boids steer to align with the average heading of neighbors.
3. **Cohesion**: Boids steer to move towards the average position of neighbors.

## Performance Comparison

The execution times of the parallelized and non-parallelized versions of the simulation are compared across various numbers of boids (100, 500, 1000). OpenMP shows a significant speedup when handling a large number of boids.

## Requirements

- **SFML**: For visualization
- **OpenMP**: For parallelization

## Running the Project

1. Clone the repository:
    ```
    git clone https://github.com/yourusername/boids_simulation_openmp.git
    ```
2. Build and run the project using your preferred IDE or command line, ensuring SFML and OpenMP are correctly configured.

## Authors

- Silviu Robert Plesoiu

# WindowCubePhysics

## Overview
WindowCubePhysics is a C++ project that simulates the interaction between a window and a cube using physics. The project uses GLEW, GLFW, and Bullet Physics libraries to handle graphics and physics calculations.

## Features
- Real-time physics simulation
- Interaction between window movement and cube
- Configurable physics settings

## Prerequisites
- CMake
- GLEW
- GLFW
- Bullet Physics

## Installation

### Clone the Repository
```sh
git clone https://github.com/srmrkN/WindowCubePhysics.git
cd WindowCubePhysics
```

### Install Dependencies

#### Ubuntu
```sh
sudo apt-get update
sudo apt-get install libglew-dev libglfw3-dev libbullet-dev
```

#### Windows (using vcpkg)
```sh
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
./vcpkg integrate install
./vcpkg install glew glfw3 bullet3
```

### Build the Project
```sh
mkdir build
cd build
cmake ..
make
```

## Usage
After building the project, you can run the executable:
```sh
./WindowCubePhysics
```


## Configuration
You can configure various physics settings in the `types.h` file under the `PhysicsSettings` struct.

## Contributing
1. Fork the repository
2. Create a new branch (`git checkout -b feature-branch`)
3. Commit your changes (`git commit -am 'Add new feature'`)
4. Push to the branch (`git push origin feature-branch`)
5. Create a new Pull Request

## License
This project is licensed under the MIT License. See the `LICENSE` file for more details.

## Acknowledgements
- [GLEW](http://glew.sourceforge.net/)
- [GLFW](https://www.glfw.org/)
- [Bullet Physics](https://pybullet.org/)

## Contact
For any questions or suggestions, please open an issue on the GitHub repository.
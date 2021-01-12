# Caching Project

## Building

In order to build the project, you will need the following:

- Conan
- Cmake

```bash
# Install the required Packages
sudo apt-get install cmake build-essential g++ gcc python3 python3-pip make

# Install Conan and add it to PATH
pip3 install conan
source ~/.profile

# Go to build directory
cd <project_path>
mkdir build
cd build

# Install Conan Packages
conan install .. -s compiler=gcc -s compiler.libcxx=libstdc++11 -s compiler.version=9

# Create cmake project
cmake ..

# Build the executable
make
```

### Binaries

The binaries are located in `<build>/bin`

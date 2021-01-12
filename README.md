# Caching Project

- [Caching Project](#caching-project)
  - [Building](#building)
    - [Binaries](#binaries)
  - [Building with Docker](#building-with-docker)
    - [Running the Docker Image](#running-the-docker-image)
  - [Custom Config and Testing file](#custom-config-and-testing-file)

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

## Building with Docker

Simply run the command

```bash
cd <project_path>
docker build -t caching_server . 
```

### Running the Docker Image

You can either just let it run on it's own with

```bash
docker run -it caching_server
```

Or you can enter the VM with 

```bash
docker run -it --entrypoint /bin/bash caching_server

# And then ./caching_server
```

## Custom Config and Testing file

To use a custom Config and Testing file natively you can just append `--config` and `--test` followed by the file paths

So, for example:

```bash
./caching_server --config=path/to/my/config/file --test=path/to/test/file
```

You can also do the same thing for docker, just a bit differently

```bash
docker run --read-only -v /mount/directory:/mount/to -it caching_server --config=/mount/to/config/path --test=/mount/to/test/path
```

If `config.yaml` is in `/settings/config.yaml` and `test.yaml` is in `/settings/tests/test.yaml`, you would execute the following command:

```bash
docker run --read-only -v /settings:/settings -it caching_server --config=/settings/config.yaml --test=/settings/tests/test.yaml
```
# Dockerfile for building the Weno Fit OS
FROM ubuntu:22.04

# Avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install all build dependencies required by Buildroot and our project
RUN apt-get update && apt-get install -y 
    build-essential 
    gcc 
    g++ 
    make 
    git 
    wget 
    tar 
    cpio 
    unzip 
    rsync 
    bc 
    libncurses-dev 
    flex 
    bison 
    python3 
    npm 
    qemu-system-arm 
    qemu-system-x86

# Set the working directory inside the container
WORKDIR /project

# The default command when the container starts
CMD ["bash"]

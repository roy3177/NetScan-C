
# Base image with GCC compiler
FROM gcc:13

# Set working directory inside container
WORKDIR /app

# Copy all project files to the container
COPY . .

# Compile the project using makefile
RUN make

# Default command to run the scanner
CMD ["./netscan", "127.0.0.1"]

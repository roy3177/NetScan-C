 # 🔎 NetScan-C – Multi-threaded Port Scanner in C

 ## 👨‍💻 Author : Roy Meoded


**NetScan-C** is a multi-threaded TCP port scanner written in C.  
It scans a range of ports on a given IP address, detects which are open, and optionally prints banner information from open services.
  

##  Features

- Scan a custom IP address over a specified port range
- Identify open, closed, or unresponsive ports
- Grab service banners from open ports
- Display results in terminal and export to HTML (`report.html`)
- Parallel scanning using `select()` for efficiency


## Project Structure

- `scanner.c` – Main file that handles CLI arguments and runs the scan loop
- `port_utils.c` – Functions for checking if a port is open
- `services.c` – Maps known port numbers to their service names
- `parallel_scan.c` – Handles multi-threaded scanning using select
- `banner.c` – Contains logic to grab banner information from open ports

## Running with Docker 🐳

The project can be built and run  inside a Docker container.

**Step 1:** Build the Docker image:
```bash 
sudo docker build -t netscan .

```
**Step 2:**Run the scanner inside Docker:
``` bash
sudo docker run --rm netscan ./netscan <IP> <START_PORT> <END_PORT>
```

**Example:** 
```bash
sudo docker run --rm netscan ./netscan 127.0.0.1 1 100
```



> ℹ️ **Note:** Inside Docker, 127.0.0.1 refers to the container itself.  
> To scan your actual local network (e.g. 192.168.x.x), you may need to configure Docker networking.

```markdown



## 📝 HTML Report 

The project includes a basic HTML file (`report.html`) that can be extended to display scan results in a user-friendly webpage.  
Currently, it can serve as:
- A placeholder for exported scan summaries
- A base for building a GUI in the browser
- An integration target for future web-based visualization

NetScan-C
├── include
│   ├── parallel_scan.h
│   ├── banner.h
│   ├── port_utils.h
│   ├── services.h 
│   
├── src
│   ├── parallel_scan.c
│   ├── banner.c
│   ├── port_utils.c
│   ├── services.c
│   ├── scanner.c
│
├── report.html
├── makefile
├── Dockerfile
└── README.md




**Compilation:** To compile, using the provided Makefile:
```bash 
make
```
**Running the Main Program:** To execute demos and examples:
``` bash
./netscan <IP> <start_port> <end_port>
```


**Cleaning Files:** To remove compile files:
```bash
make clean
```


```markdown
## 🖥️ Sample Output

Scanning 127.0.0.1 from port 0 to 100...
Port 22 (SSH) is OPEN
    [BANNER] SSH-2.0-OpenSSH_9.6p1 Ubuntu-3ubuntu13.12
Port 80 (HTTP) is OPEN
    [BANNER] HTTP/1.0 200 OK
Server: SimpleHTTP/0.6 Python/3.12.3
```



## 👨‍💻 Author

**Roy Meoded**
  

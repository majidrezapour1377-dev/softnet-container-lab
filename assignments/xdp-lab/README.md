# XDP Per-Protocol Byte Counter

## Overview

This project implements an XDP eBPF program that counts the total number of bytes received for different Layer 4 protocols.

The supported protocols are:

- TCP
- UDP
- ICMP

The counters are stored inside a BPF array map called `bytes_map`.

---
## Project Objective

The objective of this assignment is to implement an XDP eBPF program that counts the total number of bytes received for TCP, UDP, and ICMP packets using a BPF array map, and to verify its functionality using XDP tools and bpftool.

## Requirements

Before running the project, make sure the following tools are installed:

- Docker
- Containerlab
- bpftool
- clang / LLVM
- Linux kernel with XDP support

---

## Step 1 - Clone the Repository

```bash
git clone https://github.com/majidrezapour1377-dev/softnet-container-lab.git
cd softnet-container-lab

---

## Step 2 - Build the eBPF Program

Compile the source code.

```bash
cd containerlab/xdp-lab/src
make
```

Expected output:

![Build](screenshots/build.png)

---

## Step 3 - Deploy the Topology

Deploy the containerlab topology.

```bash
cd ..
./deploy.sh
```

Expected output:

![Deploy](screenshots/deploy.png)

---

## Step 4 - Attach the XDP Program

Attach the compiled eBPF object to **node1**.

```bash
docker exec clab-xdp-lab-node1 \
ip link set dev eth1 xdp obj /work/bpf/test_xdp.bpf.o sec xdp
```

Verify that the program is attached correctly.

```bash
docker exec clab-xdp-lab-node1 \
bpftool net show dev eth1
```

Expected output:

![Attach](screenshots/attach.png)

---

## Step 5 - Generate Network Traffic

Generate ICMP traffic from **node2**.

```bash
docker exec clab-xdp-lab-node2 \
ping -c 5 10.0.3.1
```

Expected output:

![Ping](screenshots/ping.png)

---

## Step 6 - Read the BPF Map

Read the values stored inside the BPF map.

```bash
docker exec clab-xdp-lab-node1 \
bpftool map dump name bytes_map
```

Expected output:

![Map](screenshots/map.png)

The map stores:

| Key | Protocol |
|-----|----------|
| 0 | TCP |
| 1 | UDP |
| 2 | ICMP |

---

## Step 7 - Read Debug Output

Open one terminal and run:

```bash
docker exec clab-xdp-lab-node1 \
timeout 5 cat /sys/kernel/debug/tracing/trace_pipe
```

Open another terminal and generate traffic again:

```bash
docker exec clab-xdp-lab-node2 \
ping -c 3 10.0.3.1
```

Expected output:

![Trace](screenshots/trace.png)

---

## Step 8 - Detach the XDP Program

```bash
docker exec clab-xdp-lab-node1 \
ip link set dev eth1 xdp off
```

---

## Step 9 - Destroy the Topology

```bash
./destroy.sh
```

---

## Source Code

Main source file:

```text
containerlab/xdp-lab/src/test_xdp.bpf.c
```

---

## Screenshots

All execution screenshots are available in the `screenshots` directory.

---

## Author

Majid Rezapour

Software Networks Project

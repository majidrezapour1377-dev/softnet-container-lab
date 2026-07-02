# XDP Per-Protocol Byte Counter

## Overview

This project implements an XDP eBPF program that counts the total number of bytes for different Layer 4 protocols.

The program supports:

- TCP
- UDP
- ICMP

Each protocol has its own counter stored inside a BPF array map.

---

## BPF Map

The program uses an array map called `bytes_map`.

| Key | Protocol |
|----:|----------|
| 0 | TCP |
| 1 | UDP |
| 2 | ICMP |

Each entry stores the total number of bytes received for that protocol.

---

## Project Structure

```text
xdp-lab
├── README.md
├── screenshots/
└── src/
    └── test_xdp.bpf.c
```

---

## Requirements

- Docker
- Containerlab
- bpftool
- clang / LLVM
- Linux kernel with XDP support

---

## Build

From the repository root:

```bash
cd containerlab/xdp-lab/src
make
```

---

## Deploy

```bash
cd ..
./deploy.sh
```

---

## Attach the XDP Program

```bash
docker exec clab-xdp-lab-node1 \
ip link set dev eth1 xdp obj /work/bpf/test_xdp.bpf.o sec xdp
```

---

## Verify

```bash
docker exec clab-xdp-lab-node1 \
bpftool net show dev eth1
```

---

## Generate ICMP Traffic

```bash
docker exec clab-xdp-lab-node2 \
ping -c 5 10.0.3.1
```

---

## Read the BPF Map

```bash
docker exec clab-xdp-lab-node1 \
bpftool map dump name bytes_map
```

Example output:

```text
[
  {
    "key": 0,
    "value": 0
  },
  {
    "key": 1,
    "value": 0
  },
  {
    "key": 2,
    "value": 490
  }
]
```

---

## Debug Output

```bash
docker exec clab-xdp-lab-node1 \
timeout 5 cat /sys/kernel/debug/tracing/trace_pipe
```

Example:

```text
ICMP bytes: 98
ICMP bytes: 196
ICMP bytes: 294
ICMP bytes: 392
ICMP bytes: 490
```

---

## Detach the Program

```bash
docker exec clab-xdp-lab-node1 \
ip link set dev eth1 xdp off
```

---

## Destroy the Topology

```bash
./destroy.sh
```

---

## Screenshots

Execution screenshots are available in the `screenshots` folder.

---

## Author

Majid Rezapour

Software Networks Project

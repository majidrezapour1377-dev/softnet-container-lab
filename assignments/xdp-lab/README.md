# XDP Lab

## Overview

This project implements an XDP eBPF program that counts the number of bytes for TCP, UDP, and ICMP packets using a BPF array map.

## Features

- XDP program attached to the network interface
- Counts TCP bytes
- Counts UDP bytes
- Counts ICMP bytes
- Stores counters in a BPF array map
- Displays debug messages using `bpf_printk()`

## BPF Map

The program uses a BPF array map called `bytes_map`.

| Key | Protocol |
|-----|----------|
| 0 | TCP |
| 1 | UDP |
| 2 | ICMP |

## Project Structure

```text
assignments/xdp-lab
├── README.md
└── screenshots/
```

## Execution Steps

1. Compile the eBPF program.
2. Deploy the topology.
3. Attach the XDP program.
4. Verify the attachment.
5. Generate network traffic.
6. Read the BPF map.
7. Inspect `trace_pipe` output.
8. Detach the XDP program.
9. Destroy the topology.

## Results

The program successfully:

- Attached the XDP program to the interface.
- Counted bytes for TCP, UDP and ICMP packets.
- Stored counters inside the BPF map.
- Printed debug messages using `bpf_printk()`.

## Screenshots

Execution screenshots are available in the `screenshots` folder.

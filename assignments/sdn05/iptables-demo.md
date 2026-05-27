# SDN05 - iptables FORWARD chain demo

## Goal

Demonstrate packet filtering using the Linux FORWARD chain.

## Topology

hs1 <--> rt1 <--> hs2

## Initial Test

Both hosts could ping each other successfully.

## Add DROP rule

The following rule was added on rt1:

```bash
iptables -A FORWARD -s 10.0.1.2 -d 10.0.2.2 -j DROP

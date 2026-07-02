#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>

/* Ethernet type for IPv4 packets */
#define ETH_P_IP 0x0800

/* Keys used in the BPF map */
#define KEY_TCP  0
#define KEY_UDP  1
#define KEY_ICMP 2

/* BPF array map that stores the total number of bytes
 * for each Layer 4 protocol.
 *
 * key 0 -> TCP bytes
 * key 1 -> UDP bytes
 * key 2 -> ICMP bytes
 */
struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __uint(max_entries, 3);
    __type(key, __u32);
    __type(value, __u64);
} bytes_map SEC(".maps");

SEC("xdp")
int xdp_counter(struct xdp_md *ctx)
{
    /* Packet start and end addresses */
    void *data = (void *)(long)ctx->data;
    void *data_end = (void *)(long)ctx->data_end;

    /* Calculate packet size */
    __u64 packet_size = data_end - data;

    /* Read Ethernet header */
    struct ethhdr *eth = data;

    /* Check that the Ethernet header is valid */
    if ((void *)(eth + 1) > data_end)
        return XDP_PASS;

    /* Ignore non-IPv4 packets */
    if (bpf_ntohs(eth->h_proto) != ETH_P_IP)
        return XDP_PASS;

    /* Read IPv4 header */
    struct iphdr *ip = (void *)(eth + 1);

    /* Check that the IP header is valid */
    if ((void *)(ip + 1) > data_end)
        return XDP_PASS;

    __u32 key;

    /* Select the correct map entry according to the protocol */
    if (ip->protocol == IPPROTO_TCP) {
        key = KEY_TCP;
    } else if (ip->protocol == IPPROTO_UDP) {
        key = KEY_UDP;
    } else if (ip->protocol == IPPROTO_ICMP) {
        key = KEY_ICMP;
    } else {
        /* Ignore all other protocols */
        return XDP_PASS;
    }

    /* Get the corresponding counter from the BPF map */
    __u64 *counter = bpf_map_lookup_elem(&bytes_map, &key);

    if (counter) {
        /* Add the current packet size */
        *counter += packet_size;

        /* Print the updated counter for debugging */
        if (key == KEY_TCP)
            bpf_printk("TCP bytes: %llu", *counter);
        else if (key == KEY_UDP)
            bpf_printk("UDP bytes: %llu", *counter);
        else if (key == KEY_ICMP)
            bpf_printk("ICMP bytes: %llu", *counter);
    }

    /* Let the packet continue through the network stack */
    return XDP_PASS;
}

/* Required license for eBPF programs */
char _license[] SEC("license") = "GPL";

